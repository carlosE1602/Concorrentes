#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>

/*
Esta errado mas nao sei aonde, talvez nao tenha entendido corretamente o problema
*/


using namespace std;

#define n 100 //numero de clientes que o barbeiro vai cortar
#define queueSize 3


class MonitorBarbeiro{
    private:
        int fila = 0;
        int numCortes;
        int cortou = 0,desistiu = 0;
        mutex mux;
        condition_variable podeCortar, esperaBarbeiro;
    public:

        MonitorBarbeiro(int numCortes) {this->numCortes = numCortes;}
        void corta(){
            unique_lock<mutex> lck(mux);
            for(int i = 0;i<numCortes;i++){
                esperaBarbeiro.wait(lck,[this]()->bool{
                        return fila != 0;
                        });
                printf("Cortando cabelo\n");
                printf("Ainda tem %d pessoas para cortar\n",--fila);
                // fila--;
                podeCortar.notify_all();
                // usleep(10000);
            }
        }
        void querCortar(int id){
            unique_lock<mutex> lck(mux);
            if(fila < queueSize){           // se tem espaco na fila, entra, avisa que quer cortar, e espera ate ser chamado
                esperaBarbeiro.notify_one();
                
                fila++;
                printf("Cliente %d entrou na fila e esta esperando\n",id);
                podeCortar.wait(lck);
                cortou++;
            }else{
                printf("A fila tem %d pessoas e o cliente %d foi nao pode cortar o cabelo\n",fila,id);
                numCortes--;
                desistiu++;
            }
            printf("%d foi embora \n",id);
        }

        void resultado(){
            printf("%d pessoas cortaram e %d desistiram, ao todo %d pessoas passaram pela barbearia\n",cortou,desistiu,cortou+desistiu);
            printf("No final %d pessoas ficaram na fila\n",fila);
        }


};

MonitorBarbeiro mf(n);

void barbeiro(){
    mf.corta();
}

void cliente(int id){
    // printf("thread %d começou\n",id);
    usleep(1000);
    mf.querCortar(id);
    // printf("thread %d terminou\n",id);

}

int main(){
    vector<thread> threads;
    threads.push_back(thread(barbeiro)); //barbeiro corta n cabelos
    for(int i = 0;i<n;i++) threads.push_back(thread(cliente,i));

    for(auto &i: threads) i.join();

    mf.resultado();
    return 0;
}