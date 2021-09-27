# Linux-Shell
ASSIGNMENT 2
Name       : Naman Anand
Roll no    : 200101070
SUBJECT    : CS242
ASSIGNMENT : 2


********************************************************************************************************************************************************
COMPILING AND FOR RUNNING COMMANDS :->
1)gcc -o 200101070 200101070_Assign2.c
2)./200101070


********************************************************************************************************************************************************
LIST OF OPERATIONS
1)setenv HOME 
  // NOTE THE STARTING COMMAND SHOULD BE SAME LIKE THIS OHERWISE IT WILL NOT WORK U HAVE TO PUT EXACT SAME SPACES
     You Will Be directed into seperate line FOR Storing
     You can use HOME,SHELL,TERM,USER replacing HOME
     U can Give Any Number if spaces after HOME it works
************************************************************
2)printenv HOME
  echo $HOME
 USE EXACTLY SAME COMMAND Without any Spaces For its Working
 You can use HOME,SHELL,TERM,USER replacing HOME
************************************************************
3) Implemented all Types of cd 
   1) cd ~ to go 
    You can Use cd ~ to go to HOME and U can Give Any Number of spaces after ~ it works .
    But not before cd or ~ Use same command till there
    cd works
   2) cd ..  // SAME LIKE OF LINUX WORKS
   3) cd..  // SAME LIKE OF WINDOWS WORKS
   4) You can go to any subdirectory using
      cd Subdirectory_name
   5) You can reach anypath by giving exact path
      cd /REALPATH
   6) cd
 just pressing cd WILL Lead You to Home
   ALL TYPES OF CD IMPLEMENTED
************************************************************
4) history
 U can use history command to print history
 Note Please use history without any spaces before 

************************************************************
5) If U WANT TO EXIT U CAN USE FOLLOWING WORDS 
 x
 quit
 exit
 It will make u exit
 Note Please use exit or x or quit without any spaces before
************************************************************
6) | PIPING WORKS 
  EX -> ls -lha | sort 
        ls -lha | wc , etc
    Please use this without any spaces before
    Only single level piping without any redirection
    (BEFORE SPACES TO COMMANDS WILL DAMAGE THE PROGRAM)
************************************************************
7)         SUPPORTS REDIRECTION:
           input(<), output(>), append(>>) Also <>
Input and Output redirection Works and Also Append
   We can use for various cases 
   Note Format 
        ls -l >output.txt
        ls -l >>output2.txt
       (if u use ls -l > output.txt) ->It creates and store in ' output.txt'
   Also for reading We can Use
        cat <input.txt
  ALso Both redirection simulataneously working 
  Please use this format 
  Ex- 
  cat <input.txt>output.txt // NO SPACE BETWEEN FILES

 ************************************************************
8) List of LINUX commands:

man, which, chsh, whereis, passwd, date, cal, clear, sleep, history, apropos, exit, logout, shutdown, ls, cat, more, less, touch, cp, mv, rm, script, find, mkdir, cd, pwd, rmdir, chmod, grep
Note In grep and echo Please Do Not Use " " Use Directly without " " 
like example
ls | grep m    etc
 ************************************************************
9) help command without any previous space
   Ex-help
 ************************************************************
Note It may not work for particular command or return error But Please try again once. It will work. Also please try to give exact commands for proper working in Shell
Reason ->
 There is A restriction on Size Of input and various commands. Overriding It may led to some error . But we can easily change in case of higher input . I thought that size of Input will work fine So I didn't Make more larger. Due to this it may not give output for any command Or it may led to segmentation fault.Please close and again run in that case . 

 Thank You !!
