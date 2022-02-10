#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <semaphore.h>

using namespace std;

//tamanho do buffer
#define bSize 5
int buffer[bSize];

sem_t naoVazio,naoCheio;

//funcao que imprime o buffer
#define print for(int i = 0;i<bSize;i++){cout << buffer[i] << " ";}cout << "\n";

void produtor(int n) {
    int pin = 0;
    int msg = 0;
    for(int i = 0;i<n;i++){
        msg++;
        sem_wait(&naoCheio);
        buffer[pin]=msg;
        print
        sem_post(&naoVazio);
        pin=(pin+1)%bSize;
   }
}

void consumidor(int n) {
    int pout = 0;
    int msg;
    int esperado = 1;
    for(int i = 0;i<n;i++){
        sem_wait(&naoVazio);
        msg=buffer[pout];
        sem_post(&naoCheio);    
        pout=(pout+1)%bSize;
        if(msg != esperado){ cout << "ERRO*************\n" << msg << " " << esperado << "\n"; }
        esperado++;
    }
}


int main(int argc, char **argv){
    sem_init(&naoVazio,0,0);        /*começa em 0, pois o buffer inicialmente nao tem espaco preenchido,
                                    ou seja esse semaforo so libera para leitura quando o buffer tem alguma coisa
                                    para ser lida*/ 
       
    sem_init(&naoCheio,0,bSize);    /*começa em 0, pois num primeiro momento temos 5 espacos vazios,
                                    ou seja esse semaforo so libera quando tem espaco vazio no buffer*/

    int n = atoi(argv[1]);
    thread p(produtor,n);
    thread c(consumidor,n);
    p.join();
    c.join();
    sem_destroy(&naoCheio);
    sem_destroy(&naoVazio);
}
