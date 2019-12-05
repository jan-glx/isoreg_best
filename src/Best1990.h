//
// Created by jan on 2016-11-28.
//

#ifndef R_BEST1990_H
#define R_BEST1990_H

#include <numeric>
#include <forward_list>
#include <vector>

template <class T>
T isoreg_Best1990(T y) {
    const std::size_t n = y.size();
    std::vector<typename T::value_type> rCS(n+1);
    std::partial_sum(y.rbegin(), y.rend(),++rCS.rbegin());
    std::forward_list<std::size_t> J;
    std::forward_list<std::size_t>::iterator B_, B_1;
    typename T::value_type avg_B0 = rCS[0]/n;
    J.push_front(0);
    for (std::size_t i = 1; i<n; i++) {
        if (rCS[i]/(n-i) > avg_B0) {
            B_ = J.begin();
            B_1 = J.begin()++;
            avg_B0 = rCS[i]/(n-i);
            while (B_1 != J.end() && (rCS[*B_]-rCS[i])/(i-*B_1) <= (rCS[*B_1]-rCS[*B_])/(*B_-*B_1)) {
                B_++; B_1++;
                J.pop_front();
            }
            J.push_front(i);
        }
    }
    J.push_front(n);

    T yf(y.size());
    for(std::size_t i, prev_i = n; J.begin() != J.end(); J.pop_front()){
        i = *J.begin();
        std::fill(yf.begin()+i, yf.begin()+prev_i, (rCS[i]-rCS[prev_i])/(prev_i-i));
        prev_i = i;
    }
    return yf;
}
#endif //R_BEST1990_H
