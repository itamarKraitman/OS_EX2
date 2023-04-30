This project is brought to you by Yulia Vanish ID 324385509 and Itamar Kraitman ID 208925578.
This project includes seven C files, divided into three parts, and a Makefile.

Part A: This part includes the implementations of two tools, which can be executed by CMD commands.

  * "cmp": This tool, compares two files, and returns 0 in case they are equal, and 1 otherwise. In Addition, the user can pass two flags, -v (for verbose), which tells the tool to print "equal" or "distinct" and addition to returning value, and -i (for ignore) which tells the tool to ignore Upper and Lower cases, meaning "AAA" and "aaa" are considered as equal.
    How to run: ./cmp <file1> <file2> <unmandatory flag>
    Exmaple: ./cmp text1.txt text2.txt -v

  * "copy": This tool copies a file to another, and returns 0 on success and 1 on failure. the tool creates a new file if does not exist, but does not overwrite a file that already exists. In addition, the user can pass two flags, -v (for verbose), which tells the tool to print "success",  "target file exists", or "general failure", and -f (for force) which tells the tool to overwrite an existing file.
    <br>How to run: ./copy [file1] [file2] [unmandatory flag]
    <br>Example: ./copy text1.txt text2.txt -f

  If one of the parameters or more is missing, the tool returns 1 and prints a usage explanation.

Part B: This part includes the implementation of two shared libraries, codecA, and codecB. 
  * codecA converts all lowercase chars to upper case, and all upper case to lower case. 
  * codecB converts all chars to 3-rd next char (Adding 3 to the asci value). according to TA- codecB refers to all chars in the ASCII table and not only to the ABC.
   - Each library supports two tools: encode for encryption and decode for decryption.
    <br>How to run: ./encode/decode [codec] [message]
    <br>Example: ./encode codecA "aaa" (returns "AAA").

Part C: This part includes the implementation of a shell program named stshell. 
   <br> This program is able to run CMD tools, stop running by pressing Ctrl+C, without killing the shell itself, redirect the output (with ">" and ">>" ), and piping (with "|") at least two pipes, and finally stop by the "Exit" command.
    <br>How to run: ./stshell -> [command]
    <br>Example: ./stshell
		          <br><stshell/> ls
		                    <br><stshell/> echo hello >> text1.txt
		                    <br><stshell/> echo hello | grep "o"
                   
Makefile: run "make all" to compile all executables, and "make clean" to clean all, you can either run "make <executable>" to run s specific executable.            

Linux environment: Ubuntu 22.04.2 LTS





