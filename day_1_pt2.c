#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_COLORS 3
#define LONGEST_COLOR_NAME 5

const char * R = "red";
const char * G = "green";
const char * B = "blue";

const char SYM_R = 'r';
const char SYM_G = 'g';
const char SYM_B = 'b';

typedef struct {
  int r;
  int g;
  int b;
}
ColorData;

int getPower(ColorData * colorData) {
  int power = 0;
  if (colorData != NULL) {
    power = colorData -> r * colorData -> g * colorData -> b;
  }
  return power;
}

int main(void) {

  char * colors[COUNT_COLORS] = {
    (char * ) R,
    (char * ) G,
    (char * ) B
  };
  char symbols[COUNT_COLORS] = {
    SYM_R,
    SYM_G,
    SYM_B
  };
  char * fileName = "./cubes.txt";
  FILE * fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("Error: could not open file %s", fileName);
    return -1;
  }

  const unsigned MAX_LENGTH_BYTES = 256;
  char buffer[MAX_LENGTH_BYTES];
  long result = 0;

  while (fgets(buffer, MAX_LENGTH_BYTES, fp)) {

    int game_number;

    if (sscanf(buffer, "Game %d:", & game_number) == 1) {

      ColorData rgb;
      rgb.r = 0;
      rgb.g = 0;
      rgb.b = 0;

      char * start = strchr(buffer, ':');
      char * game = ++start;
      char * data;

      while ((data = strtok_r(game, ";", & game))) {

        char * val;
        char set[strlen(data)];
        strcpy(set, data);
        val = strtok(set, ",");

        while (val != NULL) {

          int value;
          char color[LONGEST_COLOR_NAME + 1];
          color[0] = '\0';

          if (sscanf(val, " %d %s", & value, color) == 2) {

            for (int i = 0; i < COUNT_COLORS; i += 1) {
              if (strcmp(colors[i], color) == 0) {
                char symbol = symbols[i];
                switch (symbol) {
                case SYM_R:
                  if (value > rgb.r) {
                    rgb.r = value;
                  }
                  break;
                case SYM_G:
                  if (value > rgb.g) {
                    rgb.g = value;
                  }
                  break;
                case SYM_B:
                  if (value > rgb.b) {
                    rgb.b = value;
                  }
                  break;
                default:
                  break;
                }
              }
            }
          }
          val = strtok(NULL, ",");
        }
      }
      result += (long) getPower( & rgb);
    }
  }
  printf("%ld \n", result);
  fclose(fp);
  return 0;
}