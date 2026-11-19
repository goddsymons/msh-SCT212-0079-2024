## Mini Shell (msh) — Test Plan

This test plan is to verify the functionality of te mini shel:  ## Input parsing, External command execution, built-in commands, background execution, output redirectin and signal handling

when run

 ## Input:  
##Expected Behaivour:New prompt appears when run
The actual result: th msh> prompt appeared

 ## Input: echo hello world
Expected Behaivour: prints hello world, exit status 0
The actual result: hello world was displayed

 ## Input:  ls /nonexistent
Expected Behaivour:Displays an error from ls; non-zero exit status
The actual result: error message: cannot access '/nonexistent': No such file or directory

 ## Input:  nosuchcommand
Expected Behaivour:Displays an error message; exit status 127
The actual result:error message : nosuchcommand: No such file or directory

## ## Input:  cd /tmp then pwd
Expected Behaivour:Changes directory and prints /tmp
The actual result:Changed directory to /tmp, then printed /tmp

 ## Input: sleep 3 &
Expected Behaivour:Prompt returns immediately and displays the background PID
The actual result: background PID displayed ([bg] PID 19288)

 ## Input: echo hi > f.txt then cat f.txt  
Expected Behaivour:Creates f.txt containing hi
The actual result:([done] PID 19288) f.txt as created and hi displayed 

 ## Input:  sleep 100 then Ctrl-C
Expected Behaivour: Terminates sleep; shell survives; status 130
The actual result:Crtl+C terminated sleep

 ## Input:  Ctrl-D at the prompt
Expected Behaivour: Shell exits cleanly
The actual result: shell exited

 ## Input:  Multiple spaces: echo hello
Expected Behaivour: Prints hello correctly
The actual result:hello dislayed

 ## Input: echo second > f.txt then cat f.txt
Expected Behaivour: Overwrites the previous file contents with second
The actual result: second overwrote over what was there initially then was displayed

 ## Input:  help
Expected Behaivour:Displays the shell's built-in command information
The actual result: the built in command infromation was diplayed

## input: sleep 27
Expected Behaivour: Background process finishes and is eventually reaped without leaving a zombie
The actual result: Background process finished without leaving a zombie
                             

##  summary

The test cases above cover the main functionality of the mini shell. 
