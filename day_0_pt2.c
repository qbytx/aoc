#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int a;
  int b;
} Vector;

typedef struct {
  Vector *list;
  size_t size;
  size_t capacity;
} VectorArray;

VectorArray createVectorArray() {
  VectorArray array;
  array.list = malloc(sizeof(Vector *));
  array.size = 0;
  array.capacity = 0;
  return array;
}

void resizeArray(VectorArray *array, size_t newCapacity) {
  array->list = realloc(array->list, newCapacity * sizeof(Vector *));

  if (array->list == NULL) {
    exit(EXIT_FAILURE);
  }
  array->capacity = newCapacity;
}

void addVector(VectorArray *array, int a, int b) {
  if (array->size == array->capacity) {
    size_t newCapacity = (array->capacity == 0) ? 1 : array->capacity + 1;
    resizeArray(array, newCapacity);
  }

  Vector v;
  v.a = a;
  v.b = b;
  array->size += 1;
  array->list[array->size - 1] = v;
}

Vector getVector(const VectorArray *array, size_t index) {
  if (index < array->size) {
    return array->list[index];
  } else {
    exit(EXIT_FAILURE);
  }
}

int newVector(const VectorArray *array, int a, int b) {
  for (int i = 0; i < array->size; i += 1) {
    if (array->list[i].a == a && array->list[i].b == b) {
      return -1;
    }
  }
  return 1;
}

void freeArray(VectorArray *array) {
  free(array->list);
  array->list = NULL;
  array->size = 0;
  array->capacity = 0;
}

void swap(Vector *a, Vector *b) {
  Vector temp = *a;
  *a = *b;
  *b = temp;
}

void bubbleSort(VectorArray *array) {
  int n = array->size;
  if (n <= 1) {
    return;
  }
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (array->list[j].b > array->list[j + 1].b) {
        swap(&array->list[j], &array->list[j + 1]);
      }
    }
  }
}

int main(void) {
  const int values[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  const char *numbers[] = {"0",    "1",   "2",     "3",     "4",
                           "5",    "6",   "7",     "8",     "9",
                           "zero", "one", "two",   "three", "four",
                           "five", "six", "seven", "eight", "nine"};
  const int numbersLength = sizeof(numbers) / sizeof(numbers[0]);

  char *fileName = "./input.txt";
  FILE *fp = fopen(fileName, "r");

  if (fp == NULL) {
    printf("Error: could not open file %s", fileName);
    return -1;
  }

  const unsigned MAX_LENGTH_BYTES = 256;
  char buffer[MAX_LENGTH_BYTES];

  long result = 0;

  while (fgets(buffer, MAX_LENGTH_BYTES, fp)) {
    VectorArray vectorArray = createVectorArray();
    char *c = malloc(3);
    c[2] = '\0';

    for (int j = 0; j < numbersLength; j += 1) {
      char *src = buffer;
      char *sub = strstr(src, numbers[j]);
      while (sub != NULL) {
        int a = values[j > 9 ? j - 10 : j];
        int b = (sub - src);
        addVector(&vectorArray, a, b);
        sub = strstr(sub + 1, numbers[j]);
      }
    }

    bubbleSort(&vectorArray);

    Vector v0;
    Vector v1;

    if (vectorArray.size > 0) {
      v0 = getVector(&vectorArray, 0);
      if (vectorArray.size > 1) {
        v1 = getVector(&vectorArray, vectorArray.size - 1);
      } else {
        v1 = v0;
      }
    }

    c[0] = v0.a + '0';
    c[1] = v1.a + '0';

    result += atoi(c);

    freeArray(&vectorArray);
    free(c);
  }

  fclose(fp);
  printf("%ld", result);

  return result;
}