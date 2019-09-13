operation(L,LT,RT) :- 
    split(L,LL,RL),
    term(LL,LT),
    term(RL,RT),
    LT =:= RT.

term([X],X).                   
term(L,T) :-                   
    split(L,LL,RL),             
    term(LL,LT),                 
    term(RL,RT),                 
    binterm(LT,RT,T). 

split(L,L1,L2) :- append(L1,L2,L), L1 = [_|_], L2 = [_].

binterm(LT,RT,LT+RT).
binterm(LT,RT,LT-RT).
binterm(LT,RT,LT*RT).
binterm(LT,RT,LT/RT) :- RT =\= 0.

check(_,0).
check(Ptr, N):-
   read(Ptr, L),
   ((operation(L, LT, RT),
    writef("%w = %w\n",[LT,RT])); writef("IMPOSSIBLE\n")),
   N1 is N - 1,
   check(Ptr, N1).

main :- 
    open('input.txt',read,Ptr),
    read(Ptr,N),
    check(Ptr,N),
    close(Ptr).