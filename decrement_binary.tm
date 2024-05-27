a, b, c, d, e, f, t, n
init a
accept t, n
a:
	0, 0, >, a
	1, 1, >, a
	_, _, <, b
:
b:
	1, 0, S, t
	0, 0, <, c
:
c:
	0, 0, <, c
	_, _, S, n
	1, 0, >, d
:
d:
	0, 1, >, d
	_, _, S, t
:

	
