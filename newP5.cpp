#include <iostream>
#include <cmath>
#include <list>
#include <omp.h>
#include <algorithm>

using namespace std;


void geraPrimos(list<int> &primos, int n){
    int cont = 0;
    bool primo = true;
    #pragma omp parallel for default(none) private(cont) shared(n,primos,cout) firstprivate(primo)
    for(int i = 2;i<=n;i++){
        // printf("%f",)
        for(auto j:primos) 
            if(i%j == 0){
                primo = false;
            }
        #pragma omp critical
        if(primo) primos.push_back(i);

        primo = true;
    }

}


int main(){
    list<int> primos;

    int n;

    cin >> n;

    geraPrimos(primos,n);

    for(auto i:primos) cout << i << " ";

    cout << "\n";

    return 0;

}