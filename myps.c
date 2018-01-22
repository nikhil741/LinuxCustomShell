#include <stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <asm/errno.h>
#include <errno.h>
#include <ctype.h>


//Check string contains only numerical value or not 
int numbers_only(const char *s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }

    return 1;
}

int main(){
	uid_t uid;
	char *username;
	uid_t userid= getuid();
	username=getlogin();
	printf("UserId:%d Username:%s\n", uid, username);

	char* path = "/proc/";
	
	DIR* dir;
	struct dirent *ent;

    //Readng /proc directory
	if ((dir = opendir (path)) != NULL) {
		//printf("Inside Directory:%s\n", path);
		printf("NAME STATE PID PPID UID\n");
        while ((ent = readdir (dir)) != NULL){
			//printf("%s\n", ent->d_name);
			if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)){
                continue;
            }

            //Checking content folder or not
            //if folder then again check for folder_name is numeric only or not
            if(ent->d_type==DT_DIR && numbers_only(ent->d_name)){
             	//printf("Inside%s\n", ent->d_name);
             	char status_path[100];
             	strcpy(status_path, path);
             	strcat(status_path, ent->d_name);
             	strcat(status_path, "/status");

             	//printf("Status Path:%s\n", status_path);

             	//Read FIle
             	FILE * fp;
    			char * line = NULL;
    			size_t len = 0;
    			ssize_t read;

    			fp = fopen(status_path, "r");
    			if (fp == NULL)
         			exit(EXIT_FAILURE);
         		int count=0;

                //read file till
                //uid i.e at 9th line
    			while ((read = getline(&line, &len, fp)) != -1) {
    				count++;
    				if(count==9){
    					//printf("%s", line);
    					int i=5, k=0;
    					char num_array[10];
    					while(line[i]!='\t'){
    						num_array[k++] = line[i++];
    					}
    					num_array[k]='\0';
    					int uid_number = atoi(num_array);
    					
                        //Check wheather uid matched with current user
                        if(uid_number == uid){
    						//printf("Uid:%s Process:%s\n", num_array, ent->d_name);
    						
                            //reset file pointer to start
                            fseek(fp, 0, SEEK_SET);
    						//rewind(fp);
    						count=0;
    						

                            //get 1 3 6 7 9 line values
                            while ((read = getline(&line, &len, fp)) != -1) {
    							count++;
    							char attributes[5][10];
    							switch(count){
    								case 1:
    									i=0;
    									k=0;
    									while(line[i]!='\t'){
    											i++;
    										}
    										i++;
    									while(line[i]!='\n'){
    										attributes[0][k++]=line[i++];
    									}
    									attributes[0][k]='\0';
    									printf("%15s", attributes[0]);
    									break;
    								case 3:
    									i=0;
    									k=0;
    									while(line[i]!='\t'){
    											i++;
    										}
    										i++;
    									while(line[i]!='\n'){
    										attributes[1][k++]=line[i++];
    									}
    									attributes[1][k]='\0';
    									printf("%10s", attributes[1]);
    									break;
    								case 6:
    									i=0;
    									k=0;
    									while(line[i]!='\t'){
    											i++;
    										}
    										i++;
    									while(line[i]!='\n'){
    										attributes[2][k++]=line[i++];
    									}
    									attributes[2][k]='\0';
    									printf("%10s\t", attributes[2]);
    									break;
    								case 7:
    									i=0;
    									k=0;
    									while(line[i]!='\t'){
    											i++;
    										}
    										i++;
    									while(line[i]!='\n' && line[i]!='\t' && line[i]!=' '){
    										attributes[3][k++]=line[i++];
    									}
    									attributes[3][k]='\0';
    									printf("%10s", attributes[3]);
    									break;
    								case 9:
    									i=0;
    									k=0;
    									while(line[i]!='\t'){
    											i++;
    										}
    										i++;
    									while(line[i]!='\n'){
    										attributes[4][k++]=line[i++];
    									}
    									attributes[4][k]='\0';
    									printf("%10d\n", uid);
    									break;
    							}
    						}
    					}
    					break;
    				}
         			//printf("Retrieved line of length %zu :\n", read);
        			
    			}

    			fclose(fp);
    			if (line)
         			free(line);
    			 
            }
		}
        closedir(dir);
	}
    else{
        printf("Error Opening Directory\n");
    }
	return 0;
}