# A Minimal Command-Line Shell (msh)

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

Like other Unix shells, `msh` is designed with a basic command execution model that consists of `fork()`, `exec()`, and `wait()`. When a user enters an external command, the current shell creates a child process using `fork()` and executes the command using `execvp()`. If `execvp()` returns an error, the shell reports it using `perror()`, and the child process exits with status 127.

Meanwhile, the original (parent) shell process waits for the child process to end using `waitpid()`. Upon termination of the process, the parent shell determines the exit status of the process using macros (i.e., `WIFEXITED()`, `WEXITSTATUS()`, `WIFSIGNALED()`). Knowledge of the exit status of the child process is used to determine the next command prompt. In the event no child process exists, the shell displays a dollar sign prompt.

Some commands are handled by the shell rather than executed as separate processes. The command `cd` is a good example. If `cd` is executed as a separate process, each child process has its own working directory and running `cd` in a child process would change the working directory of only that child process. The parent shell would not be changed. That is the reason `cd` is implemented as a built-in command as `chdir()`. `exit` is also implemented as a built-in command to terminate the shell.

The shell also allows background processing by using `&` at the end of a command. Foreground commands cause the shell to wait for the process to finish. A child process that terminates is known as a zombie process. `msh` uses `waitpid(-1, &status, WNOHANG)` in its main loop to remove any zombie processes. The option `WNOHANG` allows the shell to check if any background processes have terminated without waiting for background processes that are still running.
---

## Known Limitations

Although `msh` supports the main features required by the assignment, it still has some limitations:

- **No pipe support (`|`):** Commands cannot yet be connected so that the output of one command becomes the input of another.
- **Limited redirection:** Only output redirection using `>` is supported. Input redirection (`<`) and appending output (`>>`) are not implemented.
- **Basic quote handling:** The shell separates arguments using spaces and tabs. As a result, quoted text containing spaces, such as `echo "hello world"`, is not treated as a single argument.
- **Limited job control:** Background processes can be started and their PIDs displayed, but commands such as `jobs`, `fg`, and `bg` are not available.

---

## Sources Declaration

The following resources were used as references during development:

- **Creating and Killing Child Processes in C** — Process lifecycle, fork, and signal management.
- **Mastering fork and exec in C** — Process creation and replacement mechanics.
- **Create a Zombie Process (Video)** — Demonstrating zombie process states and reaping techniques.
- **CISC 3350 Course Packet: exec() Family** — System call documentation for execvp and argument passing.
- **Systems Programming Book (Gustavo Rodriguez-Rivera)**  — Unix process architecture and shell design.
- **Practical System Programming with C (Sri Manikanta Palakollu, Apress 2021)** — Linux process handling and file descriptors.
- **Refresher in C Programming**  — String tokenization with strtok and pointer arrays.

**AI-tool declaration:** i used antigravity for conceptual clarification of POSIX system call signatures and macros, reviewing C syntax conventions, and formatting documentation/markdown tables as well as formatting the README.md .
