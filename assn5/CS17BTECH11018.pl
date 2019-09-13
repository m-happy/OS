%Programming assignmenent in prolog.
% work done by CS17BTECH11018

expression(L,LHS,RHS) :-
   split(L,LL,RL),               
   leaves(LL,LHS),                
   leaves(RL,RHS),                
   LHS =:= RHS.                          


% split(L,L1,L2) :- splits the list L into non-empty parts L1 and L2
%    such that their concatenation is L and the only element in L2 is last element of the list

split(L,L1,L2) :- append(L1,L2,L), L1 = [_|_], L2 = [_].


% binleaves(LHS,RHS,T) :- T is a combined binary leaves constructed from
%    left-hand leaves LHS and right-hand leaves RHS

binleaves(LHS,RHS,LHS+RHS).
binleaves(LHS,RHS,LHS-RHS).
binleaves(LHS,RHS,LHS*RHS).
binleaves(LHS,RHS,LHS/RHS) :- RHS =\= 0.   % avoids division by zero


% leaves(L,T) :- L is the list of numbers which are the leaves in
%    the arithmetic leaves T - from left to right.

leaves([X],X).                 
leaves(L,T) :-                  
   split(L,LL,RL),             
   leaves(LL,LHS),                
   leaves(RL,RHS),                 
   binleaves(LHS,RHS,T). 


check(_, 0).
check(F, N):-
   read(F, L),
   ((expression(L, LHS, RHS),
      writef("%w=%w\n", [LHS, RHS])); writef("IMPOSSIBLE\n")),
   N1 is N - 1,
   check(F, N1).

main :-
   open('input.txt', read, F),
   read(F,N),
   check(F, N),
   close(F).
