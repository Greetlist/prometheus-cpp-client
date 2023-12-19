#ifndef __EXPORTER_H_
#define __EXPORTER_H_

#include <CivetServer.h>
#include <string>
#include <unordered_map>
#include <chrono>
#include "metric/metric_base.h"

class Exporter : public CivetServer {
public:
  Exporter(const std::vector<std::string> &options);
  ~Exporter();
  void Init();
  void AddMetric(MetricBase*);
  void DeleteMetric(MetricBase*);
  std::unordered_map<MetricBase*, int>& GetMetricMap();
private:
  std::string listen_addr_;
  int threads_num_;
  std::unordered_map<MetricBase*, int> metric_map_;
};

class ScrapeHandler : public CivetHandler {
public:
  ScrapeHandler(Exporter* e) : exporter_(e) {}
  bool handleGet(CivetServer* server, struct mg_connection* conn) override;
private:
  Exporter* exporter_;
};

#endif
