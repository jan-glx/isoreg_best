#include <Rcpp.h>
// Enable C++11 via this plugin (Rcpp 0.10.3 or later)
// Sys.setenv("PKG_CXXFLAGS"="-std=c++11")
// f <- devtools::build()
// install.packages(f, repos = NULL, type = "source")
// devtools::check_built(f)
// [[Rcpp::plugins(cpp11)]]

#include "Best1990.h"

using namespace Rcpp;


//' Isotonic regession after Best & Chakravarti (1990)
//'
//' C++ implementation of Algorithm A from Best & Chakravarti (1990).
//' Performs iso tonic regression with a runtime complexity of \eqn{\mathcal{O}(n)}
//'
//' Best, M.J. & Chakravarti, N. Mathematical Programming (1990) 47: 425.
//' \href{http://doi.org/10.1007/BF01580873}{doi:10.1007/BF01580873}
//'
//' @useDynLib Best1990
//' @importFrom Rcpp evalCpp
//' @param y target values.
//' @return fitted values.
//' @examples
//' y <- c(1, 3, 2, 4)
//' isoreg_Best_1990_cpp(y)
//' @export
// [[Rcpp::export]]
NumericVector isoreg_Best_1990_cpp(NumericVector y) {
  return wrap(isoreg_Best1990(as< std::vector<double> >(y)));
}

/*** R
isoreg_Best_1990(c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L))
*/
