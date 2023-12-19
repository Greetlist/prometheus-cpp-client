#ifndef __COUNTER_H_
#define __COUNTER_H_

#include "metric/metric_base.h"

template <typename Value>
class Counter : public MetricBase<Value> {
public:
  Counter(const std::string& metric_name, const std::string& metric_help, LabelList labels);
  ~Counter() = default;
  virtual std::string Collect() override;
  void SetValue(Value& v);
  void operator++(int);
  void AddValue(Value incr);
private:
  virtual void GenPromeTypeStr() override;
  std::atomic<Value> value_{};
};

template <typename Value>
Counter<Value>::Counter(const std::string& metric_name, const std::string& metric_help, LabelList labels) : MetricBase<Value>(metric_name, metric_help, labels), value_(0) {
}

template <typename Value>
std::string Counter<Value>::Collect() {
  std::string res{""};
  res += MetricBase<Value>::metric_help_string_ + HTTP_CRLF;
  res += MetricBase<Value>::metric_type_string_ + HTTP_CRLF;
  res += MetricBase<Value>::name_and_label_ + " ";
  res += std::to_string(value_);
  return res;
}

template <typename Value>
void Counter<Value>::SetValue(Value& v) {
  value_.store(v);
}

template <typename Value>
void Counter<Value>::operator++(int) {
  //value_.fetch_add(1);
  value_++;
}

template <typename Value>
void Counter<Value>::AddValue(Value incr) {
  value_.fetch_add(incr);
}

template <typename Value>
void Counter<Value>::GenPromeTypeStr() {
  MetricBase<Value>::metric_type_string_ = "# TYPE " + MetricBase<Value>::metric_name_ + " counter";
}

#endif
