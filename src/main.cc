#include <iostream>
#include <thread>
#include <chrono>
#include <gflags/gflags.h>
#include "metric/counter.h"
#include "metric/guage.h"
#include "metric/histogram.h"
#include "http/exporter.h"

using namespace std;

DEFINE_string(listen_addr, "0.0.0.0:10000", "listen addr and port");
DEFINE_int32(thread_num, 5, "CivetWeb worker thread number");

int main(int argc, char** argv)
{
  Counter<int> c{std::string{"test_first_counter"}, std::string{"Test Counter"}, {{"location", "ShangHai"}, {"category", "test"}, {"name", "test"}}};
  c.Init();
  c++;
  //std::cout << c.Collect() << std::endl;
  c.AddValue(10);
  //std::cout << c.Collect() << std::endl;
  c.AddLabel({{"person", "Yellow"}, {"role", "consumer"}});
  //std::cout << c.Collect() << std::endl;

  Guage<int> g{std::string{"test_first_guage"}, std::string{"Test Guage"}, {{"location", "BeiJing"}, {"category", "test"}, {"name", "test"}}};
  g.Init();
  g++;
  //std::cout << g.Collect() << std::endl;
  g.AddValue(10);
  //std::cout << g.Collect() << std::endl;
  g.SubValue(10);
  //std::cout << g.Collect() << std::endl;

  Histogram<double> h{std::string{"test_first_historgram"}, std::string{"Test Histogram"}, {{"location", "ShenZhen"}, {"category", "test"}}, {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}};
  h.Observe(-0.1);
  h.Observe(0.1);
  h.Observe(0.2);
  h.Observe(0.4);
  h.Observe(1.5);
  h.Observe(2.8);
  h.Observe(3.4);
  h.Observe(4.1);
  h.Observe(5.8);
  h.Observe(6.6);
  h.Observe(10.5);
  //std::cout << h.Collect() << std::endl;

  std::vector<std::string> options{
    "listening_ports", FLAGS_listen_addr,
    "num_threads", std::to_string(FLAGS_thread_num),
  };
  Exporter* exporter = new Exporter(options);
  exporter->Init();
  exporter->AddMetric(&c);
  exporter->AddMetric(&g);
  exporter->AddMetric(&h);

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
