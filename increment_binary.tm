a, b, c, d, e, f, g, h, x
init a
accept x
a:
	0, 0, >, a
	1, 1, >, a
	_, _, <, b
:
b:
	1, 0, <, b
	0, 1, S, x
	_, 1, S, x
:

