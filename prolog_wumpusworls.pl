% Knowledge base

% Dimensions of the world
world_size(4, 4).

% Facts: locations of Wumpus, pits, and gold
wumpus(2, 3).
pit(3, 1).
pit(1, 3).
pit(4, 4).
gold(3, 3).

% Safe if no Wumpus or pit
safe(X, Y) :-
    \+ wumpus(X, Y),
    \+ pit(X, Y).

% Perceptions
stench(X, Y) :-
    wumpus(X1, Y1),
    adjacent(X, Y, X1, Y1).

breeze(X, Y) :-
    pit(X1, Y1),
    adjacent(X, Y, X1, Y1).

glitter(X, Y) :-
    gold(X, Y).

% Adjacent cells (4 possible moves)
adjacent(X, Y, X1, Y) :- X1 is X + 1, world_size(W, _), X1 =< W.
adjacent(X, Y, X1, Y) :- X1 is X - 1, X1 > 0.
adjacent(X, Y, X, Y1) :- Y1 is Y + 1, world_size(_, H), Y1 =< H.
adjacent(X, Y, X, Y1) :- Y1 is Y - 1, Y1 > 0.

% Moving in the world (avoiding visited positions)
move(X, Y, X1, Y, Visited) :- 
    adjacent(X, Y, X1, Y),
    safe(X1, Y),
    \+ member((X1, Y), Visited).

move(X, Y, X, Y1, Visited) :- 
    adjacent(X, Y, X, Y1),
    safe(X, Y1),
    \+ member((X, Y1), Visited).

% Find gold
find_gold(X, Y) :-
    glitter(X, Y),
    format('Gold found at (~w, ~w)~n', [X, Y]).

% Explore the world (avoiding infinite loops)
explore(X, Y, Visited) :-
    find_gold(X, Y).

explore(X, Y, Visited) :-
    move(X, Y, X1, Y1, Visited),
    format('Moving to (~w, ~w)~n', [X1, Y1]),
    explore(X1, Y1, [(X1, Y1) | Visited]).

% Starting point
start :-
    write('Exploring the Wumpus World from (1,1):'), nl,
    explore(1, 1, [(1, 1)]).

% Auto-execute the start predicate on load
:- initialization(start).

