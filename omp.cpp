#include <iostream>
#include <omp.h>
#include <assert.h>
#include <thread>

using namespace std;

// int main(){
//     omp_set_num_threads(3);
//     int x=10;
//     #pragma omp parallel reduction(*:x)
//     x+=2;
	
//     printf("x = %d\n",x);
//     return 0;
// }

// int main(int argc,char ** argv){
//     int numThreads = atoi(argv[1]);

//     #pragma omp parallel //num_threads(numThreads)
//     {
//         int myself = omp_get_thread_num();
//         int total = omp_get_num_threads();

//         printf("Hello from thread %d of %d\n",myself,total);
//     }
// }

//PRODUTO ESCALAR v = {a,b} u = {c,d} <v,u> = a*c + b*d



// 1 2 3 4 5 ==> 3 threads  5*id+1/3 = 
void preencheA(int *v,int n){

    int totalThreads = omp_get_num_threads();
    int threadId = omp_get_thread_num();

    int myBegin = n*threadId/totalThreads;
    int myEnd = n*(threadId+1)/totalThreads;

    // printf("%d %d %d \n", myBegin,myEnd,threadId);
    for(int i = myBegin;i<myEnd;i++){
        v[i] = i+1;
    }

}

void preencheB(int *v,int n){

    int totalThreads = omp_get_num_threads();
    int threadId = omp_get_thread_num();

    int myBegin = n*threadId/totalThreads;
    int myEnd = n*(threadId+1)/totalThreads;

    // printf("%d %d %d \n", myBegin,myEnd,threadId);
    for(int i = myBegin;i<myEnd;i++){
        v[i] = i+1;
    }

}

// 0 1 2 3 4 5 6 7 8 9

// id = 0==> 10 * 0/8 = 0
//id = 1 10*1/8 = 10/8 = 1

// void produtoEscalar(int *v, int *u, int n, long long int &produto){
//     int totalThreads = omp_get_num_threads();
//     int threadId = omp_get_thread_num();
//     long long int somaParcial = 0;

//     int myBegin = n*threadId/totalThreads;
//     int myEnd = n*(threadId+1)/totalThreads;

//     for(int i = myBegin;i<myEnd;i++){
//         somaParcial+= v[i]*u[i];
//     }

//     #pragma omp critical
//     produto+= somaParcial;
// }

// long long int confere(int*v,int *u,int n){
//     long long int soma = 0;
//     for(int i = 0;i<n;i++){
//         soma+= v[i] * u[i];
//     }
//     return soma;
// }

// int main(){
//     int *v,*u;
//     int SIZE = 1000000;

//     v = new int[SIZE];
//     u = new int[SIZE];

//     #pragma omp parallel
//     {
//         preencheA(u,SIZE);
//         preencheA(v,SIZE);
//     }
    

//     long long int produto = 0;
//     std::chrono::time_point<std::chrono::high_resolution_clock> tp1,tp2;
//     tp1=std::chrono::high_resolution_clock::now();
//         #pragma omp parallel
//         produtoEscalar(v,u,SIZE,produto);
    
//     tp2=std::chrono::high_resolution_clock::now();
//     double tt=std::chrono::duration<double,std::ratio<1,1000>> (tp2-tp1).count();

//     cout << produto << " " << tt << "\n";

//     long long int newsoma = 0;

//     std::chrono::time_point<std::chrono::high_resolution_clock> tp3,tp4;
//     tp3=std::chrono::high_resolution_clock::now();

//         newsoma = confere(v,u,SIZE);
    
//     tp4=std::chrono::high_resolution_clock::now();
//     double tt1=std::chrono::duration<double,std::ratio<1,1000>> (tp4-tp3).count();

//     cout << newsoma << " " << tt1 << "\n";
//     // assert(produto!=confere(v,u,SIZE));
//     delete []v;
//     delete []u;
    
// }

int soma(int a,int b) {return a+b;}
int produto(int a,int b){return a*b;}

typedef int (*func)(int,int);

int teste(int a,int b,func f){
    return f(a,b);
}

int main(){
    // int a = 10,b = 15;
    // cout << teste(a,b,soma) << "\n";
    // cout << teste(a,b,produto) << "\n";
    omp_set_num_threads(4); //4 threads executaram o loop abaixo
    
    
//    #pragma omp parallel for schedule(static,2)
    //chunk = 2, 4 threads  
    //t 0 -> 0 1, t1 -> 2 ,3  t2 4,5 t3 6,7 t0 8 9

//    #pragma omp parallel for schedule(dynamic,2)
    //chunk = 2, 4 threads
    //a thread que chegar primeiro recebe o proximo chunk

    #pragma omp parallel for schedule(guided,2)
    //chunks tem no minimo tamanho igual a 2,
    //primeira leva de chunks tem 16/4 = 4, cada thread pega um chunk com 4

    for(int i = 0;i<16;i++){
        printf("Iteração %d executada pela thread %d\n\n",i,omp_get_thread_num());
    }

    return 0;
}

