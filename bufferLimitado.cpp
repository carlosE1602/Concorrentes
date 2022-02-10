#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace std;
class MonitorProdCons {
    private:
     int bSize,pin,pout,cont;
     int *buffer;
     mutex mux;
     condition_variable naoCheio,naoVazio;

     
    public:
void printBuffer(){ for(int i =0;i<bSize;i++) cout << buffer[i] << " "; cout << "\n";}
     MonitorProdCons(int bufferSize){
        bSize=bufferSize;
        buffer = new int[bSize];
        pin=pout=cont=0;
     }
     ~MonitorProdCons(){
        delete [] buffer;
     }
        void produzir(int msg) {
             unique_lock<mutex> lck(mux);
             naoCheio.wait(lck,[this](){
                            return cont<bSize; });
             buffer[pin]=msg;
             pin=(pin+1)%bSize;
             ++cont;
             printBuffer();
             naoVazio.notify_one();
         }
         void consumir(int &msg) {
             unique_lock<mutex> lck(mux);
             naoVazio.wait(lck,[this](){
             return cont>0; });
             msg=buffer[pout];
             pout=(pout+1)%bSize;
             --cont;
             printBuffer();
             naoCheio.notify_one();
         }
};


MonitorProdCons pd(5);

void produz(int msg){
    for(int i =0;i<msg;i++){
        pd.produzir(i);
    }
}

void le(int msg){
    for(int i = 0;i<msg;i++){
        pd.consumir(i);
    }
}

int main(){
    int n = 21;
    thread p(produz,n);
    thread c(le,n);
    p.join();
    c.join();
}
