
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
//      funciones de tarea

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
            A.data[A.row_dim - 1] = realloc(A.data[A.row_dim - 1], A.col_dim * sizeof(float));
            sscanf(tok, "%f", &(A.data[A.row_dim - 1][A.col_dim - 1]));
            tok = strtok(NULL, " ");
        }
    }
    fclose(fp);
    return A;
}

void print_vector(vector v) {
  for (int i = 0; i < v.dim; i++) {
    printf("%f ", v.data[i]);
  }
  printf("\n");
}

void print_matrix(matrix A) {
    for (int i = 0; i < A.row_dim; i++) {
        for (int j = 0; j < A.col_dim; j++) {
            printf("%f ", A.data[i][j]);
        }
        printf("\n");
    }
}

void destroy_vector(vector v) { free(v.data); }

void destroy_matrix(matrix A) {
  for (int i = 0; i < A.row_dim; i++) {
    free(A.data[i]);
  }
  free(A.data);
}

//
//      funciones para probar las funciones de la tarea

int probarFuncionesVectores() {
  vector v = create_vector_from_file("ej1.txt");

  printf("Dimensión del vector: %d\n", v.dim);
  print_vector(v);
  destroy_vector(v);

  return 0;
}

int probarFuncionesMatrix() {
  matrix A = create_matrix_from_file("ej2.txt");

  printf("Dimensiones de la matriz: %dx%d\n", A.row_dim, A.col_dim);
  print_matrix(A);
  destroy_matrix(A);

  return 0;
}

//
//      funcion orquesatador

int Orq() {
  probarFuncionesVectores();
  probarFuncionesMatrix();

  return 0;
}

//
//      funcion main

int main() {
  Orq();
  return 0;
}
