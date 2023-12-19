#ifndef __GUAGE_H_
#define __GUAGE_H_

template <typename Value>
class Guage : public MetricBase {
public:
  Guage(const std::string& metric_name, const std::string& metric_help, LabelList labels);
  ~Guage() = default;
  virtual std::string Collect() override;
  void SetValue(Value& v);
  void operator++(int);
  void operator--(int);
  void AddValue(Value incr);
  void SubValue(Value sub);
private:
  virtual void GenPromeTypeStr() override;
  std::atomic<Value> value_{};
};

template <typename Value>
Guage<Value>::Guage(const std::string& metric_name, const std::string& metric_help, LabelList labels) : MetricBase(metric_name, metric_help, labels), value_(0) {
}

template <typename Value>
std::string Guage<Value>::Collect() {
  std::string res{""};
  res += metric_help_string_ + HTTP_CRLF;
  res += metric_type_string_ + HTTP_CRLF;
  res += name_and_label_ + " ";
  res += std::to_string(value_);
  res += HTTP_CRLF;
  return res;
}

template <typename Value>
void Guage<Value>::SetValue(Value& v) {
  value_ = v;
}

template <typename Value>
void Guage<Value>::operator++(int) {
  value_.fetch_add(1);
}

template <typename Value>
void Guage<Value>::operator--(int) {
  //value_.fetch_;
}

template <typename Value>
void Guage<Value>::AddValue(Value incr) {
  //value_ += incr;
}

template <typename Value>
void Guage<Value>::SubValue(Value sub) {
  //value_ -= sub;
}

template <typename Value>
void Guage<Value>::GenPromeTypeStr() {
  metric_type_string_ = "# TYPE " + metric_name_ + " guage";
}

#endif
