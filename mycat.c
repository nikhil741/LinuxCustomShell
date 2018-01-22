//
// Created by nikhil741 on 25/10/17.
//

/////////////////////////////////////////CAT COMMAND//////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int args, char** argv){
 
    int i;
    char * line = NULL;
    size_t len = 100;
    


    //Take input from stdin
    if(args==1){
        while(getline(&line, &len, stdin)!= -1){
        
        if((strncmp(line, "exit", 4)==0)||(strncmp(line, "quit", 4)==0)) {
            exit(0);
        }
        //strip \n of buffer i,e stdin
        line[strlen(line)-1]='\0';
        //print to stdout i.e default
        printf("%s\n", line);
        }
    }

    //INput from File
    else {
        for (i = 1; i < args; i++) {
            FILE *fp;
            ssize_t read;

            //Open a file
            //Description i.e path of file in agrv[index]
            fp = fopen(argv[i], "r");
            if (fp == NULL)
                exit(EXIT_FAILURE);

            //Read File
            while ((read = getline(&line, &len, fp)) != -1) {
                //printf("Retrieved line of length %zu :\n", read);
                printf("%s", line);
            }

            fclose(fp);                           //close file pointer
            if (line)
                free(line);                       //free line memory if present
        }
    }

    exit(EXIT_SUCCESS);                           //return success flag

    return 0;
}
