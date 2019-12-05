#' Isotonic / Monotone Regression
#'
#' Compute the isotonic (monotonely increasing nonparametric) least squares regression which is piecewise constant.
#'
#' @param x abscissa values (\code{x}) if \code{y} is supplied else target values.
#' @param y numeric vector target values.
#' @param implementation a character string specifying if the fast C++ or an equivalent R implementation should be used.
#' @return numeric vector of fitted values (\code{yf}).
#' @details If abscissa values are supplied the "primary" tie method is performed \eqn{(y_i<y_j \imply yf_i<yf_j)}.
#' @examples
#' y <- c(1, 3, 2, 4)
#' isoreg_Best_1990(y)
#'
#' x <- c(1, 0, 3, 2)
#' y <- c(3, 1, 4, 2)
#' isoreg_Best_1990(x, y)
#' @export
isoreg_Best_1990 <- function(x, y=NULL, decreasing = FALSE, implementation = c("cpp","R")) {
  implementation <- match.arg(implementation)

  if(doReorder <- !is.null(y)) {
    ord<-order(x,y)
    y <- y[ord]
  } else y <- x

  if(decreasing) yf <- -yf

  yf <- if(implementation=="R") isoreg_Best_1990_R(y) else isoreg_Best_1990_cpp(y)

  if(decreasing) yf <- -yf

  if(doReorder) {
    yf[ord] <- yf
  }
  yf
}

isoreg_Best_1990_R <- function(y) {
  n <- length(y)
  rCS <- c(rev(cumsum(rev(y))), 0)
  #   rCS <- c(rev(cumsum(rev(y))/seq_len(n)), 0)  # alternative using avg
  J <- c(1L, n+1L) # should rather be a double linked list
  #   avg <- function(i, i2=n+1) (rCS[i]*(n-i+1)-rCS[i2]*(n-i2+1))/(i2-i) # alternative using avg
  avg <- function(i, i2=n+1) (rCS[i]-rCS[i2])/(i2-i)
  avgB <- function(B) avg(J[B], J[B+1])
  B0 = 1
  for (i in seq_along(y)) {
    # print(paste(paste(J, collapse=" "), " -- ",paste(avg(i), collapse=" "), " -- ",B0))
    if (avg(i) > avgB(B0)) {
      J <- c(J[-(B0+1)], i, J[B0+1])
      B_ <- B0
      B0 <- B0+1
      while (B_ > 1 && avgB(B_) < avgB(B_-1)) {
        J <- J[-B_]
        B_ <- B_-1
        B0 <- B0-1 # only because we use an array for J
      }
    }
  }
  yf <- rep(avgB(seq_len(length(J)-1)), J[-1]-J[-length(J)])
}



#' compute_Patra_bound
#'
#' Compute Patra bound
#' https://arxiv.org/abs/1204.5488v5
#'
#' @param X0 Matrix of samples (rows) from 0  distribtion
#' @param Xm Matix of samples (rows) from mixture distribtion
#' @return numeric vector of fitted values (\code{yf}).
#' @examples
#' X0 <- matrix(runif(100), ncol=2)
#' Xm <- matrix(c(runif(80), runif(20)+2), ncol=2, byrow = TRUE)
#' compute_Patra_bound(X0, Xm)
#' @export
compute_Patra_bound <- function (X0, Xm){
  n_X0 <-  nrow(X0)
  n_Xm <-  nrow(Xm)

  F_X_hat <- rank_in_cpp(Xm,Xm)/n_Xm
  ord<-order(F_X_hat)
  F_X_hat<-F_X_hat[ord]
  F_H0_hat <- (rank_in_cpp(X0,Xm)/n_X0)[ord]

  testAlpha <- function(alpha){
    sapply(alpha,function(alpha){
      if (alpha==0) return(NaN)
      c_n <-  0.6792 #0.1*log(log(n_Xm)) #0.6792
      F_hat <- (F_X_hat-(1-alpha)*F_H0_hat)/alpha
      F_check <- pmin(pmax(stats::isoreg(F_X_hat,F_hat)$yf,0),1)
      alpha*sqrt(mean((F_hat-F_check)^2)) / (c_n/sqrt(n_Xm))
    })
  }
  pmax(stats::optimize(function(x)(testAlpha(x)-1)^2,c(0,1))$minimum-0.0001,0) # "Infimum"
}
