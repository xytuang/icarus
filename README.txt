18 May 2024:
Set up


19 May 2024:
Added tokens (numbers, letters, operators like less than, equal etc.) Currently
working on how to parse different tokens.

20 May 2024:
Finished parsing tokens (see scanner.cpp and scanner.h). Now working on parsing
expressions. Think of expressions as a group of tokens. Expression classes are
being written using metaprogramming (see generateast.cpp) in tools folder.

21 May 2024:
Finished metaprogramming of expression classes. Learned a lot about c++ today,
particularly about polymorphimism in c++.
I created an abstract Expr class. I then implemented concrete classes (Binary,
Grouping, Literal and Unary), which were child classes of Expr. Incorporated the
Visitor Design pattern, where each class had its own accept method, which would
invoke a specific method in the Visitor class in the Expr class.

Challenges faced
1. virtual functions cannot have templates. Struggled for at least 2 hours(?)
before realizing this. Intially I wanted to integrate templates into expr.h but
realized that this was over-engineering, as I only need to handle strings, so
there was no usecase for templates. Solution was to get rid of templates. Was a
learning experience though.

2. Cannot instantiate objects of abstract classes. Each child class (binary,
grouping etc.) had a member variable of type Expr. Struggled with this until I
realised I could make the member variables Expr* instead of Expr. In hindsight,
I should have relied on my java experience to understand this issue. Struggled
for another 2 hours on this. Solution was to turn Expr variables into Expr*.

Expr someExpr = new Binary(//some arguments) BAD

Expr* someExpr = new Binary(//some arguments) GOOD

The second works because a pointer to an Expr can be an expression to any of its
subclasses. The first is bad because we have to instantiate an abstract class,
which is impossible.

3. Dealing with variadic arguments was an experience. Each subclass (binary,
grouping etc) had different number of arguments but I wanted a function that
handled all of them. I tried to use templates to handle variadic arguments, but
ended up fighting the compiler. Solution was to turn the arguments into an
initializer list, which I passed to a vector inside the function. Made life way
easier. Struggled for another 2 hours.

24 May 2024:
I decided to implement Token class using std::any as the datatype for the
"literal" member variable. This was to allow a Token object to hold literals of
different datatypes (int, double, string etc). In this specific context, std::any 
is similar to the Object class of java, where variables of type "Object" could
hold different datatypes. The only worry I have for using std::any is that I
have to cast std::any to the correct type before using the value stored in it,
which I suspect will be very error-prone. The code also becomes very bloated
because of all the casting required.

25 May 2024:
Correction to the entry on 21 May 2024. Virtual functions can have generics.
Example:

template <typename R>
class myClass {
    virtual R myMethod() = 0;
}

The above is fine. The following is not:

class myClass {
    template <typename R>
    virtual R myMethod() = 0;
}

Knowing this, I corrected generateast.cpp, expr.h and parser.h. I also included 
templates wherever expr objects were needed. I also moved the function
implementations in parser.cpp into parser.h as the functions were all template
functions. parser.cpp is now removed from the Makefile.






















