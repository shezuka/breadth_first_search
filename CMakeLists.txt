project(test)

include_directories(include)
add_executable(test main.cpp
  include/symbol.h
  include/point.h src/point.cpp
  include/map.h src/map.cpp)
