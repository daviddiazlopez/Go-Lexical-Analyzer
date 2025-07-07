# Go Lexical Analyzer - C Implementation

This project presents a C application designed to perform **lexical analysis** of a subset of the Go programming language. It was developed as part of the *Compilers and Interpreters* course assignment.

## Features

- **Lexical Analysis**: Scans a `.go` source file, identifies and categorizes lexemes (tokens), and prints them as tuples in the form `(lexical_component, lexeme)`.
- **Symbol Table Management**: Implements a Symbol Table (ST) using a Binary Search Tree (BST) to efficiently store reserved words and user-defined identifiers.
- **Module Integration Simulation**: Includes a stub for the syntactic analyzer module, which demonstrates how components would be integrated in a full compiler pipeline. No actual syntactic analysis is performed.
- **Error Handling**: Centralized module for detecting and reporting lexical and input-related errors. Handles issues such as missing source files, unterminated strings or comments, malformed numeric literals (hexadecimal, binary, octal, exponential), overly long lexemes, and internal input system faults.
- **Clean Execution**: Dynamically allocated memory is properly freed and all files are closed after execution.

## Technologies Used

- **C Programming Language**: The entire project is implemented in modular C.
- **Binary Search Tree (BST)**: Used to implement the symbol table for token storage.
- **Makefile**: Automates the build process for all modules.
- **Bash Scripting**: Includes a compilation helper script for Unix-based environments.

## Project Structure

| File / Module             | Description                                                                                          |
|--------------------------|----------------------------------------------------------------------------------------------------|
| `main.c`                 | Program entry point: responsible for structure initialization, starting compilation (calling the syntactic analyzer), and memory cleanup with structure destruction. |
| `analisis_sintactico.c/.h` | Simulated syntactic analyzer stub: calls lexical analyzer until EOF, prints tokens, frees memory.  |
| `analisis_lexico.c/.h`   | Lexical analyzer: tokenizes input source code into lexical components and lexemes.                 |
| `sistema_entrada.c/.h`   | Buffered input system: reads source file character by character, manages input buffering.           |
| `TS.c/.h`                | Symbol Table implementation using a Binary Search Tree to store reserved words and identifiers.     |
| `ABB.c/.h`               | Generic Binary Search Tree implementation used by the Symbol Table module.                          |
| `errores.c/.h`           | Centralized error reporting and handling functions.                                                |
| `definiciones.h`         | Shared definitions of lexical component codes (e.g., reserved words, operators, literals, etc.).    |
| `concurrentSum.go`       | Example Go source file to analyze.                                                                 |
| `compilar.sh`            | Shell script that compiles the entire project using the Makefile.                                  |
| `Makefile`               | Automates compilation of all modules and linking into the executable.                              |
| `README.txt`             | Basic compilation instructions and usage details (in Galician).                                   |

poñemos o de autómata finito determinista, o tipo de sistema de entrada que se usa
