#include<stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc != 2){
		fprintf(stderr,"wrong arguments: 1 - file name\n");
		return 1;
	}

	char* fileName = argv[1];

	FILE* file = fopen(fileName, "w");

	if (file == NULL){
		fprintf(stderr,"error! couldn't open the file\n");
		return 1;
	}

	char symb;
	while ((symb = getc(stdin)) != 6){
		fputc(symb, file);
	}

	fclose(file);
}
