
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <stdlib.h>

#include <time.h>

struct FileStuff {
    struct dirent *info;
    char *directory;
};

struct LinkedList {
    struct FileStuff *file;
    struct LinkedList *next;
};

int findFilesRec(char *dirSource, char *dirSearch, FILE *file);
char* getFullPath(char* dirname, struct dirent *dir);
int filesAreEqual(const char *fn1, const char *fn2);
struct LinkedList *getSameFiles(char *directory, struct dirent *fileToFind, char *dirToSearch);
void add(struct LinkedList **head, struct FileStuff *data);


int main(int argc, char *argv[]) {
    if (argc != 4) {
        
        fprintf(stderr,"error! wrong parameters:\n1- first directory \n2 - second directory \n3 - result file\n");
        return 1;
    }

    int fDest = open(argv[3], O_WRONLY | O_CREAT, 0644);
    if (fDest == -1) {
        
        fprintf(stderr,"error! could not open the file\n");
        
        return 1;
    } 
    FILE *file= fdopen(fDest, "w");
    if (findFilesRec(argv[1], argv[2], file) != 0) {
        fprintf(stderr,"error! something went wrong while searching \n");    
    }
    
    if (fclose(file) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
        return 1;
    }

    return 0;
}

int findFilesRec(char *dirSource, char *dirSearch, FILE *file){


    struct stat *fileStat1 = (struct stat *)calloc(1, sizeof(struct stat));
    struct stat *fileStat2 = (struct stat *)calloc(1, sizeof(struct stat));

    DIR *dirStream = opendir(dirSource);
    if (dirStream == NULL) {
        
        fprintf(stderr,"error! could not open the directory\n");
        return 1;
    }

    
    struct dirent *curFile;
    
    while ((curFile = readdir(dirStream)) != NULL) {
        
        if  (curFile->d_type != DT_DIR) {
            struct LinkedList *equalFiles = getSameFiles(dirSource, curFile, dirSearch);

         
            if (equalFiles != NULL) {
                char *dirNewPath = getFullPath(dirSource, curFile);

                struct LinkedList *curEl = equalFiles;
                while (curEl != NULL) {
                    stat(dirNewPath, fileStat1);
                    stat(curEl->file->directory, fileStat2);

                    printf("\npath: %s - %s\n", dirNewPath, curEl->file->directory);
                    fprintf(file, "\npath: %s- %s\n", dirNewPath, curEl->file->directory);

                    printf("inode number: %ld - %ld\n", fileStat1->st_ino, fileStat2->st_ino);
                    fprintf(file, "inode number: %ld - %ld\n", fileStat1->st_ino, fileStat2->st_ino);

                    printf("file type and mode: %lo - %lo\n",  (unsigned long)fileStat1->st_mode, (unsigned long)fileStat2->st_mode);
                    fprintf(file, "file type and mode: %lo - %lo\n",  (unsigned long)fileStat1->st_mode, (unsigned long)fileStat2->st_mode);
                                      
                    printf("size: %lldb - %lldb\n", fileStat1->st_size, fileStat2->st_size);
                    fprintf(file, "size: %lldb - %lldb\n", fileStat1->st_size, fileStat2->st_size);

                    printf("create time: %s - %s", ctime(&fileStat1->st_ctim), ctime(&fileStat2->st_ctim));
                    fprintf(file, "create time: %s - %s", ctime(&fileStat1->st_ctim), ctime(&fileStat2->st_ctim));                    
                    
                    curEl = curEl->next;
                }
            }
        } else if ((curFile->d_type == DT_DIR) && (strcmp(curFile->d_name, ".") != 0) && (strcmp(curFile->d_name, "..") != 0)) {
            
            char *dirNewPath = getFullPath(dirSource, curFile);
            findFilesRec(dirNewPath, dirSearch, file);
        }

    }

    free(fileStat1);
    free(fileStat2);
    if(closedir(dirStream) == -1) {
        fprintf(stderr,"error! could not close the directory\n");
        return 1;
    }    
    return 0;


}
void add(struct LinkedList **head, struct FileStuff *data) {
    struct LinkedList *tmp = (struct LinkedList*) malloc(sizeof(struct LinkedList));
    tmp->file = data;
    tmp->next = (*head);
    (*head) = tmp;
}


char* getFullPath(char* dirname, struct dirent *dir) {
    char *filepath = calloc(strlen(dirname) + strlen(dir->d_name)+2, 1);
    strcpy(filepath, dirname);
    strcat(filepath,"/");
    strcat(filepath,dir->d_name);
    
    return filepath;
}

struct LinkedList *getSameFiles(char *directory, struct dirent *fileToFind, char *dirToSearch) {
    DIR *dirStream = opendir(dirToSearch);
    if (dirStream == NULL) {
        
        fprintf(stderr,"error! could not open the directory\n");
        return NULL;
    }

    struct dirent *curFile;
    struct LinkedList *resLinkedList = NULL;
    

    while ((curFile = readdir(dirStream)) != NULL) {

        if  ((curFile->d_type != DT_DIR) && (curFile->d_ino != fileToFind->d_ino))  {
            char *desiredFile = getFullPath(dirToSearch, curFile);
            char *mainFile = getFullPath(directory, fileToFind);
            

            if (filesAreEqual(mainFile, desiredFile)) {
                struct FileStuff *equalFile = malloc(sizeof(struct FileStuff));
                equalFile->info = curFile;
                equalFile->directory = desiredFile;

                add(&resLinkedList, equalFile);
            }

        } else if ((curFile->d_type == DT_DIR) && (strcmp(curFile->d_name, ".") != 0) && (strcmp(curFile->d_name, "..") != 0)) {
 
            char *dirNewPath = getFullPath(dirToSearch, curFile);
            struct LinkedList *findRes = getSameFiles(directory, fileToFind, dirNewPath);
            if (findRes != NULL) {
                struct LinkedList *curEl = findRes;
                while (curEl != NULL) {
                    add(&resLinkedList, curEl->file);
                    curEl = curEl->next;
                }
            }

        }

    }

    if(closedir(dirStream) == -1) {
        fprintf(stderr,"error! could not close the directory\n");
    }
    return resLinkedList;
}



int filesAreEqual(const char *fn1, const char *fn2) {
    int file1open = open(fn1, O_RDONLY);
    if (file1open < 0) {

        fprintf(stderr,"error! could not open the file\n");
        return NULL;
    }

    int file2open = open(fn2, O_RDONLY);
    if (file2open < 0) {

        fprintf(stderr,"error! could not open the file\n");
        return NULL;
    }

    FILE *file1 = fdopen(file1open, "r");
    FILE *file2 = fdopen(file2open, "r");

    int ch1, ch2;

    int res;
    while(1) {
        ch1 = fgetc(file1);
        ch2 = fgetc(file2);

        if(ch1 != ch2) {
            res = 0;
            break;
        } else if((ch1 == EOF) && (ch2 == EOF)) {
            res = 1;
            break;
        }

    }
    
    if (fclose(file1) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
        return NULL;
    }
    if (fclose(file2) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
        return NULL;
    }

    //fclose(file1);
    //fclose(file2);

    return res;
}
