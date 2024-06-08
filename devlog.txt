High level overview of how an interpreted programming language works:
1. Take user input
2. Turn user input into tokens (scannning phase)
3. Group tokens together based on grammar to form your abstract syntax tree 
   (parsing phase)
4. Interpret the abstract syntax tree (interpreting phase)


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



26 May 2024:
Made a very minimalist interpreter that can only do arithmetic and logical
comparisons. Was stuck on the interpreter for awhile because of 2 bugs in
interpreter.cpp.

The bug:

std::any left = expr->left;
std::any right = expr->right;

Corrected the bug to:

std::any left = evalaute(expr->left);
std::any right = evaluate(expr->right);

Essentially forgot to evaluate the operands on each side before tyring to
operate on them. Learned the job of an interpreter: to produce an output based
on an abstract syntax tree/expression. Expressions and abstract syntax trees are
synonymous with each other.


27 May 2024:
Working on statements. Incorporated print statements and expression statements
so far. Encountered bug where "print true" and "print false" would result in
errors. Fixed the bug when I realised the error was due to incorrect scanning,
which propagated to parsing in line 79 of scanner.cpp. Fixed line was initially:

std::string text = source.substr(start, current);

Changed to:

std::string text = source.substr(start, current - start)


Working on environment to allow declaration of variables (see env.h).


29 May 2024:
Implemented control flow  (see class If and class While in stmt.h, class Logical
in expr.h). if-else statements are written as such in icarus:

if (some_condition) {
    ...execute code...
}
else {
    ...some other code...
}

***else branch is optional.

while loops are written as such in icarus:

while (some condition) {
    ...some code...
}

For loops are just syntactic sugar and uses similar functionality as while loops.
They are written as such in icarus:

for (var i = 0; i < 10; i += 1) {
    print i;
}


See samples for concrete examples. The parentheses must be present (this isn't
python). For some concrete examples of if/else and loops, see samples/loops

icarus also supports defining, assigning and using variables. See class Var in
stmt.h for initializing variables. See class Variable in expr.h for accessing
variables. See class Assign in expr.h for assigning new values to predefined
variable.

Variable names and their values are saved in a map stored in an Environment object. 
See env.h for details.  

Scoping is also supported by using {}, allowing for multiple statements inside
for loops, while loops, if-else, or in general if you want blocks. See class
Block in stmt.h for details. 


Started working on implementing functions. Functions are implemented as
expressions (see class Call in expr.h). A function call looks like this:

func(arg1, arg2...)

To parse a function correctly, start parsing tokens from left to right. Treat
the function name as you would an expression if there are no parentheses found
following the function name. The number of arguments per function is limited to
255.




1 June 2024:
Implemented return statements and closures. Closures started making sense when I
implemented them (also because it was particularly easy to implement). Closures
just hold on to surrounding variables where the function is declared. The
implementation was to have an Environment member variable for each function that
held onto surrounding variables. 

Return statements were particularly interesting, especially when you consider
what should happen in a recursive function. The stack needs to unwind, and so
the idea was to piggback of an exception that would unwind the stack but not 
throw an exception. 

2 June 2024:
Implemented resolutions and bindings. Previously, this code would not resolve
properly:

var a = "global"
{
    fun showA() {
        print a;
    }

    showA();
    var a = "block";
    showA();
}

The code should output "global" two times, each time on a separate line.
However, we get "global" once and then "block". This bug occurs because of the
way we declare functions. Without resolution/binding, the environments that
functions exist in are dynamic (aka they can differ as more variables are
declared). Instead, the environment should be static/unchanging. 

Implementation process of resolution. Resolution is a single pass over the
abstract syntax trees after parsing and right before interpretation. In that
pass, we are interested in several things:
1. Block statement introduces a new scope for the statements inside it.
2. Function declaration introduces new scope for its body and binds its
paramters to that scope.
3. Variable declaration adds new variable to current scope.
4. Variable and assignment expressions need to have their variables resolved.

To achieve a static environment, we map each variable/expression to a certain
environment. Since environments are nested within each other, we just need to
find out how many levels/environments up we need to go. Then everytime we use
that variable, we know where to look for its value. 
