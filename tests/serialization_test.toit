// Copyright (C) 2018 Toitware ApS. All rights reserved.

import serialization show serialize deserialize
import expect show *

test_atom object:
  expect_equals
    object
    deserialize
      serialize object

test_array array:
  result := deserialize
    serialize array
  expect_equals array.size result.size
  for i := 0; i < array.size; i++:
    expect_equals array[i] result[i]

test_array_cycle:
/*
  TODO(anders): Re-enable when fixed
  array := Array_ 1
  array[0] = array
  result := deserialize
    serialize array
  expect_equals array.size result.size
  expect (identical result[0] result)
  */

test_map:
  map := Map
  map["1"] = "1"
  map["45"] = "45"
  result := deserialize (serialize map)
  expect_equals result["1"] "1"
  expect_equals result["45"] "45"
  expect (not result.contains 2)

main:
  test_atom 12
  test_atom true
  test_atom false
  test_atom null
  test_atom ""
  test_atom "Fiskerdreng"
  test_array (Array_ 0)
  // No cycles, no reuse array.
  a := Array_ 4
  a[0] = 12
  a[1] = true
  a[2] = false
  a[3] = null
  test_array a
  // With strings.
  a = Array_ 4
  a[0] = "Fiskerdreng"
  a[1] = ""
  a[2] = "Fiskerdreng"
  a[3] = ""
  test_array a
  a = Array_ 1
  a[0] = "Fisk"
  test_array a
  test_array_cycle
  c := 75
  test_map
