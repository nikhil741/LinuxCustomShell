# LinuxCustomShell  
Like linux terminal it provides some basic functionality like create directory move change directory etc. Commnads are implemented using different system calls provided by operating system.  

##Compilation:
'gcc file.c -o targetName'  
Replace file.c with any of the files like myrm.c, mymv.c. myls.c, main.c etc.  
Replace targetName with name you want to run with for convenience I usually prefer the name to be same as .c fileName.  
i.e targetName can be myrm, mymv, mycd, mycat etc for it's file.c file respectively.  


## Commands:  
1.myrm  
2.mymv    
4.mycd  
5.mycat   
6.myls  
7.myps  
8.mymkdir   
9.myexit  

## Sample  
'myls path'  Replace path by target address.  
'myexit'  Terminate the Programme and will return control back to linux terminal.  
'myls | mycat' Use of pipe likewise in linux terminal.  
