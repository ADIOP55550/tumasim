
// 1*01*

// 0 jeśli x nie należy do L, 1 wpp

name: dsjaio
init: A
accept: T

A, 1
A, 1, >

A, 0
B, 0, >

A, _
R, _, <

B, 1
B, 1, >

B, 0
X, 0, >

B, _
C, _, <

C, 1
C, _, <

C, 0
C, _, <

C, _
T, 1, -

X, 0
X, 0, >

X, 1
X, 1, >

X, _
R, _, <

R, 0
R, _, <

R, 1
R, _, <

R, _
T, 0, -

