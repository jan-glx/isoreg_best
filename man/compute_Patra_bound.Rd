% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/isoreg.R
\name{compute_Patra_bound}
\alias{compute_Patra_bound}
\title{compute_Patra_bound}
\usage{
compute_Patra_bound(X0, Xm)
}
\arguments{
\item{X0}{Matrix of samples (rows) from 0  distribution}

\item{Xm}{Matrix of samples (rows) from mixture distribution}
}
\value{
lower bound for the mixing coefficient alpha
}
\description{
Compute Patra bound
 F_m ~ alpha F_1 + (1-alpha) F_0
https://arxiv.org/abs/1204.5488v5
}
\examples{
X0 <- matrix(runif(100), ncol=2)
Xm <- matrix(c(runif(80), runif(20)+2), ncol=2, byrow = TRUE)
compute_Patra_bound(X0, Xm)
}
