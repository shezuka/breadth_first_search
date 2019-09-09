#include "map.h"
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
  Map map;

  // map.visual_print(true, 15000);

  map.generate_random_map(80, 40);
  // map.set_random_start_point();
  // ::sleep(rand() % 5 + 1);
  map.set_default_start_point();
  map.set_random_target_point();
  map.print();

  auto path = map.search_path(map.start_point(), map.target_point());
  cout << "Path " << (path.empty() ? "not " : "") << "found" << endl;
  map.print();

  return 0;
}

