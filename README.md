# Lisp Interpreter

## Project Overview
This project is a Lisp interpreter that is designed to read and execute Lisp code efficiently. The interpreter is built to support many of the fundamental features of Lisp programming, making it suitable for educational purposes as well as practical applications.

## Goals
- To provide a fully functional Lisp interpreter that adheres to the principles of Lisp programming.
- To facilitate learning and understanding of Lisp through implementation.

## Features
- A complete parser for Lisp expressions.
- Arithmetic operations, logical operations, and conditional evaluation.
- Support for user-defined functions and variables.
- Built-in functions for common tasks.
- Error handling and debugging capabilities.

## How It Works
The interpreter reads Lisp code in the form of expressions and evaluates them using a recursive approach. The parsing process breaks down expressions into manageable parts, while the evaluation phase processes these parts according to Lisp semantics.

## Project Structure
- `src/`: Contains the main source code for the interpreter.
- `tests/`: Includes unit tests for various components of the interpreter.
- `docs/`: Documentation related to the project.
- `examples/`: Sample Lisp programs to demonstrate functionality.

## Getting Started Guide
1. Clone the repository:
   ```bash
   git clone https://github.com/YUNTZU-YANG/Lisp-interpreter.git
   cd Lisp-interpreter
   ```
2. Install the necessary dependencies (if any).
3. Run the interpreter:
   ```bash
   python interpreter.py
   ```

## Usage Examples
- Evaluating simple expressions:
   ```lisp
   (+ 1 2)
   ```
- Defining a function:
   ```lisp
   (defun square (x) (* x x))
   ```

## Supported Features
- Basic arithmetic operations (+, -, *, /)
- List manipulation functions (car, cdr, cons)
- Function definition and invocation

## Implementation Details
This Lisp interpreter is written in Python and utilizes recursive functions to handle parsing and evaluation of expressions. The design pattern follows a modular approach for better maintainability.

## Course Information
This project is part of a course on programming languages and interpreters. It aims to provide hands-on experience in building language interpreters and understanding the underlying concepts.