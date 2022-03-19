#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

	if (argc != 3){
		fprintf(stderr,"error! wrong params: 1 - source, 2 - destination");
		return 1;
	}

	FILE* file1 = fopen(argv[1], "r");

	if (file1 == NULL) {
		fprintf(stderr,"error! couldn't open the file");

		return 1;
	}

	FILE* file2 = fopen(argv[2], "w");

	if (file2 == NULL){
		fprintf(stderr,"error! couldn't open the file");
		return 1;
	}


	char symb;
	while ((symb = fgetc(file1)) != EOF){
		fputc(symb, file2);
	}

	struct stat copyStat;
	stat(argv[1], &copyStat);

	chmod(argv[2], copyStat.st_mode);


	 if (fclose(file1) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
    
	 if (fclose(file2) != 0) {
        
        fprintf(stderr,"error! could not close the file\n");
    }
	//fclose(file2);
}
