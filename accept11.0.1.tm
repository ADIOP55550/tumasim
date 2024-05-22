A, B, C, D, E, F, G
init A
accept C, F
A:
	1, 1, >, B
:
B:
	1, 1, >, C
	0, 0, >, D
:
C:
	1, 1, >, G
	0, 0, >, D
:
D:
	1, 1, >, E
	0, 0, >, D
:
E:
	_, _, =, F
:
F:
:
G:
	0, 0, >, D
	1, 1, >, G
	_, _, =, F
:
