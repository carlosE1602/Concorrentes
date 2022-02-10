#include <iostream>
#include <unistd.h>

int main(){
//    if(fork() == 0)
//        std::cout << "processo filho\n";
//    else std::cout << "processo pai\n";

    fork();
    fork();
    fork();
    std::cout <<"hello\n";
    return 0;
}
