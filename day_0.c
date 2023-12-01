#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char * fileName = "./input.txt";
  FILE * fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("Error: could not open file %s", fileName);
    return -1;
  }

  const unsigned MAX_LENGTH_BYTES = 256;
  char buffer[MAX_LENGTH_BYTES];

  const char * numbersString = "0123456789";
  const int numbersStringLen = strlen(numbersString);

  long result = 0;

  while (fgets(buffer, MAX_LENGTH_BYTES, fp)) {
    const int len0 = strlen(buffer);
    int found = 0;
    int value = 0;
    char * c = malloc(3);
    c[2] = '\0';
    for (int i = 0; i < len0; i += 1) {
      for (int j = 0; j < numbersStringLen; j += 1) {
        if (buffer[i] == numbersString[j]) {
          if (found++ < 1) {
            c[0] = (char) numbersString[j];
          } else {
            c[1] = (char) numbersString[j];
          }
        }
      }
    }
    c[1] = (found < 2) ? c[0] : c[1];
    printf("%s \n", c);
    result += atoi(c);
    free(c);
  }
  
  fclose(fp);
  printf("%ld", result);
  return result;
}