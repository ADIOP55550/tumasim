
// sgn(x) = 0 dla x = 0 ; 1 dla x > 0

// b) liczba x podana w postaci unarnej

// Unary  dec
// 1      0
// 11     1
// 111    2
// 1111   3
// ...    ...

// 1 -> 	1 (unarny)
// 11 -> 	11
// 111 -> 	11
// 1111 -> 	11

// 1 -> 	0 (unarny)
// 11 -> 	1
// 111 -> 	1
// 1111 -> 	1


name: 
init: A
accept: A, B, C, D


// [current_state],[read_symbol]
// [new_state],[write_symbol],[>|<|-]

A, 1
B, 1, >

B, _
D, _, <

D, 1
D, 0, -

B, 1
B, _, >



