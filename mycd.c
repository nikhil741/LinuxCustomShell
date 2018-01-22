//
// Created by nikhil741 on 23/10/17.
//

/////////////////////////////////////////CHANGE DIRECTORY//////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <asm/errno.h>
#include <errno.h>
int main(int args, char** argv){
    
    char path[100];
    strcpy(path, argv[1]);

    if(chdir(path)==0){                                  //Make System call and check it's execution status
        //printf("Success\n");
        return 0;
    } else{
        printf("Unsuccesfull: Path is Incorrect or somethning went wrong\n");
        return 1;
    }
}