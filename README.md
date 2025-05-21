# Unate Recursive Complement Calculator in C

This project is a C implementation of the **Unate Recursive Complement (URC)** algorithm used in logic minimization.  
Given a Boolean function in **sum-of-products** form, it computes the **complement** of the function using the URC technique.

## Overview

The Unate Recursive Complement algorithm is used to find the complement of a Boolean function represented in cube format.  
Each cube represents a product term, and the function is the sum of these cubes.

This C program supports:
- Arbitrary number of Boolean variables (up to 20)
- Sum-of-products input in `.pcn` format
- Complement computation via recursive decomposition
- Output result in `.pcn` format

## File Format (Input/Output)

Input files must follow the **.pcn format**:
<total_variable_count>
<cube_count>
<term1_var_count> <var1> <var2> ...
<term2_var_count> <var1> <var2> ...
...
Output files follow the same format. 

### Example Input (`example.pcn`)

For the Boolean function:
f(x1,x2,x3,x4,x5) = x2x3x4 + x1'x5 + x1x3'x4'

Input file would look like:
5
3
3 2 3 4
2 -1 5
3 1 -3 -4
