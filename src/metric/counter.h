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
  void AddValue(Value& incr);
private:
  virtual void GenPromeTypeStr() override;
};

template <typename Value>
Counter<Value>::Counter(const std::string& metric_name, const std::string& metric_help, LabelList labels) : MetricBase<Value>(metric_name, metric_help, labels) {
}

template <typename Value>
std::string Counter<Value>::Collect() {
  std::string res = \
      MetricBase<Value>::metric_help_string_ + "\n" + \
      MetricBase<Value>::metric_type_string_ + "\n" + \
      MetricBase<Value>::name_and_label_ + " " +
      std::to_string(MetricBase<Value>::value_);
  return res;
}

template <typename Value>
void Counter<Value>::SetValue(Value& v) {
  MetricBase<Value>::value_ = v;
}

template <typename Value>
void Counter<Value>::operator++(int) {
  MetricBase<Value>::value_++;
}

template <typename Value>
void Counter<Value>::AddValue(Value& incr) {
  MetricBase<Value>::value_ += incr;
}

template <typename Value>
void Counter<Value>::GenPromeTypeStr() {
  MetricBase<Value>::metric_type_string_ = "# TYPE " + MetricBase<Value>::metric_name_ + " counter";
}

#endif
