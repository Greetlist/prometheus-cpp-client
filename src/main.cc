#include <iostream>
#include "metric/counter.h"
#include "metric/guage.h"

using namespace std;

int main(int argc, char** argv)
{
  Counter<double> c{std::string{"test_first_counter"}, std::string{"Test Counter"}, {{"location", "ShangHai"}, {"category", "test"}, {"name", "test"}}};
  c.Init();
  c++;
  std::cout << c.Collect() << std::endl;
  c.AddValue(10.0);
  std::cout << c.Collect() << std::endl;
  c.AddLabel({{"person", "Yellow"}, {"role", "consumer"}});
  std::cout << c.Collect() << std::endl;

  Guage<double> g{std::string{"test_first_guage"}, std::string{"Test Guage"}, {{"location", "BeiJing"}, {"category", "test"}, {"name", "test"}}};
  g.Init();
  g++;
  std::cout << g.Collect() << std::endl;
  g.AddValue(10.0);
  std::cout << g.Collect() << std::endl;
  g.SubValue(10.0);
  std::cout << g.Collect() << std::endl;

  



  return 0;
}
