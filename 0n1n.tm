A, B, C, D, E, F
init A
accept A, D
A:
	0, _, >, B
	1, 1, >, F
:
B:
	0, 0, >, B
	1, 1, >, B
	_, _, <, C
:
C:
	1, _, <, D
:
D:
	0, 0, <, E
	1, 1, <, E
:
E:
	0, 0, <, E
	1, 1, <, E
	_, _, >, A
:


