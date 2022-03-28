#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>
#include <vector>
#include <list>
#include <iostream>
#include <algorithm>
using namespace std;


vector<int> primos;
vector<int> primosCheck;

void geraPrimos(int n){
    for (int i = 2; i < n; i++) {
        int cont = 0;

        for (int j = 1; j <= int(sqrt(i)); j++) {
            if(i % j == 0) cont++;
        }

        if(cont == 1) {
            primosCheck.push_back(i);
        }
    }

}

int main(){
    int n = 100000;
    // cin >> n;

    // geraPrimos(n);

    int count = 0;
    // primos.push_back(2); //2 sempre é primo
    #pragma omp parallel for default(none) private(count) shared(n, primos, cout)

    for (int i = 2; i < n; i++) {

        for (int j = 1; j <= (int)sqrt(i); j++) {
            if (i != 2 && i % 2 == 0) { count = 2; break; }
            if (i % j == 0) {
                count++;
                if (count > 1) break;
            }
        }

        if (count == 1){
            #pragma omp critical
            primos.push_back(i);
        }

        count = 0;
    }


    // geraPrimos(n);
    // // std::sort(primos.begin(),primos.end());
    // for(auto i:primos) cout << i << " ";
    // cout << "\n";
    // // for(auto i:primosCheck) cout << i << " ";
    // // cout << "\n";
    // cout << int(primos.size()-primosCheck.size()) << " ";
    return 0;
}

