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

sem_t naoVazio,naoCheio,mux;

//funcao que imprime o buffer
#define print for(int i = 0;i<bSize;i++){cout << buffer[i] << " ";}cout << "\n";

void produtor(int n) {
    int pin = 0;
    int msg = 0;
    for(int i = 0;i<n;i++){
        sem_wait(&naoCheio);
        msg++;
        buffer[pin]=msg;
        print
        pin=(pin+1)%bSize;
        sem_post(&naoVazio);
   }
}

int pout = 0;
void consumidor(int n,int id) {
    int msg;
    int esperado = 1;
    for(int i = 0;i<n;i++){
        sem_wait(&naoVazio);
        sem_wait(&mux);
            msg=buffer[pout];
            pout=(pout+1)%bSize;
        sem_post(&mux);        
//        if(msg != esperado*id+esperado){ cout << "ERRO*************\n" << msg << " " << esperado << "\n"; }
        esperado++;
        sem_post(&naoCheio);    
    }
}


int main(int argc, char **argv){
    sem_init(&naoVazio,0,0);        /*começa em 0, pois o buffer inicialmente nao tem espaco preenchido,
                                    ou seja esse semaforo so libera para leitura quando o buffer tem alguma coisa
                                    para ser lida*/ 
       
    sem_init(&naoCheio,0,bSize);    /*começa em 0, pois num primeiro momento temos 5 espacos vazios,
                                    ou seja esse semaforo so libera quando tem espaco vazio no buffer*/
    
    sem_init(&mux,0,1);             //semaforo que faz papel de mutex
    int n = atoi(argv[1]);
    thread p(produtor,n*10);
    vector<thread> threads;
//    thread c(consumidor,n);
    
    //10 threads consumidoras
    for(int i =0;i<10;i++)
        threads.push_back(thread(consumidor,n,i));


    p.join();
    for(thread &c:threads)    c.join();

    sem_destroy(&naoCheio);
    sem_destroy(&naoVazio);
}
