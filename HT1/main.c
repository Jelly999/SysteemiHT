/**/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-cat
 * Ohjelma avaa komentoriviparametrina annetun 
   tiedoston nimen, lukee tämän ja tulostaa sisällön 
   terminaaliin.
 * Tekijät: Nico Aalto & Shan Shan Yang
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void reverse(int arguments, char fname[],char fnameOut[]){

	FILE *file; // Reading file
	if ((file = fopen(fname,"r")) == NULL) {
			fprintf(stderr,"error: cannot open file '%s'\n",fname);			
			exit(1);
		}

	int i,count=0;
	char *line = NULL;
	size_t buffer = 0;
	size_t size;
	char** pStart = NULL;
	size = getline(&line,&size,file);

	while(feof(file) == 0) {
		size = getline(&line,&buffer,file);

	}

	rewind(file); // Set pointer to beginning of the file

	if ((pStart = malloc((count+1)*sizeof(char))) == NULL){ // Reserving memory
		fprintf(stderr,"malloc failed"); // Error handling, malloc failing
		exit(1);
	}



	while(feof(file) == 0) {
		if ((pStart[count] = malloc((size +1)*sizeof(char))) == NULL){ // Reserving memory for each row
			fprintf(stderr,"malloc failed");
			exit(1);
		}
		strcpy(pStart[count],line);
		count++;
		size = getline(&line,&buffer,file);
	}

	fclose(file);
	if (arguments==1||arguments==2){ // Printing to screen
		printf("File content reverted: \n");
		for(i=count-1;i > 0;i--){
			printf("%s",pStart[i]);
		}
	}else{
		FILE *file2; // Writing output file
		if ((file2 = fopen(fnameOut,"w")) == NULL) {
			fprintf(stderr,"error: cannot open file '%s'\n",fname);			
			exit(1);
		}
		for(i=count-1;i > 0;i--){
			fprintf(file2,"%s",pStart[i]); 
		}
		printf("File %s content reversed and written to file %s\n",fname,fnameOut); 
	}
	free(pStart); // Freeing memory
  exit(0);
}

int main(int argc, char *argv[]) {
	char fname[1000];
	if(argc > 4){
		printf("Too many arguments, usage: reverse <input> <output>\n");
		exit(0);
	}
	if(argc == 1){
		printf("Input filename: ");
		scanf("%s",fname);
		reverse(argc,fname,NULL);
	}
  // depending number of arguments, selecting correct parameters
  if (argc == 2) {reverse(argc,argv[1],NULL);}
	if(argc == 3) {reverse(argc,argv[1],argv[2]);}
	if(argc == 4) {
		if (strcmp(argv[1],argv[2])==0){
			printf("Filenames must be different\n");
			exit(1);
		}
		reverse(argc,argv[1],argv[2]);
	}
	return (0);
}



// Lähteenä käytetty gvithubista käyttäjän petterigit lähdekoodia