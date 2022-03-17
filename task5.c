#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc != 3){
		fprintf(stderr,"error! wrong params: 1 - source, 2 - destination");
		return 1;
	}

	FILE* file_1 = fopen(argv[1], "r");

	if (file_1 == NULL) {
		fprintf(stderr,"error! couldn't open the file");

		return 1;
	}

	FILE* file_2 = fopen(argv[2], "w");

	if (file_2 == NULL){
		fprintf(stderr,"error! couldn't open the file");
		return 1;
	}


	char symb;
	while ((symb = fgetc(file_1)) != EOF){
		fputc(symb, file_2);
	}

	struct stat copyStat;
	stat(argv[1], &copyStat);

	chmod(argv[2], copyStat.st_mode);

	fclose(file_1);
	fclose(file_2);
}
