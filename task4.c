#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr,"error! wrong arguments: 1 - file name, 2 - N strings in group");
        return 1;
    }

    //int lines = atoi(argv[2]);
    char *test;
    errno = 0;
    int lines = strtol(argv[2],&test,10);
    
    if ((strlen(test) != 0) || (errno!=0)) {
	    fprintf(stderr,"error! not a number or over/underflow: error %d\n",errno);
	    return 1;

    }	
    if (test == argv[2]) {
        fprintf(stderr, "error! empty string\n");
        return 1;
    }

    if (lines < 0) {
        fprintf(stderr, "error! amount less than zero\n");
        return 1;    
    }


    FILE* file = fopen(argv[1], "r");
    if (file == NULL) {
		fprintf(stderr,"error! couldn't open the file");

		return 1;
	}
    char inp;
    int currLines = 1;

    while ((inp = fgetc(file)) != EOF){
        fputc(inp, stdout);

        if (inp == '\n'){
            currLines++;
            if ((lines != 0) && (currLines > lines)){
                getc(stdin);
                //putc('\n', stdout);
                currLines = 1;
            }
        }
    }
    if (fclose(file) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
        return 1;
    }

    return 0;
}
