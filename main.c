
// Tarea EDD
// Juan De la Barra
// Lucas Vigorena

//
//      enunciado

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int dim;
  float *data;
} vector;

typedef struct {
  int row_dim;
  int col_dim;
  float **data;
} matrix;


//
//      funciones requeridas en la tarea

// 1
vector create_vector_from_file(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("No se pudo abrir el archivo %s\n", filename);
    exit(1);
  }
  vector v;
  v.dim = 0;
  v.data = NULL;
  float f;
  while (fscanf(fp, "%f", &f) == 1) {
    v.dim++;
    v.data = realloc(v.data, v.dim * sizeof(float));
    v.data[v.dim - 1] = f;
  }
  fclose(fp);
  return v;
}

// 2
matrix create_matrix_from_file(char *filename) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("No se pudo abrir el archivo %s\n", filename);
    exit(1);
  }
  matrix A;
  A.row_dim = 0;
  A.col_dim = 0;
  A.data = NULL;
  char line[1000];
  while (fgets(line, sizeof(line), fp)) {
    A.row_dim++;
    A.data = realloc(A.data, A.row_dim * sizeof(float *));
    A.col_dim = 0;
    char *tok = strtok(line, " ");
    A.data[A.row_dim - 1] = malloc(sizeof(float));
    while (tok != NULL) {
      A.col_dim++;
      A.data[A.row_dim - 1] =
          realloc(A.data[A.row_dim - 1], A.col_dim * sizeof(float));
      sscanf(tok, "%f", &(A.data[A.row_dim - 1][A.col_dim - 1]));
      tok = strtok(NULL, " ");
    }
  }
  fclose(fp);
  return A;
}

// 3
void print_vector(vector v) {
  for (int i = 0; i < v.dim; i++) {
    printf("%f ", v.data[i]);
  }
  printf("\n");
}

// 4
void print_matrix(matrix A) {
  for (int i = 0; i < A.row_dim; i++) {
    for (int j = 0; j < A.col_dim; j++) {
      printf("%f ", A.data[i][j]);
    }
    printf("\n");
  }
}

// 5
void destroy_vector(vector v) { free(v.data); }

// 6
void destroy_matrix(matrix A) {
  for (int i = 0; i < A.row_dim; i++) {
    free(A.data[i]);
  }
  free(A.data);
}

// 7
matrix transpose_matrix(matrix A) {
  matrix B = {A.col_dim, A.row_dim, malloc(A.col_dim * sizeof(float *))};
  for (int i = 0; i < B.row_dim; i++) {
    B.data[i] = malloc(B.col_dim * sizeof(float));
    for (int j = 0; j < B.col_dim; j++) {
      B.data[i][j] = A.data[j][i];
    }
  }
  return B;
}

// 8
matrix sum_matrix_matrix(matrix A, matrix B) {
  if (A.row_dim != B.row_dim || A.col_dim != B.col_dim) {
    printf("Error: matrices incompatibles\n");
    return A;
  }
  matrix C = {A.row_dim, A.col_dim, malloc(A.row_dim * sizeof(float *))};
  for (int i = 0; i < C.row_dim; i++) {
    C.data[i] = malloc(C.col_dim * sizeof(float));
    for (int j = 0; j < C.col_dim; j++) {
      C.data[i][j] = A.data[i][j] + B.data[i][j];
    }
  }
  return C;
}

// 9
matrix mult_matrix_matrix(matrix A, matrix B) {
  if (A.col_dim != B.row_dim) {
    printf("Error: matrices incompatibles\n");
    return A;
  }
  matrix C = {A.row_dim, B.col_dim, malloc(A.row_dim * sizeof(float *))};
  for (int i = 0; i < C.row_dim; i++) {
    C.data[i] = malloc(C.col_dim * sizeof(float));
    for (int j = 0; j < C.col_dim; j++) {
      float sum = 0;
      for (int k = 0; k < A.col_dim; k++) {
        sum += A.data[i][k] * B.data[k][j];
      }
      C.data[i][j] = sum;
    }
  }
  return C;
}

// 10
vector *mult_matrix_vector(matrix A, vector v) {
  if (A.col_dim != v.dim) {
    printf("Error: matriz y vector incompatibles\n");
    return &v;
  }
  vector *w = malloc(sizeof(vector));
  w->dim = A.row_dim;
  w->data = malloc(w->dim * sizeof(float));
  for (int i = 0; i < w->dim; i++) {
    float sum = 0;
    for (int j = 0; j < A.col_dim; j++) {
      sum += A.data[i][j] * v.data[j];
    }
    w->data[i] = sum;
  }
  return w;
}


//
//      funciones para probar las funciones que se piden en la tarea

int probarFuncionesVectores() {

  //Crea, imprime y destruye VECTOR
  printf("\n    1. 2. 3. Crea, imprime y destruye el vector v:\n");
  vector v = create_vector_from_file("v1.txt");
  print_vector(v);
  printf("  Dimensiones: %d items\n", v.dim);
  destroy_vector(v);
  printf("\n");

  return 0;
}

int probarFuncionesMatrix() {
  
  //Crea, imprime y destruye MATRIZ
  printf("\n    4. 5. 6. Crea, imprime y destruye la matriz A:\n");
  matrix A = create_matrix_from_file("m1.txt");
  print_matrix(A);
  printf("  Dimensiones: %d filas, %d columnas\n", A.row_dim, A.col_dim);
  destroy_matrix(A);
  printf("\n");

  return 0;
}

int probarFuncionesOperaciones() {

  // transpuesta
  printf("\n    7. Transpuesta:\n");
  printf("Matriz original A:\n");
  matrix A = create_matrix_from_file("m1.txt");
  print_matrix(A);
  printf("\n");
  printf("Matriz traspuesta At:\n");
  matrix At = transpose_matrix(A);
  print_matrix(At);
  printf("\n");

  // suma
  printf("\n    8. Suma matriz A por si misma:\n");
  matrix B = sum_matrix_matrix(A, A);
  print_matrix(B);
  printf("\n");

  // multiplicacion m por m
  printf("\n    9. Multiplicacion matriz A por su traspuesta:\n");
  matrix C = mult_matrix_matrix(A, At);
  print_matrix(C);
  printf("\n");

  // multiplicacion m por v
  printf("\n    10. Multiplicacion atriz A por vector 1:\n");
  vector *w = mult_matrix_vector(A, create_vector_from_file("v1.txt"));
  print_vector(*w);

  
  // end
  printf("\n");
  destroy_matrix(A);
  destroy_matrix(At);
  destroy_matrix(B);
  destroy_matrix(C);
  destroy_vector(*w);
  
  return 0;
}


//
//      funcion orquesatador

int Orq() {
  printf("\n\n    Tarea 1 EDD \n    Juan De la Barra\n    Lucas Vigorena\n    Sec.2\n\n");
  probarFuncionesVectores();
  probarFuncionesMatrix();
  probarFuncionesOperaciones();

  return 0;
}


//
//      funcion main

int main() {
  Orq();
  
  return 0;
}
