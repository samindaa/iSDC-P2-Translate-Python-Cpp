/**
	simulate.cpp

	Purpose: implements a Simulation class which
	simulates a robot living in a 2D world. Relies 
	on localization code from localizer.py 

	This file is incomplete! Your job is to make 
	this code work. 
*/

#include <iostream>
#include <algorithm>
#include "simulate.h"
#include "localizer.h"
#include "debugging_helpers.h"

using namespace std;

/**
Constructor for the Simulation class.
*/
Simulation::Simulation(const vector<vector<char> > &map,
                       float blurring,
                       float hit_prob,
                       std::vector<int> start_pos
) {
  gen = std::mt19937(rd());
  grid = map;
  blur = blurring;
  p_hit = hit_prob;
  p_miss = 1.0;
  beliefs = initialize_beliefs(map);
  incorrect_sense_prob = p_miss / (p_hit + p_miss);
  true_pose = start_pos;
  prev_pose = true_pose;
  get_colors();
}

/**
Grabs colors from the grid map.
*/
vector<char> Simulation::get_colors() {
  vector<char> all_colors;
  char color;
  int i, j;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      color = grid[i][j];
      if (std::find(all_colors.begin(), all_colors.end(), color)
          != all_colors.end()) {
        /* v contains x */
      } else {
        all_colors.push_back(color);
        cout << "adding color " << color << endl;
        /* v does not contain x */
      }
    }
  }
  colors = all_colors;
  num_colors = colors.size();
  return colors;
}

std::vector<int> Simulation::random_move() {
  std::uniform_int_distribution<> dis(-1, 1); // -1, 0, 1
  return {dis(gen), dis(gen)};
}

void Simulation::run(int num_steps) {
  for (int i = 0; i < num_steps; ++i) {
    sense();
    auto dxy = random_move();
    move(dxy[1], dxy[0]);
    show_beliefs();
  }
}

void Simulation::show_beliefs() {
  const auto m = static_cast<int>(beliefs.size());
  const auto n = static_cast<int>(beliefs[0].size());
  int i_max = 0, j_max = 0;
  float b_max = -1;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (beliefs[i][j] > b_max) {
        b_max = beliefs[i][j];
        i_max = i;
        j_max = j;
      }
    }
  }

  bool is_localized = (true_pose[0] == i_max && true_pose[1] == j_max);
  std::cout << std::endl;
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      if (is_localized && i == i_max && j == j_max) {
        std::cout << "t{" << beliefs[i][j] << "}b ";
        continue;
      }
      if (is_localized) {
        std::cout << ". ";
      } else {
        if (true_pose[0] == i && true_pose[1] == j) {
          std::cout << "t";
        }
        std::cout << "{" << beliefs[i][j] << "}";
        if (i_max == i && j_max == j) {
          std::cout << "b ";
        } else {
          std::cout << " ";
        }
      }
    }
    std::cout << std::endl;
  }
}

char Simulation::get_observed_color() {
  const int y = true_pose[0], x = true_pose[1];
  const char true_color = grid[y][x];
  char color;
  std::uniform_real_distribution<> dis(0.0, 1.0);
  if (dis(gen) < incorrect_sense_prob) {
    std::vector<char> possible_colors = colors;
    possible_colors.erase(std::remove(possible_colors.begin(),
                                      possible_colors.end(),
                                      true_color),
                          possible_colors.end());
    std::uniform_int_distribution<> dis2(0, possible_colors.size() - 1);
    color = possible_colors[dis2(gen)];
  } else {
    color = true_color;
  }
  return color;
}

void Simulation::sense() {
  const char color = get_observed_color();
  beliefs = ::sense(color, grid, beliefs, p_hit, p_miss);
}

void Simulation::move(int dy, int dx) {
  const auto m = static_cast<int>(grid.size());
  assert(m > 0);
  const auto n = static_cast<int>(grid[0].size());
  assert(n > 0);
  const auto new_y = ((true_pose[0] + dy) % m + m) % m;
  const auto new_x = ((true_pose[1] + dx) % n + n) % n;
  prev_pose = true_pose;
  true_pose = {new_y, new_x};
  beliefs = ::move(dy, dx, beliefs, blur);
}

/**
You can test your code by running this function. 

Do that by first compiling this file and then 
running the output.
*/
//int main() {
//
//  vector<vector<char> > map;
//  vector<char> mapRow;
//  int i, j, randInt;
//  char color;
//  std::vector<int> pose(2);
//
//  for (i = 0; i < 4; i++) {
//    mapRow.clear();
//    for (j = 0; j < 4; j++) {
//      randInt = rand() % 2;
//      if (randInt == 0) {
//        color = 'r';
//      } else {
//        color = 'g';
//      }
//      mapRow.push_back(color);
//    }
//    map.push_back(mapRow);
//  }
//  cout << "map is\n";
//  Simulation simulation(map, 0.1, 0.9, pose);
//  cout << "initialization success!\n";
//  show_grid(map);
//
//  cout << "x, y = (" << simulation.true_pose[0] << ", "
//       << simulation.true_pose[1] << ")" << endl;
//  return 0;
//}
