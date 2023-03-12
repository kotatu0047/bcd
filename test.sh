#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  actual=`./bcd "$input"` 

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    make clean
    exit 1
  fi
}

make

# assert 0
assert 42 "40+2"
assert 35 "5+20+(4+6)"
assert 949088 "1 + 948969 + 5 + (4 + 99 + 10 + 0)"
assert -17 "12 - 34 + 5 "
assert 94 "99 - 5 "
assert -94 "5 - 99"
# assert 15 "5*(9-6)"
# assert 4 "(3+5)/2"

make clean

echo OK