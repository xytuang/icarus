Icarus is a dynamically typed language that is implemented in c++. It has the following features.

# NOTE: Icarus leaks memory because I have not integrated garbage collection into the language and also because this is written in C++. Yes I am working on it.

## Supported data types:<br>
Booleans <br>

        true;
        false;

Numbers <br>

        12    // an integer 12
        12.25 // a decimal 12.25

Strings <br>

        "some string"

Note that Icarus uses double quotes and not single quotes for strings

<code>nil</code> (the equivalent of null in Java or nullptr in c++)<br>

## Supported operators and their representation:<br>
#### Arithmetic <br>
Addition <br>

        5 + 2 // gives 7

Subtraction <br>

        5 - 2 // gives 3

Multiplication <br>

        5 * 2 // gives 10

Division<br>

        5 / 2 // gives 2.5

Note that numbers are automatically converted to floating point numbers under the hood, which is why <code>5/2</code> gives 2.5.

Negation<br>

        -5 //-5 duh

#### Logical <br>
Less than<br>

        5 < 2 // gives false

Less than or equal <br>

        5 <= 2 // gives false

Greater than <br>

        5 > 2 // gives true

Greater than or equal <br>

        5 >= 2 // gives true

Equal <br>

        5 == 2 // gives false

Not equal <br>

        5 != 2 // gives true

not <br>

        !true // gives false

and <br>

        true and false // gives false

or <br>

        true or false // gives true

#### Grouping <br>
Grouping using brackets<br>

        (2 + 2 + 2)/3 //gives 2


## Statements:<br>
print statements<br>

        print "a string";
        print 1;

Note how statements have to end with semicolons

For grouping statements together, use a block

        {
            print "statement1";
            print "statement2";
        }

## Variables
Variables are declared using the <code>var</code> keyword. If you do not initialize a variable, it is set to <code>nil</code>.

        var someString = "some string";
        var notInitialized; // set to nil

## Control Flow
Icarus supports <code>if</code> and an <code>else</code> statements. There is no else if in Icarus!

        if (some_condition) {
            print "inside if";
        }
        else {
            print "inside else";
        }

Icarus also supports while loops:

        var i = 0;
        while (i < 10) {
            print i;
            i = i + 1;
        }

Note how there is no <code>i++</code> or <code>i += 1</code> as you might expect in other languages.

Icarus also supports for loops:

        for (var i = 0; i < 10; i = i + 1) {
            print i;
        }

for loops are similar to those in java or c++. Again, note the <code>i = i + 1</code>


## Functions
To define a function, use the keyword <code>fun</code> like so:

        fun addThreeNumbers(a,b,c) {
            print a + b + c;
        }

To use a function, simply call it like such:

        addThreeNumbers(1,2,3)

Note how the arguments are inside the parentheses.

Functions can also return values. If you do not return a value inside your function, the function implicitly returns nil.

## Classes
To define a class in Icarus, use the <code>class</code> keyword like so:

        class Bear {
            init(name) {
                this.name = name;
            }
            sound() {
                print "GROWWL";
            }
            feed(food) {
                print "I like " + food + "!";
            }
        }

The init method initializes an instance of that class. In the above example of Bear, we would do

        var bob = Bear(); // bob is an instance of a Bear

The methods of a class (like sound and feed for the Bear class above), are defined in a similar manner as functions, but without the <code>fun</code> keyword.

To call a method of an instance, it would be something like this:

        bob.sound() // GROWWL
        bob.feed("honey"); // I like honey!


