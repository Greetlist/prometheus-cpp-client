#include "http/exporter.h"

Exporter::Exporter(const std::vector<std::string> &options) : CivetServer(options) {}

Exporter::~Exporter() {
}

void Exporter::Init() {
  ScrapeHandler* sh = new ScrapeHandler(this);
  this->addHandler("/metrics", sh);
}

void Exporter::AddMetric(MetricBase* metric_ptr) {
  metric_map_.insert(std::make_pair(metric_ptr, 1));
}

void Exporter::DeleteMetric(MetricBase* metric_ptr) {
  metric_map_.erase(metric_ptr);
}

std::unordered_map<MetricBase*, int>& Exporter::GetMetricMap() {
  return metric_map_;
}

bool ScrapeHandler::handleGet(CivetServer* server, struct mg_connection* conn) {
  mg_printf(conn, "HTTP/1.1 200 OK" HTTP_CRLF);
  mg_printf(conn, "Content-Type: text/html" HTTP_CRLF);
  mg_printf(conn, "Connection: close" HTTP_CRLF HTTP_CRLF);

  auto& metric_map = exporter_->GetMetricMap();
  for (auto [metric, i] : metric_map) {
    mg_printf(conn, "%s", metric->Collect().c_str());
  }
  return true;
}
