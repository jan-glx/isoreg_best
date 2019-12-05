#include <vector>
#include <iostream>
#include "../src/rih.h"


int main () {
  auto X = std::vector<std::vector<double>>{{ 40, 52, 52 }, { 44, 41, 37 }, { 27, 27, 9 }, { 46, 27, 35 }, { 47, 23, 5 },
                                         { 13, 49, 6 }, { 1, 10, 45 }, { 8, 22, 9 }, { 47, 2, 4 }, { 31, 16, 17 },
                                         { 39, 50, 34 }, { 13, 30, 36 }, { 8, 19, 41 }, { 37, 1, 39 }, { 54, 23, 59 },
                                         { 14, 23, 28 }, { 8, 1, 2 }, { 59, 21, 40 }, { 45, 56, 41 }, { 25, 9, 38 }};
  auto isQ = std::vector<bool>{ false, false, true, false, true, false, false, false, true, false, false, true, true, false, false, false, true, true, true, true };

  auto Q = std::vector<std::vector<double>>{{ 40, 52, 52 }, { 44, 41, 37 }, { 27, 27, 9 }, { 46, 27, 35 }, { 47, 23, 5 },
                                         { 13, 49, 6 },  { 59, 21, 40 }, { 45, 56, 41 }, { 25, 9, 38 }};
  auto res =  RIH::rank_in_hybrid_(X, isQ, X.front().size(), 1);
  for (auto &x: res) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
  std::cout << std::endl;

  res =  RIH::rank_in_hybrid(X, Q, 1);
  for (auto &x: res) {
    std::cout << x << " ";
  }
  std::cout << std::endl;

  X = std::vector<std::vector<double>>{{ 3, 1}, { 3, 6 }, { 1, 6 }};

  Q = std::vector<std::vector<double>>{{ 2, 1 }, { 1, 5 }, { 4, 1 }};
  res =  RIH::rank_in_hybrid(X, Q, 1);
  for (auto &x: res) {
    std::cout << x << " ";
  }
  std::cout << std::endl;
}
