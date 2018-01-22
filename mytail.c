//
// Created by nikhil741 on 25/10/17.
//


#include <stdio.h>
#include <stdlib.h>

int execute_tail(FILE* fp, int n){
    	
		char* line=NULL;
		size_t len = 0;
		ssize_t read;
		int total_number_of_lines=0;
    	
        //Error Reading File
    	if (fp == NULL)
        	exit(EXIT_FAILURE);

    	//read file and calculate number of lines
        while ((read = getline(&line, &len, fp)) != -1) {
        	
        	total_number_of_lines++;
    	}
    	
        //Reinintialize file pointer to starting of file
		fseek(fp, 0, SEEK_SET);
    	int start_index = total_number_of_lines-n + 1;
    	if(start_index<=0){
    		start_index=1;
    	}
    	total_number_of_lines=0;

        //read Line
    	while ((read = getline(&line, &len, fp)) != -1) {       	
        	total_number_of_lines++;
        	
            //Compare index and if satisfy condition print output
            if(start_index<=total_number_of_lines){
            	printf("%s", line);
        	}
    	}

        //Close File POinter
    	fclose(fp);

        //Free Memory
    	if (line)
        	free(line);

        //return EXIT_Success flag
    	return EXIT_SUCCESS;
    }

int main(int args, char** argv){
	FILE * fp;
    int index = 1;
    int n=5;
	char* line=NULL;
	ssize_t read;
	size_t len = 0;

    if(args != 1 && argv[1][0]=='-'){
    	index++;
        n = atoi(((*(argv+1))+1));
        
    }

    //Read from stdin
    //Create Temp file
    //Dump Stdin content to temporary file
    //Read number of lines in original Stdin
    //Read Temporary file and display last n lines
    if(args==1 || (args==2 && argv[1][0]=='-')){
    	fp=stdin;                                                 // file poninter = STDIN
    	FILE* f_write=fopen("./temp.txt", "w");
    	while ((read = getline(&line, &len, fp)) != -1) {         //Read Line By Line
        	fprintf(f_write, line);                               //Write TO file
    	}

        //Close fwrite Pointer
    	fclose(f_write);

        //Make File pointer with read provelages
    	FILE* f_read = fopen("./temp.txt", "r");

        //Call Function
    	execute_tail(f_read, n);
    	
        //Remove Temporary File
        if(remove("./temp.txt")==0){
    	}
    	else{
    		
    	}
    	exit(EXIT_SUCCESS);
    	return 0;
    }   

    //Read from file 
    else{
        //Read file path
        //Make Pointer with read privelages
        //Call custom function execute tail
    	fp = fopen(*(argv+index), "r");
    	execute_tail(fp, n);
    	exit(EXIT_SUCCESS);
    	return 0;
    }
   
    
}
