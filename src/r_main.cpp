#include <Rcpp.h>
#include "rih.hpp"

// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// Sys.setenv("PKG_CXXFLAGS"="-std=c++1y") #CXX1XSTD
// Sys.setenv("CXX1XSTD"="-std=c++1y") #CXX1XSTD
// f <- devtools::build()
// install.packages(f, repos = NULL, type = "source")
// devtools::check_built(f)
// [[Rcpp::plugins(cpp11)]]


using namespace Rcpp;


//' Needs text
//'
//' sfg
//'
//' @importFrom Rcpp evalCpp
//' @param X target values.
//' @param Q target values.
//' @return fitted values.
//' @export
// [[Rcpp::export]]
IntegerVector
rank_in_cpp(NumericMatrix const X, NumericMatrix const Q, long int const nsw = 400) {
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


