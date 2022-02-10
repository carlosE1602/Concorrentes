//#include <pthread.h>
#include <thread>
#include <iostream>
#include "math.h"

using namespace std;

//void* f(void *p){
//    //faz o casting do ponteiro pra void, para long int
//    long res = (long)p;
//    long sum = 0;
//    for(int i = 0;i<10;i++){
//        cout << res << " " << i << "\n";
//        sum+= res*i;
//    }
//    pthread_exit((void*)sum);
//}

int s = 0;

void f(){
    for(int i = 0;i<10;i++)
        s+=1;
}

int main(){
    thread t1(f);
    thread t2(f);
    t1.join();
    t2.join();
    cout << s << "\n";
    return 0;
//    pthread_t t1,t2;
//    pthread_create(&t1, NULL, f, (void*)1);
//    pthread_create(&t2, NULL, f, (void*)2);
//
//    void *r1,*r2;
//    pthread_join(t1,&r1);
//    pthread_join(t2,&r2);
//    cout << (long)r1 << " " << (long)r2 << "\n";
//    pthread_exit(NULL);

//    return 0;s
}
