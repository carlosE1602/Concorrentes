#include <pthread.h>
#include <iostream>
#include <queue>
#include <map>
#include <unistd.h>
#include <time.h>

using namespace std;

pthread_mutex_t mux =
    PTHREAD_MUTEX_INITIALIZER;
map<pthread_t, int> A;
void block()
{
    bool sair = false;
    long eu = pthread_self();
    do
    {
        pthread_mutex_lock(&mux);
        if (A[eu] > 0)
        {
            A[eu]--;
            sair = true;
        }
        pthread_mutex_unlock(&mux);
    } while (!sair);
}

void wakeup(pthread_t t)
{
    pthread_mutex_lock(&mux);
    A[t]++;
    pthread_mutex_unlock(&mux);
}

pthread_mutex_t muxrec = PTHREAD_MUTEX_INITIALIZER;;

int recursos[5] = {1,1,1,1,1};
queue<pthread_t> fila_[5];


void liberar(int R){
    pthread_mutex_lock(&muxrec);
    recursos[R] = 1;        //seta o recurso como livre
    if(!fila_[R].empty()) {             //se a fila nao tiver vazia remove o topo e acorda a thread que esta na fila de espera
        pthread_t id=fila_[R].front();
        fila_[R].pop();
        wakeup(id);
    }
    pthread_mutex_unlock(&muxrec);
}


int requisitar(int R){
    int r;
    bool bloquear;
    do{
        bloquear = 0;
        pthread_mutex_lock(&muxrec);
        if(recursos[R] == 0){       //se o recurso estiver ocupado, bloqueia a thread e coloca na fila de epera
            bloquear = 1;

            fila_[R].push(pthread_self());
        }else{  //se não, falo que esta ocupado e seto r = 1, cada recurso so tem 1s
            recursos[R] = 0;
            r = 1;
        }
        pthread_mutex_unlock(&muxrec);
        if(bloquear){
            block();       //se ao recurso estiver ocupado bloqueia a thread
        }
    }while(bloquear);
    return r;
}


void* func(void* id){

    int u;
    for(int i =0;i<1000;i++){
        int r = rand() % 5;
        u=requisitar(r);
        cout<< (long) id << " requisita " << r << " usando "<<1<<endl;
        usleep(5);
        liberar(r);
    }

    return nullptr;
}

int main() {
    srand(time(NULL));
    int num=4;
    for(int i=0;i<num;++i) {
        pthread_t t;
        pthread_create(&t,NULL,func,
        (void*)i);
        A.insert(pair<pthread_t,int>(t,0));
    }
    for (auto &p:A) {
        pthread_join(p.first,NULL);
    }
    if(!(recursos[0] && recursos[1] && recursos[2] && recursos[3] && recursos[4])){
        cout << "Algum recuros ficou sem desalocar\n";
    }else cout << "Está correto\n";
    for(auto i:fila_)
        if(!i.empty()) cout << "Fila não esta correta\n";


}