/**/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-grep
 * Ohjelma etsii tiedostosta tiettyjä sanoja ja tulostaa ne rivit jossa on niitä sanoja
 * Tekijät: Nico Aalto & Shan Shan Yang
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {

	size_t len = 0;
	size_t read;
	char *findStr = argv[1];
  char *pStart = NULL;
  char fname[999];

	if (argc==1) {
		printf("grep: <search word> <filename(s)>\n"); // Too few arguments
		return(1);
	}
  if (argc==2) {
    fgets(fname,999,stdin);
    fname[strcspn(fname, "\n")] = 0;
    printf("%s",fname);
    FILE *file = fopen(fname, "r");
    if (file == NULL) {
	    	perror("my-grep: cannot open file\n");
	   		return(1);
	   	}
	   	while ((read = getline(&pStart, &len, file)) != -1) { // reading file
	        if (strstr(pStart, findStr) != NULL) {
	        	printf("%s", pStart);
	        }
    	}
    	if (ferror(file)) {
        	perror("Error with file"); // Error handling
    	}
    	free(pStart); // Freeing memory
	   	fclose(file);;
    return 0;
	}
  if (argc > 2){
    for (int i = 2; i<argc; i++) { // for each given file
      FILE *file = fopen(argv[i], "r");
      if (file == NULL) {
          perror("my-grep: cannot open file\n");
          return(1);
        }
        while ((read = getline(&pStart, &len, file)) != -1) {
            if (strstr(pStart, findStr) != NULL) {
              printf("%s", pStart);
            }
        }
        if (ferror(file)) {
            perror("Error with file");
        }
        free(pStart); // Freeing memory
        fclose(file); // Closing file
    }
  return(0);
  }
	
	
}



// Lähteenä käytetty gvithubista käyttäjän petterigit lähdekoodia