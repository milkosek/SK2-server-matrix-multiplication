#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <thread>
#include <iostream>
#include <cstring>
#include <future>
#include <cmath>

#define SERVER_PORT 1235

using type = double;
using Matrix = type * *;
using size = std::size_t;

Matrix createMatrix(size N) {
    Matrix newMatrix = new type *[N];
    for (unsigned i = 0; i < N; ++i) {
        newMatrix[i] = new type[N];
    }
    return newMatrix;
}

Matrix fillMatrix(double Mat[], size N){
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

    /*Matrix S1 = subtract(B12, B22, K);
    Matrix S2 = add(A11, A12, K);
    Matrix S3 = add(A21, A22, K);
    Matrix S4 = subtract(B21, B11, K);
    Matrix S5 = add(A11, A22, K);
    Matrix S6 = add(B11, B22, K);
    Matrix S7 = subtract(A12, A22, K);
    Matrix S8 = add(B21, B22, K);
    Matrix S9 = subtract(A11, A21, K);
    Matrix S10 = add(B11, B12, K);*/

    auto future1 = std::async(std::launch::async, subtract, B12, B22, K);
    auto future2 = std::async(std::launch::async, add, A11, A12, K);
    auto future3 = std::async(std::launch::async, add, A21, A22, K);
    auto future4 = std::async(std::launch::async, subtract, B21, B11, K);
    auto future5 = std::async(std::launch::async, add, A11, A22, K);
    auto future6 = std::async(std::launch::async, add, B11, B22, K);
    auto future7 = std::async(std::launch::async, subtract, A12, A22, K);
	auto future8 = std::async(std::launch::async, add, B21, B22, K);
    auto future9 = std::async(std::launch::async, subtract, A11, A21, K);
    auto future10 = std::async(std::launch::async, add, B11, B12, K);

    const Matrix S1 = future1.get();
	future1 = std::async(std::launch::async, strassen, A11, S1, K);
	const Matrix S2 = future2.get();
    future2 = std::async(std::launch::async, strassen, S2, B22, K);
	const Matrix S3 = future3.get();
    future3 = std::async(std::launch::async, strassen, S3, B11, K);
	const Matrix S4 = future4.get();
    future4 = std::async(std::launch::async, strassen, A22, S4, K);
	const Matrix S5 = future5.get();
	const Matrix S6 = future6.get();
    future5 = std::async(std::launch::async, strassen, S5, S6, K);
	const Matrix S7 = future7.get();
	const Matrix S8 = future8.get();
    future6 = std::async(std::launch::async, strassen, S7, S8, K);
	const Matrix S9 = future9.get();
	const Matrix S10 = future10.get();
    future7 = std::async(std::launch::async, strassen, S9, S10, K);

    
    const Matrix P2 = future2.get();
    const Matrix P4 = future4.get();
    const Matrix P5 = future5.get();
    const Matrix P6 = future6.get();

    /*create_result = threads[0](strassen, A11, S1, K);
    create_result = threads[1](strassen, S2, B22, K);
    create_result = threads[2](strassen, S3, B11, K);
    create_result = threads[3](strassen, A22, S4, K);
    create_result = threads[4](strassen, S5, S6, K);
    create_result = threads[5](strassen, S7, S8, K);
    create_result = threads[6](strassen, S9, S10, K);

    threads[0].join();
    threads[1].join();
    threads[2].join();
    threads[3].join();
    threads[4].join();
    threads[5].join();
    threads[6].join();*/

    /*Matrix P1 = strassen(A11, S1, K);
    Matrix P2 = strassen(S2, B22, K);
    Matrix P3 = strassen(S3, B11, K);
    Matrix P4 = strassen(A22, S4, K);
    Matrix P5 = strassen(S5, S6, K);
    Matrix P6 = strassen(S7, S8, K);
    Matrix P7 = strassen(S9, S10, K);*/

    /*Matrix C11 = subtract(add(add(P5, P4, K), P6, K), P2, K);				// P5 + P4 - P2 + P6
    Matrix C12 = add(P1, P2, K);								// P1 + P2
    Matrix C21 = add(P3, P4, K);								// P3 + P4
    Matrix C22 = subtract(subtract(add(P5, P1, K), P3, K), P7, K);	*/			// P1 + P5 - P3 - P7

    Matrix C11 = subtract(add(add(P5, P4, K), P6, K), P2, K);  // P5 + P4 - P2 + P6
    const Matrix P1 = future1.get();
    Matrix C12 = add(P1, P2, K);								// P1 + P2
    const Matrix P3 = future3.get();
    Matrix C21 = add(P3, P4, K);								// P3 + P4
    const Matrix P7 = future7.get();
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

void Calculate(int desc)
{
    char buf[50];
    bzero(buf, 50);
    read(desc, buf, 50);
    size s;
    sscanf(buf, ":%lu:", &s);
    std::string b = ":" + std::to_string(s) + ":";
    write(desc, b.c_str(), b.size());
    double *Mat1 = new double[s*s];
    double *Mat2 = new double[s*s];
    for(size i = 0; i<s*s; ++i) {
        bzero(buf, 50);
        read(desc, buf, 50);
        sscanf(buf, ";%lf,%lf;", &Mat1[i], &Mat2[i]);
    }
    for(size i=0; i<s; ++i) {
        for (size j = 0; j < s; ++j) {
            std::cout << Mat1[i+j] << " ";
        }
        std::cout << "\n";
    }
    Matrix ans = strassen(fillMatrix(Mat1, s), fillMatrix(Mat2, s), s);
    delete Mat1;
    delete Mat2;
    std::string toSend = "";
    for(size i=0; i<s; ++i) {
        for (size j = 0; j<s; ++j) {
            std::cout << ans[i][j] << " ";
            toSend += ";" + std::to_string(std::ceil(ans[i][j]*100.0)/100.0);
        }
        std::cout << "\n";
    }
    toSend += ";";
    write(desc, toSend.c_str(), toSend.size());

    pthread_exit(NULL);
}

void handleConnection(int connection_socket_descriptor) {

    try {
        std::thread thread(Calculate, connection_socket_descriptor);
        thread.detach();
    } catch (const std::system_error& e) {
        printf("Błąd przy próbie utworzenia wątku\n");
    }

}

int main(int argc, char* argv[])
{
   int server_socket_descriptor;
   int connection_socket_descriptor;
   int bind_result;
   int listen_result;
   char reuse_addr_val = 1;
   struct sockaddr_in server_address;
   signal(SIGPIPE, SIG_IGN);

   //inicjalizacja gniazda serwera
   
   memset(&server_address, 0, sizeof(struct sockaddr));
   server_address.sin_family = AF_INET;
   server_address.sin_addr.s_addr = htonl(INADDR_ANY);
   server_address.sin_port = htons(SERVER_PORT);

   server_socket_descriptor = socket(AF_INET, SOCK_STREAM, 0);
   if (server_socket_descriptor < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda..\n", argv[0]);
       exit(1);
   }
   setsockopt(server_socket_descriptor, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse_addr_val, sizeof(reuse_addr_val));

   bind_result = bind(server_socket_descriptor, (struct sockaddr*)&server_address, sizeof(struct sockaddr));
   if (bind_result < 0)
   {
       fprintf(stderr, "%s: Błąd przy próbie dowiązania adresu IP i numeru portu do gniazda.\n", argv[0]);
       exit(1);
   }

   listen_result = listen(server_socket_descriptor, 3);
   if (listen_result < 0) {
       fprintf(stderr, "%s: Błąd przy próbie ustawienia wielkości kolejki.\n", argv[0]);
       exit(1);
   }

   while(1)
   {
       connection_socket_descriptor = accept(server_socket_descriptor, NULL, NULL);
       if (connection_socket_descriptor < 0)
       {
           fprintf(stderr, "%s: Błąd przy próbie utworzenia gniazda dla połączenia.\n", argv[0]);
           continue;
       }

       handleConnection(connection_socket_descriptor);
   }
   close(server_socket_descriptor);
   return(0);
}

    /*std::string s = "";

    size n = 3;
    double M1[n*n];
    double M2[n*n];

    Matrix A = fillMatrix(M1, n);
    Matrix B = fillMatrix(M2, n);

    Matrix C = strassen(A, B, n);

    for(unsigned i=0; i<n; ++i){
        for(unsigned j=0; j<n; ++j){
            std::cout << C[i][j] << " ";
        }
        std::cout << "\n";
    }*/

