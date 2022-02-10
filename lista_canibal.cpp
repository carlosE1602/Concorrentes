#include<iostream>
#include<thread>
#include<vector>
#include<mutex>
#include<semaphore.h>

using namespace std;

#define numCanibais 20

sem_t temCarne, semCarne, mux, qtdCarne;

//variaveis compartilhadas
int missionarios = 1000;
int totalDisponivel = 0;
int cont[numCanibais] = {}; //vetor usado para conferir se a soma do que os canibais comeram e igual ao total de carnes


void cozinheiro(){
    while(missionarios > 0){ //posso verificar sem exclusao mutua, pois apenas o cozinheiro acessa ela, 
        int porcoes = min(numCanibais,missionarios); //a cada rodada o cozinheiro conzinha 1 missionario para cada canibal(ou o maximo possivel se for menor), mas nao necessariamente
                                                    //cada um vai comer uma vez
        sem_wait(&semCarne);        //espera um canibal sinalizar que nao tem carne, sempre avisa no inicio

        for(int i =0;i<porcoes;i++){
            totalDisponivel++;  //posso fazer isso pois cozinheiros e canibais nunca acessam essa variavel juntos
            missionarios--;
        }

        sem_post(&temCarne); //notifica os canibais que tem carne
    }
}

void canibal(int id){
    while(true){ //permite uma thread comer mais de uma vez em seguida

        sem_wait(&mux); //exclusao mutua, todas as variaveis acessadas sao compartilhadas entre as threads canibais

            if(totalDisponivel == 0){   //testa se nao tem canibal
                if(missionarios == 0) {sem_post(&mux); return;} //se acabaram os missionarios sinaliza fim da exclusao mutua e encerra 
                sem_post(&semCarne);    //avisa que nao tem carne
                sem_wait(&temCarne);    //espera ter carne
            }
                totalDisponivel--;  //contabiliza como comido
                cont[id]++;         //incrementa para conferir

        sem_post(&mux);
    }
}

void confere(int totalMissionarios){
    cout << "Conferindo os resultados\n";


    int soma = 0;

    for(int i = 0;i<numCanibais;i++){
        soma+=cont[i];
        cout << i << " comeu " << cont[i] << " missionarios\n";
    }

    cout << "Os canibais comeram " << soma << " missionarios\n";
    if(soma != totalMissionarios) cout << "Porem, no inicio tinham " << totalMissionarios << "\n";
}


int main(){

    sem_init(&semCarne,0,0); //semaforo que controla o cozinheiro, comeca com 0, esperando um canibal sinalizar a falta de carne
    sem_init(&temCarne,0,0); //semaforo que controlam os canbais, inicialmente 0, nao tem carne
    sem_init(&mux,0,1);     //semaforo que faz papel de mutex

    int totalMissionarios = missionarios;

    thread c(cozinheiro);

    vector<thread> canibais;

    for(int i = 0;i < numCanibais;i++ ){
        canibais.push_back(thread(canibal,i));
    }

    c.join();

    for(auto &i:canibais) i.join();

    sem_destroy(&semCarne);
    sem_destroy(&temCarne);
    sem_destroy(&mux);

    confere(totalMissionarios);


    return 0;

}