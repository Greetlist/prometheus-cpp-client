#ifndef __SUMMARY_H_
#define __SUMMARY_H_

#include <initializer_list>
#include <vector>
#include <cassert>
#include <mutex>

template <typename Value>
class Summary : public MetricBase {
public:
  Summary(const std::string& metric_name, const std::string& metric_help, LabelList labels, const std::initializer_list<Value>& buckets);
  ~Summary() = default;
  void Observe(const Value v);
  virtual std::string Collect() override;
private:
  int FindBucketsIndex(const Value v);
  bool CheckBucketsIncrement();
  virtual void GenPromeTypeStr() override;
  std::vector<Value> buckets_;
  std::vector<int> buckets_counter_;
  std::atomic<int> InfinityValue_{0};
  std::mutex counter_mutex_;
};

template <typename Value>
Summary<Value>::Summary(const std::string& metric_name, const std::string& metric_help, LabelList labels, const std::initializer_list<Value>& buckets) : MetricBase(metric_name, metric_help, labels), buckets_(buckets) {
  assert(CheckBucketsIncrement());
  buckets_counter_ = std::vector<int>(buckets_.size() + 1, 0);
}

template <typename Value>
void Summary<Value>::Observe(const Value v) {
  int index = FindBucketsIndex(v);
  {
    std::lock_guard<std::mutex> lk(counter_mutex_);
    int counter_size = buckets_counter_.size();
    for (int i = index; i < counter_size; ++i) {
      buckets_counter_[i]++;
    }
  }
  InfinityValue_.fetch_add(1);
}

template <typename Value>
std::string Summary<Value>::Collect() {
  std::string res{""};
  res += metric_help_string_ + HTTP_CRLF;
  res += metric_type_string_ + HTTP_CRLF;

  {
    std::lock_guard<std::mutex> lk(counter_mutex_);
    int bucket_size = buckets_.size();
    int current_inf_value = InfinityValue_.load();
    for (int i = 0; i < bucket_size + 1; ++i) {
      if (i == bucket_size) {
        metric_labels_["le"] = "Inf";
        std::string cur_name_and_label = GenLabelString();
        res += cur_name_and_label + " ";
        res += std::to_string(InfinityValue_);
      } else {
        metric_labels_["le"] = std::to_string(buckets_[i]);
        std::string cur_name_and_label = GenLabelString();
        res += cur_name_and_label + " ";
        res += current_inf_value != 0 ? std::to_string(buckets_counter_[i] / current_inf_value) : "0";
      }
      res += HTTP_CRLF;
    }
  }

  return res;
}

template <typename Value>
int Summary<Value>::FindBucketsIndex(const Value v) {
  int start = 0, end = buckets_.size();
  int size = buckets_.size();
  while (true) {
    int mid = (start + end) / 2;
    if (buckets_[mid] > v && mid > 0 && buckets_[mid-1] < v) {
      return mid;
    } else if (buckets_[mid] > v && mid > 0) {
      end = mid;
    } else if (buckets_[mid] > v && mid == 0) {
      return 0;
    } else if (buckets_[mid] < v && mid < size - 1 && buckets_[mid+1] > v) {
      return mid + 1;
    } else if (buckets_[mid] < v && mid < size) {
      start = mid + 1;
    } else if (buckets_[mid] < v && mid == size) {
      return size;
    }
  }
  return 0;
}

template <typename Value>
bool Summary<Value>::CheckBucketsIncrement() {
  int size = buckets_.size();
  for (int i = 0; i < size - 1; ++i) {
    if (buckets_[i] > buckets_[i+1]) return false;
  }
  return true;
}

template <typename Value>
void Summary<Value>::GenPromeTypeStr() {
  metric_type_string_ = "# TYPE " + metric_name_ + "summary";
}

#endif
