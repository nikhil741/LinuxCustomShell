//
// Created by nikhil741 on 23/10/17.
//
/////////////////////////////////////////MAKE DIRECTORY//////////////////////////////////////////
/////////////////////////////////////////MYMKDIR/////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>

int main(int args, char** argv){
    int i=1;

    //Looping till end not received
    for(;i<args;i++){
        if ((mkdir(*(argv+i), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)) == 0){                 //SYSTEM CALL
        }
        else{
        	printf("Failure Making Directory:%s\n", *(argv+i));
        }
    }
    return 0;
}
