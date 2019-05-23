/**
	helpers.cpp

	Purpose: helper functions which are useful when
	implementing a 2-dimensional histogram filter.

	This file is incomplete! Your job is to make the
	normalize and blur functions work. Feel free to 
	look at helper.py for working implementations 
	which are written in python.
*/

#include <vector>
#include <iostream>
#include <numeric>
#include <cmath>
#include <string>
#include <fstream>
#include "helpers.h"
// #include "debugging_helpers.cpp"

using namespace std;

/**
	TODO - implement this function

    Normalizes a grid of numbers. 

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

    @return - a new normalized two dimensional grid where the sum of 
    	   all probabilities is equal to one.
*/
vector<vector<float> > normalize(const vector<vector<float> > &grid) {

  vector<vector<float> > newGrid = grid;
  float sum = 0;
  for (const auto &row : grid) {
    sum += std::accumulate(row.begin(), row.end(), 0.0);
  }
  for (auto &row : newGrid) {
    for (auto &cell : row) {
      cell /= sum;
    }
  }
  return newGrid;
}

/**
	TODO - implement this function.

    Blurs (and normalizes) a grid of probabilities by spreading 
    probability from each cell over a 3x3 "window" of cells. This 
    function assumes a cyclic world where probability "spills 
    over" from the right edge to the left and bottom to top. 

    EXAMPLE - After blurring (with blurring=0.12) a localized 
    distribution like this:

    0.00  0.00  0.00 
    0.00  1.00  0.00
    0.00  0.00  0.00 

    would look like this:
	
	0.01  0.02	0.01
	0.02  0.88	0.02
	0.01  0.02  0.01

    @param grid - a two dimensional grid (vector of vectors of floats)
		   where each entry represents the unnormalized probability 
		   associated with that grid cell.

	@param blurring - a floating point number between 0.0 and 1.0 
		   which represents how much probability from one cell 
		   "spills over" to it's neighbors. If it's 0.0, then no
		   blurring occurs. 

    @return - a new normalized two dimensional grid where probability 
    	   has been blurred.
*/
vector<vector<float> > blur(const vector<vector<float> > &grid,
                            float blurring) {

  vector<vector<float> > newGrid;
  // Do not mix sign and unsigned values.
  // Assume the grid is reasonably sized.
  const auto m = static_cast<int>(grid.size());
  assert(m > 0);
  const auto n = static_cast<int>(grid[0].size());
  assert(n > 0);

  // your code here
  const float center_prob = 1.0 - blurring;
  const float corner_prob = blurring / 12.0;
  const float adjacent_prob = blurring / 6.0;
  std::vector<std::vector<float> > window =
      {{corner_prob, adjacent_prob, corner_prob},
       {adjacent_prob, center_prob, adjacent_prob},
       {corner_prob, adjacent_prob, corner_prob}};
  newGrid.resize(m, std::vector<float>(n, 0));
  for (int i = 0; i < m; ++i) {
    for (int j = 0; j < n; ++j) {
      // Convolution
      for (int di : {-1, 0, 1}) {
        for (int dj : {-1, 0, 1}) {
          const auto mult = window[di + 1][dj + 1];
          const auto new_i = ((i + di) % m + m) % m;
          const auto new_j = ((j + dj) % n + n) % n;
          newGrid[new_i][new_j] += mult * grid[i][j];
        }
      }
    }
  }

  return normalize(newGrid);
}

/** -----------------------------------------------
#
#
#	You do not need to modify any code below here.
#
#
# ------------------------------------------------- */


/**
    Determines when two grids of floating point numbers 
    are "close enough" that they should be considered 
    equal. Useful for battling "floating point errors".

    @param g1 - a grid of floats
    
    @param g2 - a grid of floats

    @return - A boolean (True or False) indicating whether
    these grids are (True) or are not (False) equal.
*/
bool close_enough(const vector<vector<float> > &g1,
                  const vector<vector<float> > &g2) {
  int i, j;
  float v1, v2;
  if (g1.size() != g2.size()) {
    return false;
  }

  if (g1[0].size() != g2[0].size()) {
    return false;
  }
  for (i = 0; i < g1.size(); i++) {
    for (j = 0; j < g1[0].size(); j++) {
      v1 = g1[i][j];
      v2 = g2[i][j];
      if (abs(v2 - v1) > 0.0001) {
        return false;
      }
    }
  }
  return true;
}

bool close_enough(float v1, float v2) {
  return abs(v2 - v1) <= 0.0001;
}

/**
    Helper function for reading in map data

    @param s - a string representing one line of map data.

    @return - A row of chars, each of which represents the
    color of a cell in a grid world.
*/
vector<char> read_line(string s) {
  vector<char> row;
  if (s.empty()) {
    return row;
  }

  size_t pos = 0;
  string token;
  string delimiter = " ";
  char cell;

  while ((pos = s.find(delimiter)) != std::string::npos) {
    token = s.substr(0, pos);
    s.erase(0, pos + delimiter.length());

    cell = token.at(0);
    row.push_back(cell);
  }

  return row;
}

/**
    Helper function for reading in map data

    @param file_name - The filename where the map is stored.

    @return - A grid of chars representing a map.
*/
vector<vector<char> > read_map(const string &file_name) {
  ifstream infile(file_name);
  vector<vector<char> > map;
  if (infile.is_open()) {

    char color;
    vector<char> row;

    string line;

    while (std::getline(infile, line)) {
      row = read_line(line);
      if (!row.empty()) {
        map.push_back(row);
      }
    }
  }
  return map;
}

/**
    Creates a grid of zeros

    For example:

    zeros(2, 3) would return

    0.0  0.0  0.0
    0.0  0.0  0.0

    @param height - the height of the desired grid

    @param width - the width of the desired grid.

    @return a grid of zeros (floats)
*/
vector<vector<float> > zeros(int height, int width) {
  int i, j;
  vector<vector<float> > newGrid;
  vector<float> newRow;

  for (i = 0; i < height; i++) {
    newRow.clear();
    for (j = 0; j < width; j++) {
      newRow.push_back(0.0);
    }
    newGrid.push_back(newRow);
  }
  return newGrid;
}

// int main() {
// 	vector < vector < char > > map = read_map("maps/m1.txt");
// 	show_grid(map);
// 	return 0;
// }
