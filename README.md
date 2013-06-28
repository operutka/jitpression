Jitpression
===========
Jitpression is a just-in-time compiler for arithmetic expressions. Currently it is in early development state. 
It supports following features:
 * all valid expressions with:
   * +, -, *, / binary operators
   * - left unary operator
   * symbols (named variables)
   * integral constants
 * basic integral operations (using operators as described obove)
 * standard operation priority (unary operators has higher priority than binary operators, * and / has higher 
   priority than + and -, operations with same priority are evaluated from left to right)
 * platforms:
   * Linux x86_64

Compilation
-----------
You need GCC C++ compiler and make tool, there is no special dependency. To build Jitpression just type "make" into your 
command line. Simple test application is available in the "dist" subdir after compilation.

Testing
-------
The test application compares evaluation time between compiled and interpreted expressions. To test an expression you 
need to enter the expression and values of its variables and enter a number of test runs.

For example we want to test the formula for sum of an arithmetic progression (n * (a1 + an) / 2):

    ./test
    Enter an expression:
    n * (a1 + an) / 2
    Enter 3 integral arguments:
    10 1 10
    Enter number of test runs:
    100000000
    Testing interpreter...
    100000000 test runs took 3.4605 seconds (with result 55).
    Testing compiled function...
    100000000 test runs took 0.539827 seconds (with result 55).
