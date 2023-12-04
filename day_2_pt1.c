#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DIGITS 3
#define INITIAL_CAPACITY 1
#define VECTOR_ARRAY_SIZE 8

typedef struct {
  int x;
  int y;
} Vector;

typedef struct {
  Vector *array;
} VectorArray;

typedef struct {
  char **array;
  size_t capacity;
  size_t size;
} ListString;

typedef struct {
  char *value;
} Number;

VectorArray createVectorArray(int x, int y) {
  VectorArray vectorArray;
  vectorArray.array = malloc(VECTOR_ARRAY_SIZE * sizeof(Vector));

  // top - left
  vectorArray.array[0].x = x - 1;
  vectorArray.array[0].y = y - 1;

  // top - center
  vectorArray.array[1].x = x;
  vectorArray.array[1].y = y - 1;

  // top - right
  vectorArray.array[2].x = x + 1;
  vectorArray.array[2].y = y - 1;

  // center - right
  vectorArray.array[3].x = x + 1;
  vectorArray.array[3].y = y;

  // center - left
  vectorArray.array[4].x = x - 1;
  vectorArray.array[4].y = y;

  // bottom - center
  vectorArray.array[5].x = x;
  vectorArray.array[5].y = y + 1;

  // bottom - right
  vectorArray.array[6].x = x + 1;
  vectorArray.array[6].y = y + 1;

  // bottom - left
  vectorArray.array[7].x = x - 1;
  vectorArray.array[7].y = y + 1;

  return vectorArray;
}

void freeVectorArray(VectorArray *vectorArray) { free(vectorArray->array); }

ListString createList() {
  ListString list;
  list.array = (char **)malloc(INITIAL_CAPACITY * sizeof(char *));
  list.capacity = INITIAL_CAPACITY;
  list.size = 0;
  return list;
}

void resizeList(ListString *list, size_t newCapacity) {
  list->array = (char **)realloc(list->array, newCapacity * sizeof(char *));
  list->capacity = newCapacity;

  if (list->array == NULL) {
    printf("Error @ resizeList");
    exit(EXIT_FAILURE);
  }
}

void addElement(ListString *list, const char *element) {

  if (list->size == list->capacity) {
    size_t newCapacity = (list->capacity == 0) ? 1 : list->capacity * 2;
    resizeList(list, newCapacity);
  }

  list->array[list->size] = strdup(element);
  if (list->array[list->size] == NULL) {
    printf("Error @ addElement");
    exit(EXIT_FAILURE);
  }

  list->size += 1;
}

void printList(const ListString *list) {
  for (size_t i = 0; i < list->size; i += 1) {
    printf("%s\n", list->array[i]);
  }
}

void freeList(ListString *list) {
  for (size_t i = 0; i < list->size; i += 1) {
    free(list->array[i]);
  }
  free(list->array);
}

int isNumber(char c) {
  if (c >= '0' && c <= '9') {
    return 1;
  } else {
    return 0;
  }
}

int isSymbol(char c) {
  if (c == '.' || isNumber(c) == 1) {
    return 0;
  } else {
    return 1;
  }
}

int isChecked(int **checked, int x, int y) { return checked[x][y]; }

void setChecked(int **checked, int x, int y) { checked[x][y] = 1; }

int search(Vector *vector, char **characters, int **checked) {
  char *str = characters[vector->y];
  int a, b, len;
  a = vector->x;
  b = a;
  len = strlen(str);

  while (a > 0 && isNumber(str[a - 1])) {
    a--;
  }

  while ((b < len - 1) && isNumber(str[b + 1])) {
    b++;
  }

  if (isChecked(checked, a, vector->y) == 1) {
    return 0;
  } else {
    setChecked(checked, a, vector->y);
  }

  int length = b - a + 1;

  char *substring = (char *)malloc((length + 1) * sizeof(char));

  if (substring == NULL) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  strncpy(substring, str + a, length);

  substring[length] = '\0';

  return atoi(substring);
}

void stripNewline(char *str) {
  size_t length = strcspn(str, "\n");
  str[length] = '\0';
}

int main(void) {
  char *fileName = "./symbols_small.txt";
  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("Error: could not open file %s", fileName);
    return -1;
  }

  const unsigned MAX_LENGTH_BYTES = 256;
  char buffer[MAX_LENGTH_BYTES];
  long result = 0;

  ListString list = createList();

  while (fgets(buffer, MAX_LENGTH_BYTES, fp)) {
    stripNewline(buffer);
    addElement(&list, buffer);
  }

  int rows = (int)list.size;
  int cols = (int)strlen(list.array[0]);

  char **characters = (char **)malloc(rows * sizeof(char *));
  int **checked = (int **)malloc(rows * sizeof(int *));

  // allocate array memory
  for (int i = 0; i < rows; i += 1) {
    characters[i] = (char *)malloc(cols * sizeof(char));
    checked[i] = (int *)malloc(cols * sizeof(int));
  }

  // initialize array
  for (int y = 0; y < rows; y += 1) {
    for (int x = 0; x < cols; x += 1) {
      characters[y][x] = list.array[y][x];
      checked[y][x] = 0;
    }
  }

  // process
  for (int y = 0; y < rows; y += 1) {
    for (int x = 0; x < cols; x += 1) {
      char c = characters[y][x];
      if (isSymbol(c)) {
        VectorArray va = createVectorArray(x, y);
        for (size_t j = 0; j < VECTOR_ARRAY_SIZE; j += 1) {
          Vector v = va.array[j];
          int vx, vy;
          vx = v.x;
          vy = v.y;
          if (vx >= 0 && vy >= 0 && vx < cols && vy < rows) {
            char s = characters[vy][vx];
            if (isNumber(s)) {
              int num = search(&v, characters, checked);
              result += (long)num;
            }
          }
        }
        freeVectorArray(&va);
      }
    }
  }

  for (size_t i = 0; i < rows; i += 1) {
    free(characters[i]);
    free(checked[i]);
  }
  free(characters);
  free(checked);
  freeList(&list);
  fclose(fp);
  printf("%ld \n", result);
  return result;
}