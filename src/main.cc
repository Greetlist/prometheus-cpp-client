#include <iostream>
#include <thread>
#include <chrono>
#include <gflags/gflags.h>
#include "metric/counter.h"
#include "metric/guage.h"
#include "metric/histogram.h"
#include "metric/summary.h"
#include "http/exporter.h"

using namespace std;

DEFINE_string(listen_addr, "0.0.0.0:10000", "listen addr and port");
DEFINE_int32(thread_num, 5, "CivetWeb worker thread number");
DEFINE_int32(test_object_num, 100, "Test Object Number");

int main(int argc, char** argv)
{
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> options{
    "listening_ports", FLAGS_listen_addr,
    "num_threads", std::to_string(FLAGS_thread_num),
  };
  Exporter* exporter = new Exporter(options);
  exporter->Init();

  std::thread test_thread([&](){
    std::vector<Counter<int>*> cv;
    std::vector<Guage<int>*> gv;
    std::vector<Histogram<double>*> hv;
    std::vector<Summary<double>*> sv;
    for (int i = 0; i < FLAGS_test_object_num; ++i) {
      Counter<int>* c = new Counter<int>(
        std::string{"test_first_counter"},
        std::string{"Test Counter"},
        {{"location", "ShangHai"}, {"category", "test"}, {"name", "test"}, {"sid", std::to_string(i)}}
      );
      c->Init();
      exporter->AddMetric(c);
      cv.push_back(c);
    }

    for (int i = 0; i < FLAGS_test_object_num; ++i) {
      Guage<int>* g = new Guage<int>(
        std::string{"test_first_guage"},
        std::string{"Test Guage"},
        {{"location", "BeiJing"}, {"category", "test"}, {"name", "test"}, {"sid", std::to_string(i)}}
      );
      g->Init();
      exporter->AddMetric(g);
      gv.push_back(g);
    }

    for (int i = 0; i < FLAGS_test_object_num; ++i) {
      Histogram<double>* h = new Histogram<double>(
        std::string{"test_first_historgram"},
        std::string{"Test Histogram"},
        {{"location", "ShenZhen"}, {"category", "test"}, {"sid", std::to_string(i)}},
        {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}
      );
      h->Init();
      exporter->AddMetric(h);
      hv.push_back(h);
    }

    for (int i = 0; i < FLAGS_test_object_num; ++i) {
      Summary<double>* s = new Summary<double>(
        std::string{"test_first_summary"},
        std::string{"Test Summary"},
        {{"location", "GuangZhou"}, {"category", "test"}, {"sid", std::to_string(i)}},
        {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0}
      );
      s->Init();
      exporter->AddMetric(s);
      sv.push_back(s);
    }

    for (int i = 0; i < FLAGS_test_object_num * 3; ++i) {
      int index = i % FLAGS_test_object_num;
      cv[index]->AddValue(1);
      gv[index]->SubValue(1);
      hv[index]->Observe(2.5);
      sv[index]->Observe(3.5);
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    std::cout << "End Of Modify Metrics" << std::endl;
  });

  while (1) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }

  return 0;
}
