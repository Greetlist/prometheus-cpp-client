#ifndef __COUNTER_H_
#define __COUNTER_H_

template <typename Value>
class Counter : public MetricBase<Value> {
public:
  Counter(const std::string& metric_name, const std::string& metric_help, LabelList labels);
  ~Counter();
  virtual std::string Collect() override;
  Value& GetValue();
  void SetValue(Value& v);
  operator++(Value incr);
private:
  void GenPromeType(bool is_dynamic);

};

#endif
