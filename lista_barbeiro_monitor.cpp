#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>


using namespace std;

#define n 100 //numero de clientes que o barbeiro vai cortar
#define queueSize 30 //tamanho da fila


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
                
                printf("Barbeiro acordou\n");
                printf("Cortando cabelo\n");
                printf("Ainda tem %d pessoas para cortar\n",--fila);
                printf("Barbeiro voltou a dormir\n");
                podeCortar.notify_all();
            }
        }
        void querCortar(int id){
            // usleep(10000);
            unique_lock<mutex> lck(mux);
            printf("Cliente %d chegou..\n",id);
            if(fila < queueSize){           // se tem espaco na fila, entra, avisa que quer cortar, e espera ate ser chamado
                fila++;
                esperaBarbeiro.notify_one();
                printf("Cliente %d entrou na fila e esta esperando\n",id);
                
                podeCortar.wait(lck);
                cortou++;
            }else{
                if(fila < queueSize) { cout << "houve um erro, uma thread desistiu com espaco na fila\n"; abort();}
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
    mf.querCortar(id);
}

int main(){
    vector<thread> threads;
    threads.push_back(thread(barbeiro)); //barbeiro corta n cabelos
    for(int i = 0;i<n;i++) threads.push_back(thread(cliente,i));

    for(auto &i: threads) i.join();

    mf.resultado();
    return 0;
}