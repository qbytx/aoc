#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COUNT_COLORS 3
#define LONGEST_COLOR_NAME 5

const int LIMIT_R = 12;
const int LIMIT_G = 13;
const int LIMIT_B = 14;

const char * R = "red";
const char * G = "green";
const char * B = "blue";

const char SYM_R = 'r';
const char SYM_G = 'g';
const char SYM_B = 'b';

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
  
  char * fileName = "./input.txt";
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

      int game_success = 1;
      int game_exit = 0;

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

                switch (symbols[i]) {
                case SYM_R:
                  if (value > LIMIT_R) {
                    game_success = 0;
                    game_exit = 1;
                  }
                  break;
                case SYM_G:
                  if (value > LIMIT_G) {
                    game_success = 0;
                    game_exit = 1;
                  }
                  break;
                case SYM_B:
                  if (value > LIMIT_B) {
                    game_success = 0;
                    game_exit = 1;
                  }
                  break;
                default:
                  break;
                }

              }
            }
          }
          val = strtok(NULL, ",");

          if (game_exit) {
            break;
          }
        }

        if (game_exit > 0) {
          break;
        }
      }

      if (game_success > 0) {
        result += (long) game_number;
        printf("Game (%d) Success: %ld \n", game_number, result);
      } else {
        printf("Game (%d) Failure \n", game_number);
      }
    }
  }

  fclose(fp);
  return result;
}