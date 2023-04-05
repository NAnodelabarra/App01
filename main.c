
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
  matrix M;
  M.row_dim = 0;
  M.col_dim = 0;
  M.data = NULL;
  char MAX_LINE_SIZE[1000];
  while (fgets(MAX_LINE_SIZE, sizeof(MAX_LINE_SIZE), fp)) {
    M.row_dim++;
    M.data = realloc(M.data, M.row_dim * sizeof(float *));
    M.col_dim = 0;
    char *tok = strtok(MAX_LINE_SIZE, " ");
    M.data[M.row_dim - 1] = malloc(sizeof(float));
    while (tok != NULL) {
      M.col_dim++;
      M.data[M.row_dim - 1] =
          realloc(M.data[M.row_dim - 1], M.col_dim * sizeof(float));
      sscanf(tok, "%f", &(M.data[M.row_dim - 1][M.col_dim - 1]));
      tok = strtok(NULL, " ");
    }
  }
  fclose(fp);
  return M;
}

// 3
void print_vector(vector v) {
  for (int i = 0; i < v.dim; i++) {
    printf("%f ", v.data[i]);
  }
  printf("\n");
}

// 4
void print_matrix(matrix M) {
  for (int i = 0; i < M.row_dim; i++) {
    for (int j = 0; j < M.col_dim; j++) {
      printf("%f ", M.data[i][j]);
    }
    printf("\n");
  }
}

// 5
void destroy_vector(vector v) { free(v.data); }

// 6
void destroy_matrix(matrix M) {
  for (int i = 0; i < M.row_dim; i++) {
    free(M.data[i]);
  }
  free(M.data);
}

// 7
matrix transpose_matrix(matrix M) {
  matrix N = {M.col_dim, M.row_dim, malloc(M.col_dim * sizeof(float *))};
  for (int i = 0; i < N.row_dim; i++) {
    N.data[i] = malloc(N.col_dim * sizeof(float));
    for (int j = 0; j < N.col_dim; j++) {
      N.data[i][j] = M.data[j][i];
    }
  }
  return N;
}

// 8
matrix sum_matrix_matrix(matrix M, matrix N) {
  if (M.row_dim != N.row_dim || M.col_dim != N.col_dim) {
    printf("Error: matrices incompatibles\n");
    return M;
  }
  matrix O = {M.row_dim, M.col_dim, malloc(M.row_dim * sizeof(float *))};
  for (int i = 0; i < O.row_dim; i++) {
    O.data[i] = malloc(O.col_dim * sizeof(float));
    for (int j = 0; j < O.col_dim; j++) {
      O.data[i][j] = M.data[i][j] + N.data[i][j];
    }
  }
  return O;
}

// 9
matrix mult_matrix_matrix(matrix M, matrix N) {
  if (M.col_dim != N.row_dim) {
    printf("Error: matrices incompatibles\n");
    return M;
  }
  matrix O = {M.row_dim, N.col_dim, malloc(M.row_dim * sizeof(float *))};
  for (int i = 0; i < O.row_dim; i++) {
    O.data[i] = malloc(O.col_dim * sizeof(float));
    for (int j = 0; j < O.col_dim; j++) {
      float sum = 0;
      for (int k = 0; k < M.col_dim; k++) {
        sum += M.data[i][k] * N.data[k][j];
      }
      O.data[i][j] = sum;
    }
  }
  return O;
}

// 10
vector *mult_matrix_vector(matrix M, vector v) {
  if (M.col_dim != v.dim) {
    printf("Error: matriz y vector incompatibles\n");
    return &v;
  }
  vector *w = malloc(sizeof(vector));
  w->dim = M.row_dim;
  w->data = malloc(w->dim * sizeof(float));
  for (int i = 0; i < w->dim; i++) {
    float sum = 0;
    for (int j = 0; j < M.col_dim; j++) {
      sum += M.data[i][j] * v.data[j];
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
  printf("\n    4. 5. 6. Crea, imprime y destruye la matriz M:\n");
  matrix M = create_matrix_from_file("m1.txt");
  print_matrix(M);
  printf("  Dimensiones: %d filas, %d columnas\n", M.row_dim, M.col_dim);
  destroy_matrix(M);
  printf("\n");

  return 0;
}

int probarFuncionesOperaciones() {

  // transpuesta
  printf("\n    7. Transpuesta:\n");
  printf("Matriz original M:\n");
  matrix M = create_matrix_from_file("m1.txt");
  print_matrix(M);
  printf("\n");
  printf("Matriz traspuesta Mt:\n");
  matrix Mt = transpose_matrix(M);
  print_matrix(Mt);
  printf("\n");

  // suma
  printf("\n    8. Suma matriz M por si misma:\n");
  matrix N = sum_matrix_matrix(M, M);
  print_matrix(N);
  printf("\n");

  // multiplicacion m por m
  printf("\n    9. Multiplicacion matriz M por su traspuesta Mt:\n");
  matrix O = mult_matrix_matrix(M, Mt);
  print_matrix(O);
  printf("\n");

  // multiplicacion m por v
  printf("\n    10. Multiplicacion atriz M por vector 1:\n");
  vector *w = mult_matrix_vector(M, create_vector_from_file("v1.txt"));
  print_vector(*w);

  
  // end
  printf("\n");
  destroy_matrix(M);
  destroy_matrix(Mt);
  destroy_matrix(N);
  destroy_matrix(O);
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
