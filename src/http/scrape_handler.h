#ifndef __SCRAPE_HANDLER_H_
#define __SCRAPE_HANDLER_H_

#include <CivetServer.h>
#include "metric/metric_base.h"

class Exporter;

class ScrapeHandler : public CivetHandler {
public:
  bool handleGet(CivetServer* server, struct mg_connection* conn) override;
};

#endif
