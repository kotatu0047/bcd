#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./bcd "$input"
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

# assert 0
assert 42 "40+2"
# assert 21 "5*20-(4+6)"
# assert 41 " 1 + 948969 * 5 + (4 * 99 + 10 + 0) "
# # assert 41 " 12 -+ 34 - 5 "
# # assert 41 " 12 ++ 34 - 5 "
# assert 47 "5+6*7"
# assert 15 "5*(9-6)"
# assert 4 "(3+5)/2"

echo OK