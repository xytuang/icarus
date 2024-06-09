Icarus is a dynamically typed language that is implemented in c++. It has the following features.

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

nil (the equivalent of null in Java or nullptr in c++)<br>

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

not (!some_boolean) <br>

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

