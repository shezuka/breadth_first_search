#ifndef MAP_H
#define MAP_H

#include "point.h"
#include "symbol.h"

#include <stack>

using namespace std;

class Map {
  public:
    using byte = unsigned char;
    using ulong = unsigned long;

  private:
    bool _visual = false;
    ulong _visual_print_rate = 20000;

    ulong _width, _height;
    ::Point **_map = nullptr;
    ::Point *_start_point = nullptr;

    void generate_rectangle();
    void clear_map();
    void unvisit_all();
    void clear_weights();

    void visual_print() const;

  public:
    ~Map();

    Point *start_point() const;
    Point *target_point() const;

    void set_default_start_point();
    void set_random_start_point();
    void set_random_target_point();

    void generate_random_map(ulong width = 20, ulong height = 20);
    void visual_print(bool value, ulong rate = 20000);

    std::stack<Point*> search_path(Point *from, Point *to);
    void print() const;
};

#endif
