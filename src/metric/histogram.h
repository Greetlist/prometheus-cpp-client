#ifndef __GUAGE_H_
#define __GUAGE_H_

template <typename Value>
class Histogram : public MetricBase<Value> {
public:
  Histogram(const std::string& metric_name, const std::string& metric_help, LabelList labels, const std::vector<Value>& buckets);
  ~Histogram() = default;
  void Observe(const Value v);
  virtual std::string Collect() override;
private:
  int FindBucketsIndex(const Value v);
  bool CheckBucketsIncrement();
  virtual void GenPromeTypeStr() override;
  std::vector<Value> buckets_;
  std::vector<int> buckets_counter_;
  std::atomic<int> InfinityValue_{0};
};

template <typename Value>
Histogram<Value>::Histogram(const std::string& metric_name, const std::string& metric_help, LabelList labels, const std::vector<Value>& buckets) : MetricBase<Value>(metric_name, metric_help, labels), buckets_(buckets) {
  assert(CheckBucketsIncrement());
  buckets_counter_ = std::vector<int>(buckets_.size() + 1, 0);
}

template <typename Value>
void Histogram<Value>::Observe(const Value v) {
  int index = FindBucketsIndex(v);
  int counter_size = buckets_counter_.size();
  for (int i = index; i < counter_size; ++i) {
    buckets_counter_[i]++;
  }
  InfinityValue_.Add(1);
}

template <typename Value>
std::string Histogram<Value>::Collect() {
  std::string res{""};
  res += MetricBase<Value>::metric_help_string_ + "\n";
  res += MetricBase<Value>::metric_type_string_ + "\n";

  int bucket_size = buckets_.size();
  for (int i = 0; i < bucket_size + 1; ++i) {
    if (i == bucket_size) {
      metric_labels["le"] = "Inf";
      std::string cur_name_and_label = GenLabelString();
      res += MetricBase<Value>::name_and_label_ + " ";
      res += std::to_string(buckets_counter_[i]);
    } else {
      metric_labels["le"] = std::to_string(bucket_[i]);
      std::string cur_name_and_label = GenLabelString();
      res += MetricBase<Value>::name_and_label_ + " ";
      res += std::to_string(InfinityValue_);
    }
  }
}

template <typename Value>
int Histogram<Value>::FindBucketsIndex(const Value v) {
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
      return mid;
    } else if (buckets_[mid] < v && mid < size) {
      start = mid + 1;
    } else if (buckets_[mid] < v && mid == size) {
      return size;
    }
  }
  return 0;
}

template <typename Value>
bool Histogram<Value>::CheckBucketsIncrement() {
  int size = buckets_.size();
  for (int i = 0; i < size - 1; ++i) {
    if (buckets_[i] > buckets_[i+1]) return false;
  }
  return true;
}

template <typename Value>
void Histogram<Value>::GenPromeTypeStr() {
  MetricBase<Value>::metric_type_string_ = "# TYPE " + MetricBase<Value>::metric_name_ + " histogram";
}

#endif
