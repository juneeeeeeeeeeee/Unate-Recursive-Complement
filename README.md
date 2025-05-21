# Unate Recursive Complement Calculator in C

This project is a C implementation of the **Unate Recursive Complement (URC)** algorithm used in logic minimization.  
Given a Boolean function in **sum-of-products** form, it computes the **complement** of the function using the URC technique.

## 📚 Overview

The Unate Recursive Complement algorithm is used to find the complement of a Boolean function represented in cube format.  
Each cube represents a product term, and the function is the sum of these cubes.

This C program supports:
- Arbitrary number of Boolean variables (up to 20)
- Sum-of-products input in `.pcn` format
- Complement computation via recursive decomposition
- Output result in cube format

## 🛠 File Format (Input)

Input files must follow the **.pcn format**:
