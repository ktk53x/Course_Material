 :- dynamic faultynode/1.

/* finds all possible next locations from the current location */
all_possible_next(CurrLoc, NextLocs) :- findall(parent(NextLoc,CurrLoc), possible_next(CurrLoc, NextLoc), NextLocs). 

/* find a possible next location from the current location mark it visited and make the current location parent of this possible next location */
possible_next(CurrLoc, NextLoc) :- mazelink(CurrLoc, NextLoc), \+ faultynode(NextLoc), \+ visited(NextLoc), assertz(visited(NextLoc)), assertz(parent(NextLoc, CurrLoc)).

/* retrieve the shortest path after bfs is complete */ 
retrieve_path([Src|RestOfPath],[Src|RestOfPath], Src).
retrieve_path([CurrLoc|RestOfPath], Path, Src) :- parent(CurrLoc, NextLoc), retrieve_path([NextLoc, CurrLoc|RestOfPath], Path, Src).

/* performs bfs from source position to destination position and erases all the temporary dynamic facts */
bfs([parent(Dst, _)| _], Src, Dst, Path) :- !, retractall(visited(_)), retrieve_path([Dst], Path, Src), retractall(parent(_, _)).
bfs([parent(CurrLoc, _)| RestOfQueue], Src, Dst, Path) :- all_possible_next(CurrLoc, NextLocs), append(RestOfQueue, NextLocs, Queue), bfs(Queue, Src, Dst, Path).  

/* mark Src visited and push it into queue with parent -1 */
shortest_path(Src, Dst, Path) :- retractall(visited(_)), retractall(parent(_,_)), assertz(visited(Src)), bfs([parent(Src, -1)], Src, Dst, Path).

/* to add and remove faulty node dynamically */
remove_faulty_node(X) :- retract(faultynode(X)).
add_faulty_node(X) :- \+ faultynode(X), assertz(faultynode(X)).

/* remove loading file */
clear_loading(X) :- unload_file(X), retractall(faultynode(_)), retractall(visited(_)), retractall(parent(_, _)).
