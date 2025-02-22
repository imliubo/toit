// Copyright (C) 2019 Toitware ApS. All rights reserved.

import expect show *

gee /int := 499

foo x/int:
  return x

foo [x]:
  return x.call

bar -> bool:
  return true

class A:
  bar /int := 499
  operator ~:
    return "not"
  operator / other:
    return "div"

main:
  expect_equals 499 gee

  expect_equals 499 (foo 499)
  expect_equals 499 (foo: 499)

  expect_equals true bar
  expect_equals "not" ~A
  expect_equals "div" A / A

  expect_equals 499 (A).bar
