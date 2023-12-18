#include <iostream>
#include "metric/counter.h"

using namespace std;

int main(int argc, char** argv)
{
  //Counter<int> c(std::string{"test_first_counter"}, std::string{"Test Help"}, {{"location", "ShangHai"}, {"category", "test"}, {"name", "test"}});
  Counter<double> c{std::string{"test_first_counter"}, std::string{"Test Help"}, {{"location", "ShangHai"}, {"category", "test"}, {"name", "test"}}};
  c.Init();
  c++;
  std::cout << c.Collect() << std::endl;
  return 0;
}
