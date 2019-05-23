#ifndef DEBUGGING_HELPERS_H
#define DEBUGGING_HELPERS_H

#include <iostream>
#include <vector>

// Displays a grid of beliefs. Does not return.
//void show_grid(const std::vector<std::vector<float> >& grid);

// Displays a grid map of the world
//void show_grid(const std::vector<std::vector<char> >& map);

template<typename  T>
void show_grid(const std::vector<std::vector<T> >& map) {
  for (const auto& row: map) {
    for (const auto& value : row) {
      std::cout << value << ' ';
    }
    std::cout << std::endl;
  }
}

#endif /* DEBUGGING_HELPERS_H */