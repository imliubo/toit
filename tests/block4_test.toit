// Copyright (C) 2019 Toitware ApS. All rights reserved.

import expect show *

main:
  b := (: debug it)
  run_loop := true
  b2 := while run_loop:
    run_loop = false
    b
  expect_null b2
  
  b2 = for i := 0; i < 1; i++:
    b
  expect_null b2
