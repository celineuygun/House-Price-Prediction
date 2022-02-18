#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"models.h"

float model_by_similarity(House* houses, House new_house, int m2, int year, int a, int b, int c) {
    int ind = 0;
    float tmp_mean = 0;
    House* similars = calloc(100, sizeof(House));
    if(!similars) exit(1);
    House* neighborhood = get_neighborhoods(houses, new_house);
    tmp_mean = mean_sale_prices(neighborhood);
    House* sorted = sort_houses(neighborhood, 3, LOT_SIZE, 0, 0, 0, 0, 0);
    similars->size = 0;
    for(int i = 0, ind = 0; i < sorted->size; ++i) {
        if((sorted[i].lotarea <= new_house.lotarea + m2) && (sorted[i].lotarea >= new_house.lotarea - m2)) {
            struct_filler(&(similars[ind]), &(sorted[i]));
            similars[ind].index = ind;
            similars->size++;
            ++ind;
        } 
    } 
    if(similars->size <= 5) {
        if(similars->size != 0) 
            tmp_mean = mean_sale_prices(similars);
        free(similars);
        return tmp_mean;
    } 
    
    free(sorted);
    tmp_mean = mean_sale_prices(similars);
    sorted = sort_houses(similars, 7, YEAR_SIZE, 0, 0, 0, 0, 0);
    similars->size = 0;
    for(int i = 0, ind = 0; i < sorted->size; ++i) {
        if((sorted[i].yearbuilt <= new_house.yearbuilt + year) && (sorted[i].yearbuilt >= new_house.yearbuilt - year)) {
            struct_filler(&(similars[ind]), &(sorted[i]));
            similars[ind].index = ind;
            similars->size++;
            ++ind;
        } 
    } 
    if(similars->size <= 5) {
        if(similars->size != 0) 
            tmp_mean = mean_sale_prices(similars);
        free(similars);
        return tmp_mean;
    } 

    free(sorted);
    tmp_mean = mean_sale_prices(similars);
    sorted = sort_houses(similars, 0, OVERALL_SIZE * (a + b) + KIT_TYPES * c, 0, a, b, c, 0);
    similars->size = 0;
    int overall = new_house.overallqual * a + new_house.overallcond * b;
    if(!strcmp(new_house.kitchenqual, "Gd")) overall += c;
    else if(!strcmp(new_house.kitchenqual, "TA")) overall += 2 * c;
    else if(!strcmp(new_house.kitchenqual, "Fa")) overall += 3 * c;
    else if(!strcmp(new_house.kitchenqual, "Po")) overall += 4 * c;
    overall /= 3;
    for(int i = 0, ind = 0; i < sorted->size; ++i) {
        if((sorted[i].overall <= overall + 1) && (sorted[i].overall >= overall - 1)) {
            struct_filler(&(similars[ind]), &(sorted[i]));
            similars[ind].index = ind;
            similars->size++;
            ++ind;
        } 
    } 
    if(similars->size != 0) 
        tmp_mean = mean_sale_prices(similars);
    free(similars);
    return tmp_mean;
}

void create_data_matrices(House *houses, int n, int Xpitch, float X[], int Ypitch, float Y[]) {
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < Xpitch; ++j) {
            if(j == 0)
                X[i * Xpitch + j] = 1;
            else
                X[i * Xpitch + j] = houses[i].lotarea;
        }
    }
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < Ypitch; ++j)
            Y[i * Ypitch + j] = houses[i].saleprice;
    }
}

void print_matrix(int n, int pitch, const float M[]) {
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < pitch; ++j)
            printf("%.1f ", M[i * pitch + j]);
        printf("\n");
    }
}

void get_transpose(int n, int Apitch, const float A[], float S[]) {
    for(int i = 0; i < n; ++i){
        for(int j = 0; j < Apitch; ++j)
            S[j * n + i] = A[i * Apitch + j];
    }
}

void get_cofactor(int n, const float A[], float C[], int p, int q) {
    int i = 0, j = 0;
    for(int row = 0; row < n; ++row) {
        for(int col = 0; col < n; ++col) {
            if(row != p && col != q) {
                C[i * n + j] = A[row * n + col];
                ++j;
                if(j == n - 1) {
                    j = 0;
                    ++i;
                }
            }
        }
    }
}

float get_determinant(int n, const float A[]) {
    if(n == 1) return A[0];
    float det = 0, B[(n - 1) * (n - 1)];
    int sign = 1;
    for(int i = 0; i < n; ++i) {
        get_cofactor(n, A, B, 0, i); // A[0][i]
        det += sign * A[i] * get_determinant(n - 1, B);
        sign *= -1;
    } return det;
}

void get_adjoint(int n, const float A[], float S[]) {
    if(n == 1) {
        S[0] = 1;
        return;
    }

    int sign = 1;
    float C[n * n];

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            get_cofactor(n, A, C, i, j);
            sign = ((i + j) % 2) ? -1 : 1;
            S[j * n + i] = sign * get_determinant(n - 1, C);
        }
    }
}

int get_inverse(int n, int Apitch, const float A[], float I[]) {
    float S[n * n], det = get_determinant(n, A);
    if(n != Apitch && det == 0) return 0;
    get_adjoint(n, A, S);
    
    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            I[i * n + j] = S[i * n + j] / det;
        }
    } return 1;
}

/*
 * C(i,j) = L(i)A x C(j)B = k: 1->n, sum(a(i,k) . b(k,j))
 */
void get_multiplication(int Arow, int Apitch, const float A[],
                        int Brow, int Bpitch, const float B[],
                        float C[]) {
    if(Apitch != Brow) {
        printf("Error while multiplying.\n");
        exit(1);
    }
    for(int i = 0; i < Arow; ++i) {
        for(int j = 0; j < Bpitch; ++j) {
            float sum = 0;
            for(int k = 0; k < Apitch; ++k) {
                sum += A[i * Apitch + k] * B[k * Bpitch + j];
            } C[i * Bpitch + j] = sum;
        }
    }
}

/*
 * (X^T x X)^-1 x X^T x Y = W
 */
float *calculate_parameter(int n, int Xpitch, float X[], int Ypitch, float Y[]) {
    float *XT = calloc(Xpitch * n, sizeof(float)), *XT_X = calloc(Xpitch * Xpitch, sizeof(float)), *I = calloc(Xpitch * Xpitch, sizeof(float)), *XT_Y = calloc(Xpitch * Ypitch, sizeof(float));
    get_transpose(n, Xpitch, X, XT);
    get_multiplication(Xpitch, n, XT, n, Xpitch, X, XT_X);
    if(!get_inverse(Xpitch, Xpitch, XT_X, I)){
        printf("Matrix isn't invertible\n");
        return NULL;
    }
    get_multiplication(Xpitch, n, XT, n, Ypitch, Y, XT_Y);
    float *W = calloc(Xpitch * Ypitch, sizeof(float));
    if(!W) exit(1);
    get_multiplication(Xpitch, Xpitch, I, Xpitch, Ypitch, XT_Y, W);
    free(XT);
    free(XT_X);
    free(I);
    free(XT_Y);
    return W; 
}

void make_prediction(House *houses_test, float W[]) {
    int Xpitch = 2, Ypitch = 1, size = houses_test->size;
    float *X = calloc(Xpitch * size, sizeof(int));
    float *Y = calloc(Ypitch * size, sizeof(int));
    if(!X || !Y) exit(1);
    create_data_matrices(houses_test, size, Xpitch, X, Ypitch, Y);
    get_multiplication(size, Xpitch, X, Xpitch, Ypitch, W, Y);

    FILE* fp = fopen("price_prediction_by_linear_regression.txt", "w");
    if(fp) {
        for(int i = 0; i < size; ++i)
          fprintf(fp, "House ID: %-3d Estimated price: %.2f\n", houses_test[i].id, Y[i * Ypitch]);
        fclose(fp);
        printf("\nThe file named 'price_prediction_by_linear_regression.txt' is created.\n");
    } else printf("\nError while opening the file.\n");
    free(X);
    free(Y);
}