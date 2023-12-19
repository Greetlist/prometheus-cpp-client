#ifndef __EXPORTER_H_
#define __EXPORTER_H_

#include <CivetServer.h>
#include <string>
#include <unordered_map>
#include "http/scrape_handler.h"

class Exporter : public CivetServer {
public:
  Exporter(const std::vector<std::string> &options);
  ~Exporter();
  void Init();
  void AddMetric(void*);
  void DeleteMetric(void*);
  std::unordered_map<void*, int>& GetMetricMap();
private:
  std::string listen_addr_;
  int threads_num_;
  std::unordered_map<void*, int> metric_map_;
};

#endif
