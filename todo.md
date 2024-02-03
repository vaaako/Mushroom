- [X] When a lunarbat error occurs, cause Memory leaks, because some memory was not deleted
	+ Lexer and Parser need to have *"LexerResponse"* and *"ParserResponse"*
- [X] Make respose better
	+ Currently is very simple
	+ Better logic
> ^ Make it to have only "errorMessage" and "has_error" fields,
 the result is stored by the class as a method

> ^ This can sound like a bad design choice, but i makes sense because the result will
 be only used while the class stills allocated, so when deleting the class, the class
 also deletes the method

> Later: I had to add "value" field again, but I made it optional, it's way better this way


- [X] Make a Lunarbat method
	+ Execute all of **Lexer**, **Parser** and stuff
	+ To not re-write on *command line* code
> ^ Done


- [X] `BinaryExpr` operator doesn't show on **debug**
	+ ???

- [X] `Identifier` type cause an error *(`std::bad_alloc`)*
	+ ???
> ^ The two problems above was solved the same way

> ^ Turns out is not a good idea to store a class variable as reference, because can
 lead to weird errors like this



- ~[?] Lunarbat errors on `parser.cpp` don't show **filename** and return only half of the error content~
	+ ~???~
> ^ Almost sure the error solved itself (computer magic)


- [X] Unary support
> ^ It was actually really easy to implement

- [X] `3 (1 + 1)` is valid *(`x (..)`)*
> ^ Took me some minutes to figure it out but i solved

> ^ [!!!] When adding Semicolon support I will need to come back with the while-loop from before

- [X] `x + 1` crashes
	+ `terminate called after throwing an instance of 'std::out_of_range' what():  array::at: __n (which is 1431655834) >= _Nm (which is 3)`
	+ ???
> ^ The error was partionally solved, it was a ownership error, I changed the ownership of the element in "symbolsTable" and when I tried to
> acess it's type it returned a random number like "1431655834". I'm not sure how to solve this, what is the "right way" of doing this, but
> "temporarily" solved by removing the for-loop that printed all the variables in envrionment

> ^ I discovered what it was... after 2 hours... i am so tired and lazy to explain here
> read the devlog of day 10 (same day)

> ^ Finally solved

> ^ Solved for real now... i don't want to talk about it... read Day 11


- [X] Nodes need to have `Position`
	+ To solve below problem and Handl Error on `interpreter.cpp`

- [X] Handle error of *Environment*
	+ See `interpreter.cpp`
	+ To do this *Nodes* will need to have `Postion` and *RuntimeVal* as well
> ^ It a bit confusing, but works


- [X] Change `expected` on `parser`

- [ ] Pow support
	+ More priority than Mul
	+ Not that hard to implement I think, just add `parse_pow_expr` and add it to `parse_add_expr`

- ~[-] Put type check from `interpreter.cpp` to `envrionment.cpp`~

- [ ] Semicolon support

- [ ] Int and Float support

- [X] Optimize `lunarbat.cpp`


# Stuff I can do later
- ~[-] Change lexer while-loop to for-loop?~
	+ ~`for(char ch : source)`~
	+ ~I think the speed doens't changes, so a `for-loop` might be a better choice~
> ^ Forget it, I need the while-loop, because some statements I need to start a new loop (continue) but not advance the character

- ~[-] Use `stack-allocation` to store variables~
	+ ~I tested and I got the same time for `stack-allocation` and `hash-map`~

- ~[ ] See if `std::vector<const Token*>` does some change *(instead of alloc the vector itself)*~
	+ ~Or `std::vector<const Token&>`?~
	+ ~I don't know if this would work~
> ^ Maybe this wouldn't work because I need vector to be a pointer so I can delete the tokens inside
> and this would cause to clone the vector twice, on returning and on passing to parser

- [ ] Make type safe on var declaration

- ~[ ] In `envrionment.cpp` make position be a reference?~
> ^ Not using position anymore, and yes, the new one is a reference

- [ ] In parser check if is trying to assing or declare a global variable
	+ Currently this will throw an error from interpreter

- [ ] `obj.y` -> check if exists inside of object -> if not, check if is a variable on envrionment -> if not, error
	+ ~This will avoid the clone to inside ObjectVal
	+ ~But I think it can also cause some errors, but I can test it later
> ^ Solved

- [ ] Make possible to make a executable file
	+ `lunarbat -e main.lb` -> `./main`
