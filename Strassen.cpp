#include <iostream>

using type = int;
using Matrix = type * *;
using size = std::size_t;

Matrix createMatrix(size N) {
	Matrix newMatrix = new type *[N];
	for (unsigned i = 0; i < N; ++i) {
		newMatrix[i] = new type[N];
	}
	return newMatrix;
}

Matrix fillMatrix(int Mat[], size N){
    Matrix A = createMatrix(N);
    
    unsigned k = 0;
    for(unsigned i=0; i<N; ++i){
        for(unsigned j=0; j<N; ++j){
            A[i][j] = Mat[k++];
        }
    }
    return A;
}

Matrix padMatrix(Matrix Mat, size N){
	Matrix A = createMatrix(N+1);

	for(unsigned i=0; i<N+1; ++i){
        for(unsigned j=0; j<N+1; ++j){
			if(i < N && j < N){
            	A[i][j] = Mat[i][j];
			}
			else{
				A[i][j] = 0;
			}
        }
    }

	return A;
}

Matrix add(Matrix A, Matrix B, size N) {

	Matrix C = createMatrix(N);

	for (unsigned i = 0; i < N; ++i)
		for (unsigned j = 0; j < N; ++j)
			C[i][j] = A[i][j] + B[i][j];

	return C;
}

Matrix subtract(Matrix A, Matrix B, size N) {
	Matrix C = createMatrix(N);

	for (unsigned i = 0; i < N; ++i)
		for (unsigned j = 0; j < N; ++j)
			C[i][j] = A[i][j] - B[i][j];

	return C;
}

// Strassen's Algorithm
Matrix strassen(Matrix A, Matrix B, size N) {

	if (N == 1) {
		Matrix C = createMatrix(N);
		C[0][0] = *A[0] * *B[0];
		return C;
	}
	else if(N % 2 == 1){
		A = padMatrix(A, N);
		B = padMatrix(B, N);
		++N;
	}

	Matrix C = createMatrix(N);

	size K = N / 2;

	Matrix A11 = createMatrix(K);
	Matrix A12 = createMatrix(K);
	Matrix A21 = createMatrix(K);
	Matrix A22 = createMatrix(K);
	Matrix B11 = createMatrix(K);
	Matrix B12 = createMatrix(K);
	Matrix B21 = createMatrix(K);
	Matrix B22 = createMatrix(K);

	for (unsigned i = 0; i < K; ++i) {
		for (unsigned j = 0; j < K; ++j) {
			A11[i][j] = A[i][j];
			A12[i][j] = A[i][K + j];		
			A21[i][j] = A[K + i][j];
			A22[i][j] = A[K + i][K + j];
			B11[i][j] = B[i][j];
			B12[i][j] = B[i][K + j];		
			B21[i][j] = B[K + i][j];
			B22[i][j] = B[K + i][K + j];
		}
	}

	Matrix S1 = subtract(B12, B22, K);
	Matrix S2 = add(A11, A12, K);
	Matrix S3 = add(A21, A22, K);
	Matrix S4 = subtract(B21, B11, K);
	Matrix S5 = add(A11, A22, K);
	Matrix S6 = add(B11, B22, K);
	Matrix S7 = subtract(A12, A22, K);
	Matrix S8 = add(B21, B22, K);
	Matrix S9 = subtract(A11, A21, K);
	Matrix S10 = add(B11, B12, K);

	Matrix P1 = strassen(A11, S1, K);
	Matrix P2 = strassen(S2, B22, K);
	Matrix P3 = strassen(S3, B11, K);
	Matrix P4 = strassen(A22, S4, K);
	Matrix P5 = strassen(S5, S6, K);
	Matrix P6 = strassen(S7, S8, K);
	Matrix P7 = strassen(S9, S10, K);

	Matrix C11 = subtract(add(add(P5, P4, K), P6, K), P2, K);				// P5 + P4 - P2 + P6
	Matrix C12 = add(P1, P2, K);								// P1 + P2
	Matrix C21 = add(P3, P4, K);								// P3 + P4
	Matrix C22 = subtract(subtract(add(P5, P1, K), P3, K), P7, K);				// P1 + P5 - P3 - P7

																			
	for (unsigned i = 0; i < K; ++i) {
		for (unsigned j = 0; j < K; ++j) {

			C[i][j] = C11[i][j];

			C[i][j + K] = C12[i][j];

			C[K + i][j] = C21[i][j];

			C[K + i][K + j] = C22[i][j];
		}
	}

	return C;
}

void main(){

    int M1[] = {1, 2, 8, 3, 6, 8, -7, 0, -7};
    int M2[] = {0, 10, 4, 9, 5, 6, 7, -1, -11};
    size n = 3;

    Matrix A = fillMatrix(M1, n);
    Matrix B = fillMatrix(M2, n);

    Matrix C = strassen(A, B, n);

    for(unsigned i=0; i<n; ++i){
        for(unsigned j=0; j<n; ++j){
            std::cout << C[i][j] << " ";
        }
        std::cout << "\n";
    }
}