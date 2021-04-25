/**/
/* CT30A3370 Käyttöjärjestelmät ja systeemiohjelmointi
 * Otsikkotiedot: my-cat
 * Ohjelma avaa komentoriviparametrina annetun 
   tiedoston nimen, lukee tämän ja tulostaa sisällön 
   terminaaliin.
 * Tekijät: Nico Aalto & Shan Shan Yang
*/

#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

    char buff [200];
    for (int i = 1; i<argc; i++) {
        FILE *file = fopen(argv[i], "r"); 
        if (file == NULL) {
            perror("Cannot open file\n"); // Error handling
               exit(1);
           }
           while(fgets(buff, 200, file)) { // Reading from file
            printf("%s", buff); // Printing to screen
        }
           fclose(file); // closing file
    }
    return(0);
}
/**/
/* eof */