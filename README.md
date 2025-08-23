# C Programs Repository

This repository contains two C programs:

1.  A program to remove comments from a C file.
2.  A program to calculate the FIRST and FOLLOW sets for a given context-free grammar.

## 1. Remove Comments

The `removeComments.c` program removes single-line (`// ...`) and multi-line (`/* ... */`) comments from a C source file.

### Compilation

To compile the program, use a C compiler like `gcc`:

```bash
gcc removeComments.c -o removeComments
```

### Usage

To run the program, pass the path to the C file you want to process as a command-line argument:

```bash
./removeComments <filename>.c
```

**Warning:** This program modifies the input file in place. It's recommended to create a backup of your file before running this program.

An example input file `input.c` is provided.

## 2. FIRST and FOLLOW Sets Calculator

The `lab2` directory contains a program that calculates the FIRST and FOLLOW sets for a given context-free grammar. This is a common tool used in compiler design.

### Compilation and Execution

The `lab2` directory contains a `Makefile` to simplify compilation and execution. To build and run the program, navigate to the `lab2` directory and use the `make` command:

```bash
cd lab2
make com
```

This will compile `firstfollow.c` and run it with the example grammar file `prod.txt`.

### Input Grammar Format

The input grammar file should contain one production per line. The format for each production is `A->...`, where `A` is a non-terminal and `...` is a sequence of terminals and non-terminals. Epsilon is represented by `~`.

An example grammar is provided in `lab2/prod.txt`.
