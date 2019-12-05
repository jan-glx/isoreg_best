#include <Rcpp.h>
#include "rih.h"

using namespace Rcpp;


//' Compute rank of Q in X
//'
//' @importFrom Rcpp evalCpp
//' @param X Matrix of target values
//' @param Q Matrix of query values
//' @param nsw integer to tune hybrid algorithm
//' @return vector of rank of each row of Q in X
//' @export
// [[Rcpp::export]]
IntegerVector
rank_in_cpp(NumericMatrix const X, NumericMatrix const Q, long int const nsw = 400L) {
  int nrow = X.nrow(), ncol = X.ncol();
  auto sX = std::vector<std::vector<double>>(nrow);
  for (int i = 0; i < nrow; i++) {
    sX[i]  = std::vector<double>(ncol);
    for (int j = 0; j < ncol; j++) {
      sX[i][j]= X(i, j);
    }
  }

  int nrowQ = Q.nrow(), ncolQ = Q.ncol();
  auto sQ = std::vector<std::vector<double>>(nrowQ);
  for (int i = 0; i < nrowQ; i++) {
    sQ[i]  = std::vector<double>(ncolQ);
    for (int j = 0; j < ncolQ; j++) {
      sQ[i][j]= Q(i, j);
    }
  }
    IntegerVector ret = wrap(RIH::rank_in_hybrid(sX, sQ, nsw)); //
    return ret;
  }

/*** R
rank_in_cpp(matrix(c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L),ncol=2)+0.0,matrix(c(3,6,6,9),ncol=2)+0.0)
*/


