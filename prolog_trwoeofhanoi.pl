
% Base case: Move one disk directly from Source to Destination
move(1, Source, Destination, _) :-
    format('Move disk from ~w to ~w~n', [Source, Destination]).

% Recursive case: Move N disks from Source to Destination using Auxiliary
move(N, Source, Destination, Auxiliary) :-
    N > 1,
    M is N - 1,

    % Move the top N-1 disks from Source to Auxiliary using Destination as a temporary peg
    move(M, Source, Auxiliary, Destination),

    % Move the remaining disk directly from Source to Destination
    move(1, Source, Destination, _),

    % Move the N-1 disks from Auxiliary to Destination using Source as a temporary peg
    move(M, Auxiliary, Destination, Source).

% Hardcoded start for 3 disks
start :-
    write('Solving Tower of Hanoi with 3 disks:'), nl,
    move(3, 'A', 'C', 'B').

% Auto-execute the start predicate on load
:- initialization(start).
