library(Best1990)
context("isoreg")

test_that("str_length is number of characters", {
  y <- c(1L, 1L, 10L, 8L, 3L, 9L, 3L, 8L, 8L, 7L)
  expect_equal(
    isoreg_Best_1990(y),
    isoreg(y)$yf)
})
