#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace std;

class semaforo{
    private:
        int s;
        condition_variable cond;
        mutex mux;
    public:

        semaforo(int s){
            this->s = s;        
        }
        void P(){
            unique_lock<mutex> lck(mux);
            s--;
            if(s < 0) cond.wait(lck);
        }
        void V(){
            unique_lock<mutex> lck(mux);
            s++;
            if(s>=0) cond.notify_all();
        }
};


semaforo sem(1);

int soma = 0;


//testando um semaforo, para delimitar regiao critica(tipo lock unlock)
void func(int id){
    sem.P();
    soma -= id;
    sem.V();
}


int main(){
    
    vector<thread> t;

    for(int i = 1;i<=10;i++){
        t.push_back(thread(func,i));
    }
    

    for(auto &i:t) i.join();

    cout << soma << "\n";
}
