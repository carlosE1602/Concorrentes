/* Uma variavel compartilhada, por n threads com um id distinto
    o dado so pode ser acessado se a soma for igual a um determinado valor
*/

#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <semaphore.h>

using namespace std;

#define N 7
int dadoCompartilhado = 0;
int soma = N;
sem_t mux,cont;

void func(int id){
    while(true){
        sem_wait(&mux);
        if(soma-id >= 0){
            printf("thread %d passou\n",id);
            soma-= id;
            sem_post(&mux);
            break;
        }else if (id > N){              //se o id da thread for maior do que o maximo permitido de forma sozinho(N = 7 id = 10), termina a thread
            sem_post(&mux);
            return;
        }
        sem_post(&mux);
    }     
    usleep(10000);           //permite ver que mais threads entram juntas
    sem_wait(&mux);
        printf("thread %d esta consultando o dado coma a soma igual a %d\n",id,soma);
        dadoCompartilhado += id;
    sem_post(&mux);

    sem_wait(&mux);
        soma+=id;
    sem_post(&mux);
}

int main(){
    sem_init(&mux,0,1);
    // sem_init(&main,0,1);
    vector<thread> t;

    int nThreads = 7;

    for(int i = 0;i<nThreads;i++){
        t.push_back(thread(func,i+1));
    }

    for(auto &i:t) i.join();

    sem_destroy(&mux);
    sem_destroy(&cont);

    //confere se as threads 
    if(dadoCompartilhado != ((N)*(N+1))/2) cout << "Houve um erro durante o compartilhamento de recursos\n";
    else cout << dadoCompartilhado << "\n";
    return 0;
}