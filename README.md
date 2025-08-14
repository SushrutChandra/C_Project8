# Automated Source Comment Audit

Automated Source Comment Audit is a C program that determines whether a C source file contains "enough comments" by comparing the total number of characters in the file to the number of characters remaining after all comments have been removed. This tool demonstrates the use of **process control, child processes, and pipes** in a UNIX environment.

---

## Features

- Runs two separate programs in child processes:
  1. `remove-comments.x` – counts characters excluding comments.
  2. `wc -m` – counts total characters.
- Captures output from child processes using pipes.
- Compares character counts to determine if the file has sufficient comments.
- Prints either:
  - `Enough comments!`  
  - `Too few comments!`
- Uses `safe_fork()` instead of `fork()` for safer process creation.

---

## Usage

Compile the program:

```bash
gcc check-comments.c safe-fork.o -o AutomatedSourceCommentAudit.x
Run it with a C source file as an argument:

bash
Copy
Edit
./AutomatedSourceCommentAudit.x <sourcefile.c>
Example:

bash
Copy
Edit
./AutomatedSourceCommentAudit.x test1program.c
# Output: Too few comments!

./AutomatedSourceCommentAudit.x test2program.c
# Output: Enough comments!
Exit codes:

0 – Enough comments

1 – Too few comments

2 – No filename provided

Implementation Details
Child Process 1:

Executes ./remove-comments.x <sourcefile> using execlp.

Redirects stdout to a pipe so the parent can read the output.

Child Process 2:

Executes wc -m <sourcefile> using execlp.

Also writes output to the pipe.

Parent Process:

Reads both outputs from the pipe using scanf.

Compares the counts and prints the appropriate message.

Note: Uses safe_fork() to prevent fork bombs and standard C I/O (printf/scanf) for simplicity.

Tests
Two example tests are included:

test1program.c – too few comments

test2program.c – enough comments

Run the included test scripts:

bash
Copy
Edit
./test1
./test2
Expected output:

mathematica
Copy
Edit
Success!
Dependencies
remove-comments.x (compiled executable for stripping comments)

safe-fork.o and safe-fork.h for safe process creation

Standard C libraries (stdio.h, unistd.h, sys/types.h, err.h, sysexits.h)
