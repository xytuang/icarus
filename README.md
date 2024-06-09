Icarus is a dynamically typed language that has the following features.

## Supported data types:<br>
Booleans <br>
        true;
        false;
Numbers <br>
        12    // an integer 12
        12.25 // a decimal 12.25
Strings <br>
        "some string"
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
Less than or equal (number1 <= number<br>
Greater than (number1 > number2)<br>
Greater than or equal (number1 >= number2)<br>
Equal (number1 == number2)<br>
Not equal (number1 != number2) <br>

not (!some_boolean) <br>
and (boolean1 and boolean2) <br>
or (boolean1 or boolean2) <br>

#### Grouping <br>
Grouping using brackets (eg. (2 + 2 + 2)/3 = 2)


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

