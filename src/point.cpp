#include "point.h"

#include <iostream>

using namespace std;

Point::Point(::byte symbol): symbol(symbol) {
}

Point::~Point() {
  if (top) top->bottom = nullptr;
  if (bottom) bottom->top = nullptr;
  if (left) left->right = nullptr;
  if (right) right->left = nullptr;
}

void Point::visit() {
  visited = true;
}

void Point::unvisit() {
  visited = false;
}

bool Point::movable() const {
  return symbol != WALL;
}

void Point::print() const {
  cout << symbol;
}

bool Point::operator>(const Point &other) const {
  return weight > other.weight;
}

bool Point::operator<(const Point &other) const {
  return weight < other.weight;
}

bool Point::operator==(const Point &other) const {
  return weight == other.weight;
}

std::vector<Point*> Point::movable_around(bool ignore_visited) const {
  std::vector<Point*> result;
  for (Point *p: connected_points) {
    if (p->movable()) {
      if (ignore_visited && p->visited) continue;
      result.emplace_back(p);
    }
  }
  return result;
}

Point *Point::min_weight_point() const {
  Point *result = nullptr;
  for (Point *p: connected_points) {
    if (!p->visited) continue;

    if (result == nullptr) {
      result = p;
      continue;
    }

    result = (*p < *result) ? p : result;
  }
  return result;
}

Point *Point::max_weight_point() const {
  Point *result = nullptr;
  for (Point *p: connected_points) {
    if (!p->visited) continue;

    if (result == nullptr) {
      result = p;
      continue;
    }

    result = (*p > *result) ? p : result;
  }
  return result;
}
