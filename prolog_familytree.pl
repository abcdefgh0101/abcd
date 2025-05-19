/* Basic Family Tree Program */

% Facts about parents
parent(john, mary).    % John is Mary's parent
parent(john, tom).     % John is Tom's parent
parent(mary, ann).     % Mary is Ann's parent
parent(mary, bob).     % Mary is Bob's parent
parent(tom, lucy).     % Tom is Lucy's parent
parent(tom, emma).     % Tom is Emma's parent

% Facts about gender
male(john).
male(tom).
male(bob).
female(mary).
female(ann).
female(lucy).
female(emma).

% Rules for family relationships
father(Father, Child) :-
    parent(Father, Child),
    male(Father).

mother(Mother, Child) :-
    parent(Mother, Child),
    female(Mother).

grandparent(GP, GC) :-
    parent(GP, Parent),
    parent(Parent, GC).

sibling(X, Y) :-
    parent(P, X),
    parent(P, Y),
    X \= Y.  % X and Y are different people

aunt_or_uncle(A, N) :-
    parent(P, N),
    sibling(A, P).

cousin(X, Y) :-
    parent(P1, X),
    parent(P2, Y),
    sibling(P1, P2),
    X \= Y.

% Query examples:
% ?- father(X, mary).    % Who is Mary's father?
% ?- mother(X, bob).     % Who is Bob's mother?
% ?- grandparent(john, X). % Who are John's grandchildren?
% ?- sibling(ann, bob).  % Are Ann and Bob siblings?