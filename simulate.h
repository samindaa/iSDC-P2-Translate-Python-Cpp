#ifndef SIMULATE_H
#define SIMULATE_H

#include <vector>
#include <random>

class Simulation {

 private:
  std::random_device rd;
  std::mt19937 gen;

  std::vector<char> get_colors();

  std::vector<int> random_move();

  void sense();

  void move(int dy, int dx);

  char get_observed_color();

 public:
  std::vector<std::vector<char> > grid;
  std::vector<std::vector<float> > beliefs;

  float blur, p_hit, p_miss, incorrect_sense_prob;

  int height, width, num_colors;

  std::vector<int> true_pose;
  std::vector<int> prev_pose;

  std::vector<char> colors;
  Simulation(const std::vector<std::vector<char> > &,
             float,
             float,
             std::vector<int>);

  void run(int num_steps=1);

  void show_beliefs();

};

#endif /* SIMULATE_H */