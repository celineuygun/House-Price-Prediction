#ifndef _MODELS_H
#define _MODELS_H

#include "dataset.h"

float model_by_similarity(House* houses, House new_house, int m2, int year, int a, int b, int c);
void create_data_matrices(House *houses, int n, int Xpitch, float X[], int Ypitch, float Y[]);
void print_matrix(int n, int pitch, const float M[]);
void get_transpose(int n, int Apitch, const float A[], float S[]);
void get_cofactor(int n, const float A[], float C[], int p, int q);
float get_determinant(int n, const float A[]);
void get_adjoint(int n, const float A[], float S[]);
int get_inverse(int n, int Apitch, const float A[], float I[]);
void get_multiplication(int Arow, int Apitch, const float A[], int Brow, int Bpitch, const float B[], float C[]);
float *calculate_parameter(int n, int Xpitch, float X[], int Ypitch, float Y[]);
void make_prediction(House *houses_test, float W[]);

#endif // _MODELS_H
