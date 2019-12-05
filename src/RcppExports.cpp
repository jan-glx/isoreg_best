// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// isoreg_Best_1990_cpp
NumericVector isoreg_Best_1990_cpp(NumericVector y);
RcppExport SEXP _Best1990_isoreg_Best_1990_cpp(SEXP ySEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericVector >::type y(ySEXP);
    rcpp_result_gen = Rcpp::wrap(isoreg_Best_1990_cpp(y));
    return rcpp_result_gen;
END_RCPP
}
// rank_in_cpp
IntegerVector rank_in_cpp(NumericMatrix const X, NumericMatrix const Q, long int const nsw);
RcppExport SEXP _Best1990_rank_in_cpp(SEXP XSEXP, SEXP QSEXP, SEXP nswSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< NumericMatrix const >::type X(XSEXP);
    Rcpp::traits::input_parameter< NumericMatrix const >::type Q(QSEXP);
    Rcpp::traits::input_parameter< long int const >::type nsw(nswSEXP);
    rcpp_result_gen = Rcpp::wrap(rank_in_cpp(X, Q, nsw));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_Best1990_isoreg_Best_1990_cpp", (DL_FUNC) &_Best1990_isoreg_Best_1990_cpp, 1},
    {"_Best1990_rank_in_cpp", (DL_FUNC) &_Best1990_rank_in_cpp, 3},
    {NULL, NULL, 0}
};

RcppExport void R_init_Best1990(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
