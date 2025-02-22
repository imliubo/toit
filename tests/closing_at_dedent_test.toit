// Copyright (C) 2019 Toitware ApS. All rights reserved.

import expect show *

main:
  executed := false
  fun := (:
    executed = true
  )  // Closing parenthesis at dedent-level.
  fun.call
  expect executed

  map := {
    "foo": "bar"
  }  // Closing brace at dedent-level.

  list := [
    1
  ]  // Closing bracket at dedent-level.
