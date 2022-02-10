/* Carlos Eduardo Sabino 98894 */

#include <iostream>
#include <thread>
#include <vector>

using namespace std;

void sum(const int* v, int n,int id, long long int &sum, int num_threads){
    int inicio = id*n / num_threads;
    int final = (id+1)*n / num_threads;

    long long int sum_aux = 0;
    for(int i = inicio; i<final ;i++){
        sum_aux+=v[i];
    }
    //retorno da função
    sum = sum_aux;
}

int main(int argc, char ** argv){
    //para executar o programa passe ./a.out tamanho_array num_threads
    int n,num_threads;
    if (argc == 3){
        n = atoi(argv[1]);
        num_threads = atoi(argv[2]);
    }else if(argc == 2){
        n = atoi(argv[1]);
        num_threads = thread::hardware_concurrency();
    }else{
        n = 100000;
        num_threads = thread::hardware_concurrency();
    }
    int *v = new int[n];
    //v = {1,2,3,4....}
    for(int i = 0;i<n;i++){
        v[i] = i+1;
    }

    //vetor com as somas parciais, começa em 0, long pq a soma 
    long long int sum_[num_threads] = {};

    //incia contagem de tempo
    std::chrono::time_point<std::chrono::high_resolution_clock> tp1,tp2;
    tp1=std::chrono::high_resolution_clock::now();
    //vetor como todas as threads definidas
    vector<thread> threads;
    for(int i =0;i<num_threads;i++){
        threads.push_back(thread(sum,v,n,i,ref(sum_[i]),num_threads));
    }

    for(thread &t: threads)
        t.join();

    //termina contagem de tempo
    tp2=std::chrono::high_resolution_clock::now();
    double tt=std::chrono::duration<double,std::ratio<1,1000>> (tp2-tp1).count();

    long long int total = 0;
    
    for(int i =0;i<num_threads;i++){
        total+= sum_[i];
    }
    //gauss first n numbers sum
    long long int total_ = (long(n)*long(n+1))/2;

    if(total == total_)
        cout << "Soma correta, executada em " << tt << "\n";
    else cout << "Soma incorreta\n";

    delete [] v;
    return 0;
}