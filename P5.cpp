#include <iostream>
#include <cmath>
#include <list>
#include <omp.h>
#include <algorithm>
#include <chrono>

using namespace std;


void geraPrimos(list<int> &primos, int n){

    //e possível paralelizar pois cada thread executara um trecho tentando testar se os valores desse trecho são primos, se forem adiciona na lista de vetores
    //para evitar que as ultimas threads peguem valores maiores de i, mudei o schedule para guided com minimo de 8 iterações
    #pragma omp parallel for default(none) schedule(guided,8)shared(n,primos,cout)
    for(int i = 2;i<=n;i++){
        int cont = 0;
        
        //poderia paralelizar, mas seria meio inutil ja que teria que ficar sempre criando novas threads, além disso da pra usar o break para agilizar
        for(int j = 1;j<= int(sqrt(i));j++){
            if (i != 2 && i % 2 == 0) { cont = 2; break; } //se for par e diferente de dois não precisa mais testar


            if(i%j == 0){
                cont ++;
                if(cont > 1) break; //se ja é maior que 1 nao precisa mais testar
            }
        }

        if(cont == 1){
            #pragma omp critical
            primos.push_back(i);
        }
    }

}


void checaPrimos(list<int> &primos, int n){
    int cont = 0;
    bool primo = true;
    for(int i = 2;i<=n;i++){
        // printf("%f",)
        for(auto j:primos) 
            if(i%j == 0){
                primo = false;
            }
        if(primo) primos.push_back(i);

        primo = true;
    }

}



int main(){
    list<int> primos;
    list<int> primosCheck;


    int n = 1000000;

    // cin >> n;


    auto start = chrono::system_clock::now();
    geraPrimos(primos,n);

    auto end = chrono::system_clock::now();
    chrono::duration<double> time = end - start;

    cout << time.count() << '\n';

    
    primos.sort();
    
    // for(auto i:primos) cout << i << " ";
    // cout << "\n";


    //Parte usada para conferir, esta comentado pois o sequencial estava lento
    // checaPrimos(primosCheck,n);


    // cout << "\n";
    // if(primosCheck.size() != primos.size()) cout << "ERRO\n";
    // else{
    //     list<int>::iterator it = primos.begin();
    //     list<int>::iterator it1 = primosCheck.begin();
    //     while(it != primos.end() and it1 != primosCheck.end()){
    //         if(*it != *it1) {
    //             cout << "ERRO\n";
    //             printf("%d != %d",*it,*it1);
    //         }

    //         it++;it1++;

    //     }
    // }
    return 0;

}
