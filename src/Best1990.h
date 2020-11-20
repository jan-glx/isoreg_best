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
    const std::size_t n = y.size();                                                                      // n <- length(y)
    std::vector<typename T::value_type> rCS(n+1);                                                        //
    std::partial_sum(y.rbegin(), y.rend(), ++rCS.rbegin());                                              // rCS <- c(rev(cumsum(rev(y))), 0)
    std::forward_list<std::size_t> J;                                                                    // J <- c(1L, n+1L)
    std::forward_list<std::size_t>::iterator B_, B_1;                                                    // avg <- function(i, i2=n+1) (rCS[i]-rCS[i2])/(i2-i)
    typename T::value_type avg_B0 = rCS[0]/n;                                                            // avgB <- function(B) avg(J[B], J[B+1])
    J.push_front(0);                                                                                     // B0 = 1
    for (std::size_t i = 1; i<n; i++) {                                                                  // for (i in seq_along(y)) {
        if (rCS[i]/(n-i) > avg_B0) {                                                                     //     if (avg(i) > avgB(B0)) {
            B_ = J.begin();                                                                              //         J <- c(J[-(B0+1)], i, J[B0+1])
            B_1 = J.begin()++;                                                                           //         B_ <- B0
            avg_B0 = rCS[i]/(n-i);                                                                       //         B0 <- B0+1
            while (B_1 != J.end() && (rCS[*B_]-rCS[i])/(i-*B_1) <= (rCS[*B_1]-rCS[*B_])/(*B_-*B_1)) {    //         while (B_ > 1 && avgB(B_) < avgB(B_-1)) {
                B_++; B_1++;                                                                             //             J <- J[-B_]
                J.pop_front();                                                                           //             B_ <- B_-1
            }                                                                                            //             B0 <- B0-1 # only because we use an array for J
            J.push_front(i);                                                                             //         }
        }                                                                                                //     }
    }                                                                                                    // }
    J.push_front(n);                                                                                     //
                                                                                                         //
    T yf(y.size());                                                                                      // yf <- rep(avgB(seq_len(length(J)-1)), J[-1]-J[-length(J)])
    for(std::size_t i, prev_i = n; J.begin() != J.end(); J.pop_front()){                                 //
        i = *J.begin();                                                                                  //
        std::fill(yf.begin()+i, yf.begin()+prev_i, (rCS[i]-rCS[prev_i])/(prev_i-i));                     //
        prev_i = i;                                                                                      //
    }                                                                                                    //
    return yf;                                                                                           //
}
#endif //R_BEST1990_H

/* M.J. Best, N. Chakravarti 1990 / Active set algorithms for isotonic regression :
 * A new O(n) primal active set algorithm In this section we describe Agorithm A, a new O(n) primal active set algorithm.
 * The algorithm has available, at each iteration, the current active set J. The block containing n, at any iteration,
 * will be designated B_0. B_ will denote P(B_0) and I(B_0) will denote the least integer in B_0.
 *
 * Algorithm A. :
 *  Set B_0 = {1, 2,..., n}, J = {B_0}.
 *   Compute Av(B_0) and Av(U_i) for each i \in B_0.
 *  Go to Step 1.
 *  Step 1:
 *   Compute j = min{i|Av(U_i) > Av(B_0), or i = n}.
 *   If j = n then stop with the conclusion that the optimal solution is given by x_i = Av(B) for each i \in B, for each block B of J.
 *   Else go to Step 2.
 *  Step 2:
 *   Set J = J\B_0 \union {L_j, U_j}.
 *   Set B_0 = U_j and B_=L_j.
 *   Repeat Step 2.1 while Av(B_) < Av(P(B_)) and P(B_) \neq {}.
 *  Step 2.1:
 *   Compute Av(B_).
 *   If Av(B_) \leq Av(P(B_)), set J = J\{B_, P(B_)} \union {B_ \union P(B_)} and B_ = B_ \union P(B_). Continue.
 *   Go to Step 1.
 */