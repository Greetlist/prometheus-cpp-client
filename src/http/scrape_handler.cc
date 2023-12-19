#include "http/scrape_handler.h"

bool ScrapeHandler::handleGet(CivetServer* server, struct mg_connection* conn) {
  const struct mg_request_info* req_info = mg_get_request_info(conn);
  mg_printf(conn, "HTTP/1.1 200 OK" HTTP_CRLF);
  mg_printf(conn, "Content-Type: text/html" HTTP_CRLF);
  mg_printf(conn, "Connection: close" HTTP_CRLF HTTP_CRLF);

  Exporter* e = reinterpret_cast<Exporter*>(server);
  auto& metric_map = e->GetMetricMap();
  for (auto [metric, i] : metric_map) {
    mg_printf(conn, metric->Collect());
  }
  return true;
}
