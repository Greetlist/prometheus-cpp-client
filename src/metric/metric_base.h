#ifndef __METRIC_BASE_H_
#define __METRIC_BASE_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <initializer_list>
#include <sstream>
#include <atomic>

using LabelList = std::initializer_list<std::pair<const std::string, std::string>>;
#define HTTP_CRLF "\r\n"

template <typename Value>
class MetricBase {
public:
  MetricBase(const std::string& metric_name, const std::string& metric_help, LabelList labels) : metric_name_(metric_name), metric_labels_(labels) {
    metric_help_string_ = "# HELP " + metric_name + " " + metric_help;
    name_and_label_ = GenLabelString();
  }
  ~MetricBase() = default;
  virtual std::string Collect() {return "";}
  void AddLabel(LabelList l) {
    for (auto& p : l) {
      metric_labels_.insert(p);
    }
    name_and_label_ = GenLabelString();
  }
  void Init() {
    GenPromeTypeStr();
  }
protected:
  virtual void GenPromeTypeStr() { //Type need specified Data Type
    metric_type_string_ = "";
  }
  inline std::string& GetLabelString(bool is_dynamic) {
    if (is_dynamic) {
      name_and_label_ = GenLabelString();
    }
    return name_and_label_;
  }
  std::string GenLabelString() {
    std::stringstream ss;
    ss << metric_name_ << "{";
    auto it = metric_labels_.begin();
    while (true) {
      ss << (*it).first << "=\"" << (*it).second << "\"";
      if (++it != metric_labels_.end()) {
        ss << ",";
      }
      if (it == metric_labels_.end()) {
        break;
      }
    }
    ss << "}";
    return ss.str();
  }
protected:
  std::string metric_name_;
  std::string metric_help_string_;
  std::string metric_type_string_;
  std::string name_and_label_;
  std::unordered_map<std::string, std::string> metric_labels_;
};

#endif
