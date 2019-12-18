library(isoregr)
context("isoreg")

test_that("isoreg_Best_1990 identical to base, y only", {
  y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)
  expect_equal(
    isoreg_Best_1990(y),
    stats::isoreg(y)$yf)
})

test_that("isoreg_Best_1990 identical to base, x and y", {
  x <- c(2L, 1L, 11L, 5L, 2L, 8L, 3L, 7L, 2L, 9L)
  y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)
  expect_equal(
    isoreg_Best_1990(x, y),
    stats::isoreg(x,y)$yf)
})

test_that("isoreg_Best_1990 identical R and Cpp are identical", {
  x <- c(2L, 1L, 11L, 5L, 2L, 8L, 3L, 7L, 2L, 9L)
  y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)
  expect_equal(
    isoreg_Best_1990(x, y, implementation = "cpp"),
    isoreg_Best_1990(x, y, implementation = "R"))
})
