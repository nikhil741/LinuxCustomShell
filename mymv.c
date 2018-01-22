//
// Created by nikhil741 on 23/10/17.
//

/////////////////////////////////////////MOVE DIRECTORY or FILE//////////////////////////////////////////
/////////////////////////////////////////  MYMV   //////////////////////////////////////////////////////
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

//Checks file or Directory
//FIle:1 return_value
//Directory:0 return_value
int is_regular_file(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}


int move_file(char* source_path, char* destination_path, int flag, int flag_parent){


    //Source Directory Name can also contain file_name
    char source_directory_name[100];
    int i=0, k=0;
    while((source_path[i]!='\0')&&(source_path[i]!='\n')){
        if(source_path[i]=='/'){
            k=0;
            source_directory_name[k++] = source_path[i];
            //printf("%c\n", source_dir_name[k-1]);
        }
        else{
            source_directory_name[k++] = source_path[i];
            //printf("%c\n", source_dir_name[k-1]);
        }
        i++;
    }
    source_directory_name[k]='\0';


    //printf("Indside Move_File SourcePath:%s\n", source_path);
    //printf("Inside Move_FIle Destination Path:%s\n", destination_path);
    //Regular File
    if(is_regular_file(source_path)==1){
        char new_destination_address[100];
        strcpy(new_destination_address, destination_path);
        strcat(new_destination_address, source_directory_name);
        rename(source_path, new_destination_address);
        return 0;
    }

    //Directory
    else{

        //Create Directory in Destination
        //Destinaton Path = DestinationPath + Directroy from source
        if(flag==0) {
            char new_destination_address[200];
            strcpy(new_destination_address, destination_path);
            strcat(new_destination_address, source_directory_name);
            
            if (mkdir(new_destination_address, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0) {
                flag++;
            }
            else
                printf("Folder Creation Error\n");
        }

        //Read Direcory Content of source
        DIR* dir;
        struct dirent *ent;
        if((dir=opendir(source_path))!=NULL){

            //Directory Opened

            //read directory one by one element
            while((ent=readdir(dir))!=NULL){
                if((strcmp(ent->d_name, ".")==0) || (strcmp(ent->d_name, "..")==0)){
                    continue;
                }

                //Content is Directory
                if(ent->d_type==DT_DIR){
                    char new_source_path[100];
                    char new_destination_path[100];

                    strcpy(new_source_path, source_path);
                    strcat(new_source_path, "/");
                    strcat(new_source_path, ent->d_name);

                    strcpy(new_destination_path, destination_path);
                    strcat(new_destination_path, source_directory_name);
                    strcat(new_destination_path, "/");
                    strcat(new_destination_path, ent->d_name);

                    //Create Folder
                    if(mkdir(new_destination_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)==0){}
                    else
                        printf("Folder Creation Error\n");

                    move_file(new_source_path, new_destination_path, flag, 1);
                }

                //Content is file
                else{
                    //Append Source Path
                    //SourcePath = SourcePath+/File Name
                    char new_source_path[100];
                    char new_destination_path[100];
                    strcpy(new_source_path, source_path);
                    strcat(new_source_path, "/");

                    strcat(new_source_path, ent->d_name);

                    strcpy(new_destination_path, destination_path);
                    if(flag_parent==0)
                        strcat(new_destination_path, source_directory_name);

                    strcat(new_destination_path, "/");
                    strcat(new_destination_path, ent->d_name);
                    printf("File Source Path:%s Destination Path:%s\n", new_source_path, new_destination_path);
                    if(rename(new_source_path, new_destination_path)==0){
                    
                    }
                    else{
                        printf("Failed\n");
                    }

                }
            }
            closedir(dir);
            remove(source_path);
        }
    }
    return 0;
}

//Removes dot if path is relative
//Remove . from 0th index
//returns new string without dot
char* remove_dot_from_path(char* path){
    int k=0;
    char* path_without_dot;
    path_without_dot = (char*)(malloc(sizeof(char)*200));
    for(int i=1;path[i]!='\0';i++){
        path_without_dot[k++]=path[i];
    }
    path_without_dot[k] = '\0';
    return path_without_dot;
}

int main(int args, char** argv){
    char absolute_current_working_address[200];
    char absolute_source_path[200];
    char absolute_target_path[200];
    char* path_without_dot;
    
      
    //Get Current Directory
    //Helful in case if path is relative
    getcwd(absolute_current_working_address, sizeof(absolute_current_working_address));


    //Relative Target path
    //Check if target path is relative
    if(argv[args-1][0]=='.'){
        strcpy(absolute_target_path, absolute_current_working_address);
        path_without_dot = remove_dot_from_path(*(argv+args-1));
        strcat(absolute_target_path, path_without_dot);
    }

    //Absolute Target Path
    else{
        strcpy(absolute_target_path, *(argv+args-1));
    }

    //Source Path
    //Loop before target path
    for(int i=1;i<args-1;i++){    
        
        //Relative Source Path
        //Conversion of Relative Path to Absolute Path of source
        //printf("Source path:%s", argv[i]);
        if(argv[i][0]=='.'){
            path_without_dot = remove_dot_from_path(*(argv+i));
            
            strcpy(absolute_source_path, absolute_current_working_address);
            strcat(absolute_source_path, path_without_dot);
            move_file(absolute_source_path, absolute_target_path, 0, 0);                                 ///Call Function to execute move
        }

        //Absolute Source Path
        else{
            move_file(*(argv+i), absolute_target_path, 0, 0);                                           ///Call Function to execute move
        }
        

    }

    return 0;
}
