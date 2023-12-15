#ifndef __METRIC_BASE_H_
#define __METRIC_BASE_H_

#include <string>
#include <unordered_map>
#include <initializer_list>

template <typename Value>
class MetricBase {
using std::initializer_list<std::pair<std::string, std::string>> = LabelList;
public:
MetricBase(const std::string& metric_name, const std::string& metric_help, LabelList labels) : metric_name_(metric_name), metric_help_(metric_help), metric_labels_(labels), value_(0), is_dynamic_label_mode_(false) {}
  ~MetricBase();
  virtual std::string Collect() {return "";}
  void SetValue(Value& v) {value_ = v;}
  void AddLabel(LabelList l) {
    for (auto& p : LabelList) {
      metric_labels.insert(p);
    }
    is_dynamic_label_mode_ = true;
  }
private:
  void GenPromeHelp(); //Help just need metric_name and metric_help
  //void GenPromeType(); //Type need specified Data Type
  void GenMetricAndLabel(bool is_dynamic) {}
private:
  std::string metric_name_;
  std::string metric_help_;
  std::unordered_map<std::string, std::string> metric_labels_;
  Value value_;
  bool is_dynamic_label_mode_;
};

#endif
