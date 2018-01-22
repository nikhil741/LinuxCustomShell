//
// Created by nikhil741 on 23/10/17.
//

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
#include <stdbool.h>

//Function
//Check whether path is file or directory
int is_regular_file(char *path)
{
    struct stat path_stat;
    stat(path, &path_stat);
    return S_ISREG(path_stat.st_mode);
}

//Function
//Check wheter folder is empty or not
int isDirectoryEmpty(char *dirname) {
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(dirname);
    if (dir == NULL) //Not a directory or doesn't exist
        return 1;
    while ((d = readdir(dir)) != NULL) {
        if(++n > 2)
            break;
    }
    closedir(dir);
    if (n <= 2) //Directory Empty
        return 1;
    else
        return 0;
}

int remove_directory_by_system_call(char* path, int flag_recursive_delete){
    //printf("%s\n", path);
    //printf("Flag Value: %d\n", flag_recursive_delete);
    //printf("FIle FLag Value: %d\n",is_regular_file(path));

    //FILE NORMAL
    if((is_regular_file(path)) == 1){
        if(remove(path) == 0){                                    //System Call
            //printf("Succesful Deleted FIle: %s\n", path);
        }else{
            printf("Failed Deletion FIle: %s\n", path);
        }
        return 0;
    }

    //DIRECTORY
    struct stat s;
    if(stat(path,&s) == 0 ) {
        if (s.st_mode & S_IFDIR) {
            //printf("File Path:%s\n", path);

            //Non-Empty Directory
            if(isDirectoryEmpty(path)==0){
                if(flag_recursive_delete == 1){
                    //printf("Recursive Deletion On\n");

                    DIR *dir;
                    struct dirent *ent;
                    if ((dir = opendir (path)) != NULL) {
                        /* print all the files and directories within directory */
                        while ((ent = readdir (dir)) != NULL) {
                            if((strcmp(ent->d_name, ".") == 0) || (strcmp(ent->d_name, "..") == 0)){
                                continue;
                            }
                            //printf ("%s\n", ent->d_name);
                            char path_of_file_or_directory[200];
                            strcpy(path_of_file_or_directory, path);
                            strcat(path_of_file_or_directory, "/");
                            strcat(path_of_file_or_directory, ent->d_name);
                            remove_directory_by_system_call(path_of_file_or_directory, flag_recursive_delete);
                            //remove_directory_by_system_call()
                        }
                        closedir (dir);
                        remove(path);       //System Call
                    } else {
                        /* could not open directory */
                        perror ("");
                        return EXIT_FAILURE;
                    }

                    //Get all content of current Directory
                    //get_all_content_of_directory(path);
                }
                else{
                    printf("Recursive Deletion OFf\n");
                }
            }

            //Empty Directory
            else if(isDirectoryEmpty(path)==1){
                if(remove(path)==0){                                          //System Call
                    //printf("Successfull Deletion Directory: %s\n", path);
                }else{
                    printf("Failed Deletion Directroy: %s\n", path);
                }
            }
        }
    }
    return 0;
}

int main(int args, char** argv){
    //printf("%d\n", args);
    //printf("%s\n", argv[1])
    int flag_recursive_delete = 0, i=1;
    if(strcmp(argv[1], "-r")==0) {
        i++;
        flag_recursive_delete=1;
    }
    for(i;i<args;i++)
        remove_directory_by_system_call(*(argv+i), flag_recursive_delete);
    return 0;
}

