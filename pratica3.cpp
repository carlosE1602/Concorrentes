#include <mutex>
#include <iostream>
#include <condition_variable>
#include <thread>
#include <unistd.h>
#include <vector>
#include <time.h>

using namespace std;
//total de bebidas de cada tipo
#define beb1 10
#define beb2 20
#define beb3 30

class monitorFesta{
    private:
    int *numBebidas;
    int cont[3] = {0,0,0};
    int copo; //0 vazio, 1 cheio
    int tipo; //tipo = i, bebida do tipo i
    mutex mux;
    condition_variable cheio, vazio; //estado do copo
    bool semBebidas,ultimaDose;

    public:

        void printBebidas(){
            for(int i = 0;i<3;i++){
                cout << "Bebidas " << i << " " << numBebidas[i] <<"\n";
            }
        }

        monitorFesta(){
            numBebidas = new int[3];
            numBebidas[0] = beb1;
            numBebidas[1] = beb2;
            numBebidas[2] = beb3;
            copo = 0; //Inicialmente o copo esta vazio
            tipo = 0;
            ultimaDose = false;
            semBebidas = false;

        }
        ~monitorFesta(){
            delete [] numBebidas;
        }
        bool serve(int b){
            // cout << "garrei no serve\n";
            unique_lock<mutex> lck(mux);
            
            if(numBebidas[b] == 0) return true;  //esse tipo de bebida não pode ser mais servido

            vazio.wait(lck,[this]()->bool{
                return copo == 0;
            });
            usleep(1000);
            copo = 1; //copo agora esta cheio
            tipo = b;
            numBebidas[b]--;

            cout << "Servindo a bebida " << b << "\n";
            cheio.notify_all();

            if(numBebidas[0]+numBebidas[1]+numBebidas[2] == 0){
                ultimaDose = true; //notifica que acabaram as bebidas, so tem a ultima
                return false;
            }else{
                return true;
            }            
            
        }

        void bebe(int c){
            unique_lock<mutex> lck(mux);
            cheio.wait(lck, [this, c]()->bool{ //so deixa avançar se a bebida for permitida para o convidado c e se o copo esta cheio
                //so pode beber
                bool podeBeber = tipo <= c;
                return (copo == 1 and podeBeber) || semBebidas;
            });

            if(semBebidas){
                return;
            }
            
            usleep(1000);
            copo = 0;           //copo agora esta vazio
            cont[c]++;
            cout << c << " Esta bebendo a bebida " << tipo << "\n";
            cout << "Situação das bebidas no momento\n";
            printBebidas();

            if(ultimaDose){ //se todas as bebidas acabaram, notifica os outros que nao tem mais, para que a festa acabe
                semBebidas = true;
                cheio.notify_all();
                return;
            }
            vazio.notify_one();

        }


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
        printf("%.0f%% das doses foram consumidas\n",(100.0*soma/numDoses));
    }
};



monitorFesta mf;
bool acabou = false;

void garcon(){
    bool condition = true;
    while(condition){
        int b = rand()%3;
        condition = mf.serve(b);
    }
    acabou = true;
}

void convidado(int id){
    while(!acabou){
        mf.bebe(id);
    }
    cout << "a thread " << id << " acabou\n";
}

int main(){ 
    srand(time(NULL));

    thread t1(garcon);
    vector<thread> convidados;

    for(int i = 0;i<3;i++){
        convidados.push_back(thread(convidado,i)); //passa o id do convidado 0 = chato, 1 = de boa, 2 cachaceiros
    }

    t1.join();

    for(auto &t: convidados) t.join();

    cout << "Situação das bebidas ao final da festa\n";
    mf.printBebidas();
    mf.resultados(60);
}