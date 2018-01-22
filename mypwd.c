//
// Created by nikhil741 on 23/10/17.
//

#include <printf.h>
#include <malloc.h>
//#include <zconf.h>

int main(int args, char** argv){
    
    char absolute_current_working_address[100];

    getcwd(absolute_current_working_address, sizeof(absolute_current_working_address));          //System Call
    printf("%s\n", absolute_current_working_address);
    
    return 0;
}