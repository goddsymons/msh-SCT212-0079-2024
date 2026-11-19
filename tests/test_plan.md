# Mini Shell (`msh`) — Test Plan
**Course:** ICS 2305: Systems Programming  
**Author:** Nevins Symons  
**Student ID:** SCT212-0079/2024  

This test plan verifies the core functionality of `msh`: input parsing, external command execution via `fork`/`execvp`/`waitpid`, built-in commands (`cd`, `help`, `exit`), background execution (`&`) with zombie reaping, output redirection (`>`), and signal handling (`SIGINT`).

## Test Cases

| # | Input | Expected Behaviour | Actual Result |
|---|---|---|---|
| 1 | *(blank line)* | New prompt, no error | `msh> ` prompt reappeared without error |
| 2 | `echo hello world` | Prints text; status 0 | `hello world` was displayed; exit status 0 |
| 3 | `ls /nonexistent` | Error from `ls`; non-zero status | `ls: cannot access '/nonexistent': No such file or directory`; non-zero exit status |
| 4 | `nosuchcommand` | Error message; status 127 | `nosuchcommand: No such file or directory`; status 127 |
| 5 | `cd /tmp` then `pwd` | Prints `/tmp` | Changed directory to `/tmp`, then printed `/tmp` |
| 6 | `sleep 3 &` | Prompt returns at once; PID shown | Background PID displayed (`[bg] PID 19288`) and prompt returned immediately |
| 7 | `echo hi > f.txt` then `cat f.txt` | File contains `hi` | File `f.txt` was created containing `hi`; `cat` displayed `hi` |
| 8 | `sleep 100` then Ctrl-C | Sleep stops; shell survives; status 130 | `sleep` terminated immediately; shell survived and displayed prompt; status 130 |
| 9 | Ctrl-D at the prompt | Shell exits cleanly | Shell exited cleanly without memory leaks |
| 10 | *(Custom)* Multiple spaces: `   echo    hello   ` | Ignores extra whitespace and prints argument | Arguments parsed correctly; printed `hello` |
| 11 | *(Custom)* `echo second > f.txt` then `cat f.txt` | Overwrites previous file contents (`O_TRUNC`) | Overwrote previous contents; `cat f.txt` printed `second` |
| 12 | *(Custom)* `help` | Displays shell built-in command documentation | Built-in command help information was displayed |
| 13 | *(Custom)* `sleep 2 &` then wait 3s | Background job reaped without creating zombie | Job completed; reaped via `WNOHANG` with `[done] PID ...` without leaving zombie |

## Summary
All 13 test cases (the 9 required tests plus 4 custom tests) executed successfully and matched expected behavior.
