# Go Lexical Analyzer - C Implementation

This project presents a lexical analyzer developed in C, designed to extract the lexical components from a subset of the Go programming language. It was created as part of a course assignment for the Compilers and Interpreters subject in the Computer Engineering degree at the University of Santiago de Compostela (USC).

## Description

The goal of this project is to build a lexical analyzer capable of identifying and classifying lexical components in a Go source file, specifically `concurrentSum.go`. Rather than analyzing the entire Go language, this analyzer focuses on a representative subset sufficient to process the content of the provided file.

The program is structured modularly to separate concerns such as lexical analysis, symbol table management, error handling, and input system control. It prints the lexical components found in the source file to standard output for evaluation and debugging purposes.


## Features

- **Lexical Analysis**: Scans a `.go` source file, identifies and categorizes lexemes (tokens), and prints them as tuples in the form `(lexical_component, lexeme)`.
- **Symbol Table Management**: Implements a Symbol Table (ST) using a Binary Search Tree (BST) to efficiently store reserved words and user-defined identifiers.
- **Reserved Keyword Handling**: All reserved Go keywords used in the target file are preloaded into the symbol table to distinguish them from user-defined identifiers.
- **Deterministic Finite Automata (DFA)**: Lexical recognition is implemented through a switch-based state machine that simulates DFAs for different token types (identifiers, integers, operators, etc.).
- **Generalized Lexeme Recognition**: Despite focusing on one Go source file, the analyzer generalizes number parsing, identifier handling, and symbol classification.
- **Input System with Sentinel Method**: Uses a two-buffer scheme with sentinels (EOF) to simulate continuous input. Pointers `inicio` and `delantero` handle scanning, lookahead, and rollback (`devolver()`), while ensuring efficient reloading and detection of oversized lexemes.
- **Module Integration Simulation**: Includes a stub for the syntactic analyzer module, which demonstrates how components would be integrated in a full compiler pipeline. No actual syntactic analysis is performed.
- **Error Handling**: Centralized module for detecting and reporting lexical and input-related errors. Handles issues such as missing source files, unterminated strings or comments, malformed numeric literals (hexadecimal, binary, octal, exponential), overly long lexemes, and internal input system faults.
- **Terminal Output**: Outputs lexical components in the format: <TOKEN_CODE, "lexeme">
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

## Output Overview

Upon execution, the program performs the following actions:

- **Prints the initial Symbol Table**, which includes all **Go reserved words** used in the input file.
- **Performs lexical analysis** on **`concurrentSum.go`**, printing each recognized token as a **`(lexical_component, lexeme)`** pair.
- **Prints the final Symbol Table**, now updated to include any **user-defined identifiers** discovered during analysis.
- **Frees all dynamically allocated memory** and **closes any open file streams** before exiting cleanly.

## Example Output

```text
IMPRESIÓN TÁBOA DE SÍMBOLOS
/**************************************/
(306, chan)
(309, for)
(304, func)
(317, go)
(302, import)
(300, package)
(311, range)
(307, var)
/**************************************/


IMPRESIÓN (compoñente léxica, lexema)
/**************************************/
(300, package)
(301, main)
(59, (null))
...
ERRO: lexema excede o tamaño máximo permitido na liña 10
(43, +)
(125, })
/**************************************/


IMPRESIÓN TÁBOA DE SÍMBOLOS
/**************************************/
(301, Println)
(301, c)
(306, chan)
...
(301, sum)
(301, v)
(307, var)
/**************************************/
```

## Limitations

- **Only lexical analysis is implemented**: The syntactic analyzer module is a stub that simply invokes the lexical analyzer until the end of input and prints the given tokens. 
- **Subset of Go supported**: The analyzer only handles a representative subset of Go syntax and tokens, sufficient for processing the example file.
- **No semantic analysis or code generation**: The project focuses exclusively on lexical structure.
- **Simplified lexical rules**: Certain aspects like multi-line comments are handled in a simplified manner.

## Author

- David Díaz López  

**For compilation instructions and basic usage**, please refer to the `README.txt` file included in the repository (written in Galician).

