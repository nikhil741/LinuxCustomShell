//
// Created by nikhil741 on 23/10/17.
//

/////////////////////////////////////////MY-SHELL////////////////////////////////////////////////
/////////////////////////////////////////MAIN PROGRAMME//////////////////////////////////////////

#include<unistd.h>
#include <curses.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <zconf.h>
#include <sys/wait.h>
#include <stdlib.h>

#define buff_size 100
size_t command_size = 100;
#define PROMT ">="


char* strip(char* s){
    char* temp;
    int i=0, k=0;
    while(s[i]!='\0' && s[i]!=' '){
        if(s[i]!='\n'||s[i]!='\t'){
            temp[k++]=s[i++];
        }
    }
    temp[k]='\0';
    return temp;
}


int main(){

    int i=0,j=0,k=0;
    char path_of_current_directory[200], command_path[200];    //Contain current address of all commands
    char* command_array_pointer[10];
    char* after_pipe_command_array_pointer[10];
    char command_array[10][100], after_pipe_commands[5][100];
    char* command =  (char*)malloc(buff_size*sizeof(char));
    int pipeFlag=0;

    //Get Current Directory where all commmands binary are present
    getcwd(path_of_current_directory, sizeof(path_of_current_directory));
    strcat(path_of_current_directory, "/");     //Concatenate / to get inside of directory

    while(1) {

    	//PROMT
        printf("%s", PROMT);
        
        gets(command);      //take input from user
   		
   		//Reinitialize Indexes
        i=0,j=0,k=0;
        
        //Reinitialize Pipe flag  
        pipeFlag=0;

        //LOOP till /0 or \n
        //Break if pipe command ie |(Pipe commmand)
        while(command[k]!='\0' && command[k]!='\n'){
            if(command[k]=='|'){                         //Found | break
                pipeFlag++;
                k++;
                break;
            }
            else if(command[k]==' '){
                command_array[i][j]='\0';
                k++;
                i++;
                j=0;
                continue;
            }
            else
                command_array[i][j]=command[k++];
            j++;
        }
        command_array[i][j]='\0';

        //Converting array to string array
        for(j=0;j<=i;j++){
            command_array_pointer[j]=command_array[j];
        }
        command_array_pointer[j]=NULL;


        //Will Execute Only if Pipe Flag is set
        //After pipe command
        //Storing Pipe Command
        //strcpy(after_pipe_commands[0], command_array[0]);
        i=0,j=0;
        while((pipeFlag==1) && (command[k]!='\0') && (command[k]!='\n')){
            if(command[k]==' '){
                after_pipe_commands[i][j]='\0';
                k++;
                i++;
                j=0;
                continue;
            }

            else
                after_pipe_commands[i][j] = command[k++];
            j++;
        }
        after_pipe_commands[i][j]='\0';
        

        //Converting Array of Character Array to Array of ponter of type char array
        for(j=0;j<=i;j++){
            after_pipe_command_array_pointer[j]=after_pipe_commands[j];
        }
        //Appending NULL pointer to last string
        after_pipe_command_array_pointer[j]=NULL;

        //Stripping Command to get command only
        command = strip(command_array[0]);
        
        

        //Checking for valid command only
        if(strcmp(command, "myls") == 0 || strcmp(command, "mypwd") == 0 || strcmp(command, "mycat") == 0 || strcmp(command, "mycd") == 0 || strcmp(command, "mymkdir")==0 || strcmp(command, "myps")==0 || strcmp(command, "myrm")==0 || strcmp(command, "mytail")==0 || strcmp(command, "mymv")==0){    //CHeck Wheather User Input mypwd commmand or not
            strcpy(command_path, path_of_current_directory);
            strcat(command_path, command_array[0]);
            
            //Two File Descriptor
            int p[2];
            pipe(p);                                       //System Call

            //Created Child                               
            int child_process_id = fork();				   //System Call

            //Child Process 
            //Child commands Execution
            if(child_process_id == 0){
                
            	//Check if pipe is present in command
                if(pipeFlag!=0){
	                //printf("Dupe\n");
	                dup2(p[1], 1);
	                close(p[0]);
                }
	            execv(command_path, command_array_pointer);     //Call to mypwd executable file
            }

            //Parent Process
            //Parent Process Command Execution
            if(child_process_id!=0){
            	if(pipeFlag!=0){                              //Check Pipe
            		close(p[1]);                              //pipe close file Descriptor
            	}
            	int status;
            	waitpid(child_process_id, &status, 0);
            	
            	//Special Case for mycd command
            	//Execute in main too if successfull in child
            	//Check Conditon and then execute
            	if(strcmp(command_array[0], "mycd")==0) {   
	            	if (WEXITSTATUS(status)==0) {
                		//printf("exited, status=%d\n", WEXITSTATUS(status));
                		chdir(command_array[1]);
                    }
	            }

            }                
            
            //Execute if pipe was present
            //Execution of 2nd Commmand after pipe
            if(pipeFlag==1){  
            	//Child Creation
                int childid = fork();                           //System Call
                
                //Child Process
                //Execution of Child Commands
                if(childid==0){
	                dup2(p[0], 0);
	      			close(p[1]);
	      			char * line = NULL;
    				size_t len = 100;
		            strcpy(command_path, path_of_current_directory);
		            strcat(command_path, "/");
		            strcat(command_path, after_pipe_commands[0]);
	                execv(command_path, after_pipe_command_array_pointer);
	            }

	            //Parent Process
	            if(childid!=0){
	            	wait(&childid);                //Wait Parent till child gets executed
 	            }
                
        	}
        }

        //Check for exit command
        else if(strncmp(command, "myexit", 6)==0){
            exit(0);
        }

        //Invalid Command
        //Promt user for Invalid Command
        else{
        	printf("Invalid Command\n");
        }
    }
}
