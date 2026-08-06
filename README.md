*This project has been created as part of the 42 curriculum by rmandimb.*

# Get_next_line

**A C function that reads a line from a file descriptor, one line at a time.**

---

## Description

### Project Goal

The objective of **get_next_line** is to implement a function that reads and returns a single line from a file descriptor each time it is called. This function is fundamental for text file parsing in C and serves as an introduction to several advanced concepts in systems programming.

### Overview

`get_next_line()` reads from a file descriptor and returns the next line, including the terminating newline character (`\n`), unless the end of the file is reached without a newline. Successive calls to the function allow reading an entire file line by line, maintaining the reading state between calls using a static variable.

**Core Challenge**: The `read()` system call operates on fixed-size buffers (defined by `BUFFER_SIZE`), while lines in files have variable lengths. The function must efficiently handle this mismatch by:
- Reading data in chunks of `BUFFER_SIZE` bytes
- Accumulating these chunks until a complete line is formed
- Preserving leftover data for subsequent calls
- Managing memory carefully to prevent leaks

**Key Learning Outcomes**:
- Understanding static variables and their persistence
- Mastering dynamic memory allocation and deallocation
- Working with file descriptors and system calls
- Handling edge cases (empty files, missing newlines, EOF)
- Writing modular, norm-compliant code

---

## Instructions

### Main testing

```c
//main.c

#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc != 2)
	{
		printf("Usage: %s <filename>\n", argv[0]);
		return (1);
	}
	fd = open(argv[1], O_RDONLY);
	if (fd == -1)
	{
		printf("Error: Cannot open file\n");
		return (1);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("%s", line);
		free(line);
	}
	close(fd);
	return (0);
}
```

### Compilation

The function requires a compilation flag to define `BUFFER_SIZE`:
```bash
gcc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl_test

```

### Testing
```bash
# Memory leak check
valgrind --leak-check=full --show-leak-kinds=all ./a.out test.txt
```

### File Structure

Your project directory should contain:
```
get_next_line/
├── get_next_line.h          # Function prototypes and includes
├── get_next_line.c          # Main implementation (4 functions max)
├── get_next_line_utils.c    # Utility functions (5 functions max)
└── main.c                   # Your test file (not submitted)
```

---

## Resources

### Official Documentation

**Man Pages** (Essential reading):
```bash
man 2 read      # Read bytes from file descriptor
man 2 open      # Open file and get file descriptor
man 2 close     # Close file descriptor
```

**C Standard Library**:
- [File I/O in C](https://www.gnu.org/software/libc/manual/html_node/I_002fO-on-Streams.html) - GNU C Library documentation

### Conceptual Resources

**File Descriptors**:
- [File Descriptor Wikipedia](https://en.wikipedia.org/wiki/File_descriptor) - Comprehensive overview

**Static Variables in C**:
- [Static Variables - GeeksforGeeks](https://www.geeksforgeeks.org/static-variables-in-c/) - Clear explanation with examples

### Testing Resources

**Automated Testers**:
- [Francinette](https://github.com/xicodomingues/francinette) - Comprehensive 42 project tester

### Articles and Tutorials

- [Reading Files in C](https://www.codingunit.com/c-tutorial-file-io-using-text-files) - Practical file handling guide
- [Understanding Static in C](https://www.embedded.com/understanding-static-in-c/) - Deep dive into static keyword
- [Buffer Management Strategies](https://www.ibm.com/docs/en/aix/7.2?topic=management-understanding-buffer) - IBM documentation

---


## Algorithm Explanation

#### The Core Problem

read() gives us fixed chunks (BUFFER_SIZE), but we need to return variable-length lines (ending at \n). We must maintain reading position across function calls.
The Solution: Read-Accumulate-Extract Pattern

#### Key Insight

Use a static variable (remain) to store leftover data between function calls.
Four-Phase Algorithm
- Phase 1: Read and Accumulate
Read BUFFER_SIZE chunks from file
Append each chunk to 'remain'
Stop when '\n' is found OR reach EOF
Why: Build up enough data to form at least one complete line.
- Phase 2: Extract Line
Copy everything from 'remain' up to and including '\n'
(or entire 'remain' if no '\n')
Return this as the line
Why: Give the user exactly one line as requested.
- Phase 3: Update Remain
Remove the extracted line from 'remain'
Keep only what comes AFTER the '\n'
(or set to NULL if no '\n' - last line)
Why: Preserve unprocessed data for the next call.
- Phase 4: Orchestrate
Call phases 1-3 in sequence
Return the extracted line
'remain' persists via 'static' keyword
Why: Coordinate the process and maintain state.
Example Execution
File: "AB\nCD\nEF" with BUFFER_SIZE = 2
Call 1:
  remain: NULL
  Read: "AB" → "AB", "\nC" → "AB\nC" ('\n' found, stop)
  Extract: "AB\n"
  Update: remain = "C"
  Return: "AB\n"

Call 2:
  remain: "C" (saved from call 1)
  Read: "D\n" → "CD\n" ('\n' found, stop)
  Extract: "CD\n"
  Update: remain = ""
  Return: "CD\n"

Call 3:
  remain: ""
  Read: "EF" → "EF", EOF
  Extract: "EF" (no '\n', last line)
  Update: remain = NULL
  Return: "EF" 

Call 4:
  remain: NULL
  Return: NULL (done)
Why This Works
- **Efficient**: Minimizes system calls (reads in chunks)
- **Stateful**: static variable remembers position
- **Flexible**: Handles any line length via dynamic allocation
- **Correct**: Includes \n except on last line without one
- **Clean**: Separates concerns into logical functions
- **The magic**: The static keyword makes remain survive between function calls, acting as a memory of what we've read but not yet returned.
## AUTHOR
rmandimb
Student at 42 school.
