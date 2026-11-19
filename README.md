# msh — A Minimal Command-Line Shell

**Course:** ICS 2305: Systems Programming  
**Author:** Nevins Symons  
**ID:** SCT212-0079/2024

---

## How to Build and Run

### Prerequisites

To build and run this project, you need a Linux environment, such as Ubuntu on WSL or a native Linux installation, with GCC and GNU Make installed.

### Compilation

If you are using the provided Makefile, compile the program by running:

```bash
make
```

Alternatively, you can compile it manually using GCC:

```bash
gcc -Wall -Wextra -std=c11 -o msh msh.c
```

### Running the Shell

Once compilation is complete, start the shell with:

```bash
./msh
```

To remove the compiled executable and other build files, run:

```bash
make clean
```

---

## Features Implemented

The following table summarises the main features implemented in `msh`:

| Task | Requirement | Status |
| :---: | :--- | :---: |
| **1** | Display a command prompt and read input of any length using `getline()`. Exit cleanly when Ctrl-D is pressed. | Complete |
| **2** | Split command input into arguments using `strtok()`, with spaces and tabs as delimiters. Blank lines are ignored. | Complete |
| **3** | Run external commands using `fork()`, `execvp()`, and `waitpid()`. Handle execution errors and return status 127 for unknown commands. | Complete |
| **4** | Support built-in commands: `cd [dir]`, `help`, and `exit`. The `cd` command uses `$HOME` when no directory is provided. | Complete |
| **5** | Display the previous command's exit status in the prompt (`msh[status]>`) and handle commands terminated by signals. | Complete |
| **6** | Run commands in the background using `&`, display their process IDs, and collect completed processes using `WNOHANG`. | Complete |
| **7** | Support output redirection using `>`, allowing output to be written to a file. | Complete |
| **8** | Handle Ctrl-C (`SIGINT`) so that a running foreground command can be interrupted without terminating the shell. | Complete |

---

## Design Explanation

The main part of `msh` follows the usual Unix process execution pattern: **fork, exec, and wait**. When a user enters an external command, the shell calls `fork()` to create a child process. The child then uses `execvp()` to run the requested program, replacing its own process image with the new program. If the command cannot be executed, `execvp()` returns an error. The shell reports this using `perror()` and the child exits with status 127.

While the child is running, the parent shell uses `waitpid()` to wait for it to finish. Once the process terminates, the shell checks how it ended using macros such as `WIFEXITED()`, `WEXITSTATUS()`, and `WIFSIGNALED()`. This allows `msh` to keep track of the command's exit status and display it in the next prompt.

Some commands need to be handled directly by the shell rather than run as separate processes. The `cd` command is a good example. Each process has its own current working directory, so running `cd` in a child process would only change that child's directory. Once the child exits, the parent shell would remain in its original directory. For this reason, `cd` is implemented as a built-in command using `chdir()`. The `exit` command is also handled internally so that it can terminate the shell itself.

The shell also supports background execution using `&`. Unlike foreground commands, background commands do not make the shell wait for them to finish. However, when a child process terminates, it can remain in the system as a zombie until its parent collects its exit status. To prevent this, `msh` uses `waitpid(-1, &status, WNOHANG)` at the beginning of each prompt loop. The `WNOHANG` option allows the shell to check for completed background processes without waiting for running ones, keeping the prompt available for new commands.

---

## Known Limitations

Although `msh` supports the main features required by the assignment, it still has some limitations:

- **No pipe support (`|`):** Commands cannot yet be connected so that the output of one command becomes the input of another.
- **Limited redirection:** Only output redirection using `>` is supported. Input redirection (`<`) and appending output (`>>`) are not implemented.
- **Basic quote handling:** The shell separates arguments using spaces and tabs. As a result, quoted text containing spaces, such as `echo "hello world"`, is not treated as a single argument.
- **Limited job control:** Background processes can be started and their PIDs displayed, but commands such as `jobs`, `fg`, and `bg` are not available.

---

## Sources & AI-Tool Declaration

The following resources were used as references during development:

- Linux Programmer's Manual and POSIX documentation for `fork(2)`, `execvp(3)`, `waitpid(2)`, `getline(3)`, `chdir(2)`, `dup2(2)`, and `signal(2)`.
- Course lecture notes and slides covering Unix processes, signals, and system calls.

**AI-tool declaration:** i used antigravity for conceptual clarification of POSIX system call signatures and macros, reviewing C syntax conventions, and formatting documentation/markdown tables as well as formatting the README.md .