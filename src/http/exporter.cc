#include "http/exporter.h"

Exporter::Exporter(const std::vector<std::string> &options) : CivetServer(options) {}

Exporter::~Exporter() {
}

void Exporter::Init() {
  ScrapeHandler sh;
  this->addHandler("/metrics", sh);
}

void Exporter::AddMetric(void* metric_ptr) {
  metric_map_.insert(std::make_pair(metric_ptr, 1));
}

void Exporter::DeleteMetric(void* metric_ptr) {
  metric_map_.erase(metric_ptr);
}

std::unordered_map<void*, int>& Exporter::GetMetricMap() {
  return metric_map_;
}

