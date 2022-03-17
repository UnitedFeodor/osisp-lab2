#include <stdio.h>
#include <dirent.h>


int printDir(char const *name){
    DIR *currDir = opendir(name);
    if(!currDir){
        fprintf(stderr,"error! couldn't open the directory");
        return 1;
    }

    struct dirent *file;

    while((file = readdir(currDir))){
        printf("%s\n", file->d_name);
    }

    if(closedir(currDir)){
        fprintf(stderr,"error! couldn't close the directory");
        return 1;
    }

    return 0;
}

int main(int argc, char const *argv[]) {
    printf("ROOT DIRECTORY:\n");
    printDir("/");
    printf("\n\n");

    printf("CURRENT DIRECTORY:\n");
    printDir("./");
    
    
    return 0;

}

