#include "map.h"

#include <iostream>
#include <ctime>
#include <unistd.h>
#include <queue>

using namespace std;

void Map::generate_rectangle() {
    for (int y = 0; y < _height; y++) {
        _map[0][y].symbol = WALL; 
        _map[_width - 1][y].symbol = WALL;
        }

        for (int x = 0; x < _width; x++) {
        _map[x][0].symbol = WALL;
        _map[x][_height - 1].symbol = WALL;
    }
}

void Map::clear_map() {
    if (_map) {
        for (unsigned long x = 0; x < _width; x++) {
            delete[] _map[x];
        }
        delete []_map;
        _map = nullptr;
    }
}

void Map::unvisit_all() {
    for (unsigned long y = 1; y < _height - 1; y++) {
        for (unsigned long x = 1; x < _width - 1; x++) {
            _map[x][y].unvisit();
        }
    }
}

void Map::clear_weights() {
    for (unsigned y = 0; y < _height; y++) {
        for (unsigned x = 0; x < _width; x++) {
            _map[x][y].weight = 0;
        }
    }
}

void Map::generate_random_map(Map::ulong width, Map::ulong height) {
    _width = width;
    _height = height;
    _map = new Point*[width];
    for (int x = 0; x < width; x++) {
        _map[x] = new Point[height];
    }

    for (unsigned long y = 0; y < height; y++) {
        for (unsigned long x = 0; x < width; x++) {
            Point *current = &_map[x][y];
            if (x > 0) {
                current->left = &_map[x - 1][y];
                current->connected_points.emplace_back(current->left);
            }
            if (x < width - 1) {
                current->right = &_map[x + 1][y];
                current->connected_points.emplace_back(current->right);
            }
            if (y > 0) {
                current->top = &_map[x][y - 1];
                current->connected_points.emplace_back(current->top);
            }
            if (y < height - 1) {
                current->bottom = &_map[x][y + 1];
                current->connected_points.emplace_back(current->bottom);
            }
        }
    }

    generate_rectangle();
    unvisit_all();
    ::srand(time(nullptr) % 10000);

    Point *generate_start_point = &_map[1 + (rand() % (_width - 2))][1 + (rand() % (_height - 2))];
    queue<Point*> points;
    points.push(generate_start_point);
    while(!points.empty()) {
        Point *point = points.front();
        points.pop();
        if (point->visited) continue;
        point->visit();

        auto movable = point->movable_around();
        if (movable.empty()) continue;

        for (Point *p: movable) {
            p->symbol = (rand() % 2) == 1 ? WALL : SPACE;
            if (p->symbol == SPACE) points.emplace(p);
        }

        bool all_blocked = true;
        for (Point *p: movable) {
            if (p->symbol == SPACE) {
                all_blocked = false;
                break;
            }
        }

        if (all_blocked) {
            Point *p = movable[rand() % movable.size()];
            p->symbol = SPACE;
            points.emplace(p);
        }
    }
}

Point *Map::target_point() const {
    for (ulong x = 1; x < _width - 1; x++) {
        for (ulong y = 1; y < _height - 1; y++) {
            Point *p = &_map[x][y];
            if (p->symbol == TARGET) {
                return p;
            }
        }
    }
    return nullptr;
}

void Map::visual_print() const {
    if (_visual) {
        cout << endl << endl << endl << endl;
        print();
        cout << endl << endl << endl << endl;
        usleep(_visual_print_rate);
    }
}

Map::~Map() {
    clear_map();
}

void Map::visual_print(bool value, Map::ulong rate) {
    _visual = value;
    _visual_print_rate = rate;
}

std::stack<Point*> Map::search_path(Point *from, Point *to) {
    unvisit_all();

    std::queue<Point*> points;
    points.emplace(from);

    bool found = false;
    while (!points.empty()) {
        Point *p = points.front();
        points.pop();
        if (p->visited) continue;
        p->visit();

        if (p->symbol != TARGET)
          p->symbol = SCAN;
        auto movable = p->movable_around();
        for (Point *mp: movable) {
          points.emplace(mp);
          if (!mp->weight) {
              mp->weight = p->weight + 1;
          }
          if (mp == to) {
              found = true;
              break;
          }
        }
        if (found) break;
        visual_print();
    }

    std::stack<Point*> path;
    if (!found) return path;

    for (ulong x = 1; x < _width - 1; x++) {
        for (ulong y = 1; y < _height - 1; y++) {
            Point *p = &_map[x][y];
            if (p->symbol != SCAN) continue;
            p->symbol = SPACE;
        }
    }

    Point *current = to;
    path.emplace(current);
    while(current != from) {
        current = current->min_weight_point();
        current->symbol = PATH;
        path.emplace(current);
        visual_print();
    }
    return path;
}

void Map::print() const {
    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            _map[x][y].print();
        }
        cout << endl;
    }
}

Point *Map::start_point() const {
    return _start_point;
}

void Map::set_default_start_point() {
    _start_point = &_map[1][1];
}

void Map::set_random_start_point() {
    ::srand(time(nullptr));
    _start_point = nullptr;
    do {
        _start_point = &_map[1 + (rand() % (_width - 2))][1 + (rand() % (_height - 2))];
    } while (!_start_point->movable() && !_start_point->movable_around(false).empty());
}

void Map::set_random_target_point() {
    ::srand(time(nullptr));
    Point *point = nullptr;
    do {
        point = &_map[1 + (rand() % (_width - 2))][1 + (rand() % (_height - 2))];
    } while (!point->movable() && !point->movable_around(false).empty());
    point->symbol = TARGET;
}
