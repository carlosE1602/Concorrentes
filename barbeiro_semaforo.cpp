#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <semaphore.h>

using namespace std;

sem_t sCli, sFila; //iniciados com 0
sem_t mux;         //iniciado com 1
int i = 0;

int atendidos = 0, desistiram = 0;
void barbeiro(int n)
{
    for (; i < n; ++i)
    {
        sem_wait(&sCli);
        cout << "Chama cliente" << endl;
        sem_post(&sFila);
    }
    pthread_exit(NULL);
}

void cliente(int id)
{
    sem_wait(&mux);
    int nEspera;
    sem_getvalue(&sCli, &nEspera);
    if (nEspera < 3)
    {
        sem_post(&sCli);
        atendidos++;
        sem_post(&mux);
        sem_wait(&sFila);
        cout << id << " atendido" << endl;
    }
    else
    {   
        i++;
        cout << id << " desiste" << endl;
        desistiram++;
        sem_post(&mux);
    }
    pthread_exit(NULL);
}
int main()
{
    sem_init(&sCli, 0, 0);
    sem_init(&sFila, 0, 0);
    sem_init(&mux, 0, 1);
    int n = 100;
    vector<thread> threads;
    threads.push_back(thread(barbeiro,n)); //barbeiro corta n cabelos
    for(int c = 0;c<n;c++) threads.push_back(thread(cliente,c));

    for(auto &t: threads) t.join();

    // mf.resultado();
    cout << atendidos << " clientes foram atendidos e " << desistiram << " clientes desistiram\n"; 
    return 0;
}