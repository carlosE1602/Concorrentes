#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<semaphore.h>
using namespace std;

//     int bebida=0;           //inicialmente o copo está vazio
//     mutex mux;
//     int cont[3]={0,0,0};    //usado para verificação do resultado

//     void serve(int b) {
//         unique_lock<mutex> lck(mux);
//         vazio.wait(lck,[this]{return bebida == 0;});
//         bebida=b;
//         // printf("servindo bebida %d\n",b);

//         cheio.notify_all();         //versão 1
//         // cheio.notify_one();         //versão 2
//     }

//     bool bebe(int id) {
//         unique_lock<mutex> lck(mux);

//         /* versão 1: Garçon precisa fazer notify-all */
//         cheio.wait(lck,[this,id]{return bebida!=0 && bebida<=id;});

//         /* versão 2: Se o convidado não quer beber, ele acorda um possível 
//         interessado e volta a dormir */
//         // while(bebida==0 || bebida>id) {
//         //     cheio.wait(lck);
//         //     if (bebida>id)
//         //         cheio.notify_one();
//         // }

//         if (bebida==-1) return false;   //indica o fim da festa
//         // printf("             %d bebe bebida %d\n",id,bebida);
//         cont[id-1]++;                   //doses bebidas por cada convidado
//         bebida=0;
//         vazio.notify_one();
//         return true;
//     }

//     void fim() {
//         unique_lock<mutex> lck(mux);
//         vazio.wait(lck,[this]{return bebida == 0;});
//         bebida=-1;
//         cheio.notify_all();
//     }

int cont[3] = {0,0,0};
    void resultados(int numDoses) {
        /* Como o monitor não deve dar preferência para nenhum dos convidados,
        o esperado é que cada convidado beba aproximadamente:
        - convidado 1: 1/3 * 1/3 =                   11% do total
        - convidado 2: 1/3 * 1/3 + 1/3 * 1/2 =       28% do total
        - convidado 3: 1/3 * 1/3 + 1/3 * 1/2 + 1/3 = 61% do total  */
        printf("%d doses servidas\n", numDoses);
        int soma=0;
        for (int i=0;i<3;++i){
            printf("%d bebeu %d doses (%.1f%%)\n",i+1,cont[i],(100.0*cont[i]/numDoses));
            soma+=cont[i];
        }
        if(100.0*soma/numDoses != 100) abort();
        printf("%.0f%% das doses foram consumidas\n",(100.0*soma/numDoses));
        printf("\n\n\n");
    }
// };

// MonitorFesta bebida;


sem_t naoCheio, naoVazio;
int numDoses = 1000;

int bebida = 0;

sem_t mux,fim_de_festa;

void garcon(int n) {
    for(int i=0; i<n; ++i) {
        int b=rand()%3+1;           //busca uma bebida aleatória
        sem_wait(&naoCheio);
        // sem_wait(&mux);
        //cuidado, região critica
            // cout << "Servindo " << b << "\n";
            bebida = b;
        // sem_post(&mux);
        sem_post(&naoVazio);
        // bebida.serve(b);            //serve bebida aleatória
    }
    sem_wait(&naoCheio);
    bebida = -1;
    sem_post(&naoVazio);
    // bebida.fim();                   //mandar convidados embora
}

void convidado(int id) {
    while (true) {
        sem_wait(&naoVazio);
        sem_wait(&mux);
            if(bebida == -1){   //se nao tem mais bebidas
                sem_post(&naoVazio);    //avisa aos proximos convidados
                sem_post(&mux);         //termina o trecho de exclusão mutua aqui e encerra essa thread
                break;
            }
            if(id >= bebida ){
                cont[id-1]++;
                bebida = 0;
                // cout << id << " bebendo " << bebida << "\n";
            } 
            
            else{                           //se nao pode beber a bebida, acorda outro convidado, passa a vez e encerra a exclusao
                sem_post(&mux);
                sem_post(&naoVazio);
                continue;
            }
            
        sem_post(&mux);
        
        sem_post(&naoCheio);
    }
}

void testa() {
    sem_init(&naoCheio,0,1);
    sem_init(&naoVazio,0,0);
    sem_init(&mux,0,1);
    

    vector<thread> threads;
    threads.push_back(thread(garcon,numDoses));
    for(int i=1; i<=3; ++i)
        threads.push_back(thread(convidado,i));
    
    for(thread &t : threads)
        t.join();

    sem_destroy(&naoCheio);
    sem_destroy(&naoVazio);
    sem_destroy(&mux);


    resultados(numDoses);
}

int main(){
    for(int i =0;i<10000;i++){
        testa();
        // numDoses = 100;
        bebida = 0;
        cont[0] = 0;cont[1] = 0; cont[2] = 0;
    }
}