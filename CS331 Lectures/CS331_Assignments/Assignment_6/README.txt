Assignment-6
Advanced Prolog Programming

Kartikeya Saxena
Roll Number: 180101034

dependencies on UBUNTU system:
sudo apt-get install swi-prolog-nox
sudo apt-get install gprolog

Solution:
I have used BFS to construct the solution of the shortest path from source to destination. So the predicate shortest_path(Src, Dst, Path) calls the bfs(Queue, Src, Dst, Path) and after the bfs is complete
the retrieve_path([CurrLoc|RestOfPath], Path, Src) constructs the shortest path.

Note: You might get partial lists on the screen while running shortest_path query, press w to get the full list.
To run use:
____________________________________________________________________________________________________________
Test Case 1:
swipl
[test1].
[ans].
shortest_path(11, 99, X).
 X = [11, 12, 13, 14, 24, 34, 44, 54, 55, 56, 66, 76, 77, 87, 97, 98, 99] .
clear_loading(test1).


For next test you can either close swipl terminal and reopen again or just continue from the terminal(without reloading ans.pl again) since clear_loading removes all the previous test's loadings.
____________________________________________________________________________________________________________
Test Case 2:
swipl
[test2].
[ans].
shortest_path(11, 99, X).
 X = [11, 21, 22, 32, 42, 52, 62, 72, 82, 92, 93, 94, 95, 96, 97, 98, 99] .
add_faulty_node(21).
 true.
shortest_path(11, 99, X).
 X = [11, 12, 22, 32, 42, 52, 62, 72, 82, 92, 93, 94, 95, 96, 97, 98, 99] .
remove_faulty_node(21).
 true.
clear_loading(test2).


____________________________________________________________________________________________________________
Test Case 3:
swipl
[test3].
[ans].
shortest_path(16, 144, X).
 false
clear_loading(test3).

____________________________________________________________________________________________________________
Test Case 4:
swipl
[test4].
[ans].
shortest_path(11, 99, X).
 X = [11, 12, 32, 52, 53, 54, 55, 75, 76, 96, 97, 98, 99] .
clear_loading(test4).

Note: you don't need to load [ans]. every time if you are not closing swipl. The third test case is showing false because there is no path 
from source to destination. You can use shortest_path multiple times with different valid source and destination of your choice. You cannot
remove a faulty node that is not there and cannot add a faulty node that is already there.
