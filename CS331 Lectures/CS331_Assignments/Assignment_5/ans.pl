subList([], _).
subList([H|R], [H|T]) :- subList(R, T).
subList([F|R], [H|T]) :- F \== H, subList([F|R], T).

squareroot(N, X, L) :- N >= 0, N1 is N / 2.0, newton(N, N1, L, X).
newton(N, Root, Err, Root) :- abs(Root * Root - N) < Err.
newton(N, Root, Err, X) :- abs(Root * Root - N) >= Err, NewRoot is 0.5*(Root+(N/Root)), newton(N, NewRoot, Err, X).

