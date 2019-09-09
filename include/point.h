#ifndef POINT_H
#define POINT_H

#include "symbol.h"

#include <vector>

using byte = unsigned char;

struct Point {
  bool visited = false;
  unsigned weight = 0;
  ::byte symbol;

  Point *top = nullptr;
  Point *bottom = nullptr;
  Point *left = nullptr;
  Point *right = nullptr;

  std::vector<Point*> connected_points;

  Point(::byte symbol = SPACE);
  ~Point();

  void visit();
  void unvisit();

  bool movable() const;

  void print() const;

  bool operator>(const Point&) const;
  bool operator<(const Point&) const;
  bool operator==(const Point&) const;

  std::vector<Point*> movable_around(bool ignore_visited = true) const;
  Point *min_weight_point() const;
  Point *max_weight_point() const;
};

#endif
