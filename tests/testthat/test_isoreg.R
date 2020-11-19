library(isoregr)

context("isoreg with only y")

y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)
test_that("isoreg_Best_1990 identical to base, y only", {
  expect_equal(
    isoreg_Best_1990(y),
    stats::isoreg(y)$yf)
})

test_that("isoreg_Best_1990 R and Cpp are identical", {
  expect_equal(
    isoreg_Best_1990(y, implementation = "cpp"),
    isoreg_Best_1990(y, implementation = "R"))
})

context("isoreg with x and y, no ties")
x <- c(2L, 1L, 11L, 5L, 2.3, 8L, 3L, 7L, 2.4, 9L)
y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)

test_that("isoreg_Best_1990 identical to base, x and y, no ties", {
  expect_equal(
    isoreg_Best_1990(x, y),
    {r <- stats::isoreg(x, y); r$yf[order(r$ord)]})
})

test_that("isoreg_Best_1990 R and Cpp are identical", {
  expect_equal(
    isoreg_Best_1990(x, y, implementation = "cpp"),
    isoreg_Best_1990(x, y, implementation = "R"))
})

context("isoreg with x and y, with ties")
x <- c(2L, 1L, 11L, 5L, 2L, 8L, 3L, 7L, 2L, 9L)
y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)

test_that("isoreg_Best_1990 identical to base, x and y, with ties", {
  expect_equal(
    isoreg_Best_1990(x, y),
    {r <- stats::isoreg(x, y); r$yf[order(r$ord)]})
})

test_that("isoreg_Best_1990 R and Cpp are identical", {
  expect_equal(
    isoreg_Best_1990(x, y, implementation = "cpp"),
    isoreg_Best_1990(x, y, implementation = "R"))
})
