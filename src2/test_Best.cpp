//
// Created by jan on 2016-11-28.
//
#include "../src/Best1990.h"
#include <iostream>
#include <vector>
#include <iterator>

int main() {
    std::vector<double> in = {1, 8, 3, 1, 3, 8, 8, 9, 7, 10};
    std::vector<double> out = isoreg_Best1990(in);
    std::cout << "in" << std::endl;
    std::copy(in.begin(), in.end(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << "out" << std::endl;
    std::copy(out.begin(), out.end(), std::ostream_iterator<double>(std::cout, " "));
    std::cout << "end" << std::endl;
    return 0;
}
