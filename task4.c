#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    if (argc != 3){
        fprintf(stderr,"error! wrong arguments: 1 - file name, 2 - N strings in group");
        return 1;
    }

    int lines = atoi(argv[2]);
    int  groupNumber = 0;

    FILE* file = fopen(argv[1], "r");
    char inp;
    int currLines = 1;

    while ((inp = fgetc(file)) != EOF){
        fputc(inp, stdout);

        if (inp == '\n'){
            currLines++;
            if ((lines != 0) && (currLines > lines)){
                getc(stdin);
                putc('\n', stdout);
                currLines = 1;
            }
        }
    }

    fclose(file);
    return 0;
}
