equation(L,LT,RT) :-
   split(L,LL,RL),              % decompose the list L
   term(LL,LT),                 % construct the left term
   term(RL,RT),                 % construct the right term
   LT =:= RT.                   % evaluate and compare the terms

% term(L,T) :- L is the list of numbers which are the leaves in
%    the arithmetic term T - from left to right.

term([X],X).                    % a number is a term in itself
term(L,T) :-                    % general case: binary term
   split(L,LL,RL),              % decompose the list L
   term(LL,LT),                 % construct the left term
   term(RL,RT),                 % construct the right term
   binterm(LT,RT,T).            % construct combined binary term

% binterm(LT,RT,T) :- T is a combined binary term constructed from
%    left-hand term LT and right-hand term RT

binterm(LT,RT,LT+RT).
binterm(LT,RT,LT-RT).
binterm(LT,RT,LT*RT).
binterm(LT,RT,LT/RT) :- RT =\= 0.   % avoid division by zero

% split(L,L1,L2) :- split the list L into non-empty parts L1 and L2
%    such that their concatenation is L

split(L,L1,L2) :- append(L1,L2,L), L1 = [_|_], L2 = [_].

solver(_, -1).
solver(F, N):-
   read(F, L),
   ((equation(L, LT, RT),
      writef("%w=%w\n", [LT, RT])); writef("IMPOSSIBLE\n")),
   N1 is N - 1,
   solver(F, N1).

main :-
   open('input.txt', read, F),
   read(F,N),
   solver(F, N).
