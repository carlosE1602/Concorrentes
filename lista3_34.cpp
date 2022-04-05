#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    size_t N = 5;
    int A[N][N] = { {1,1, 1,1,1},
                    {0,1,1,1,1},
                    {0,0,1,1,1},
                    {0,0,0,1,0},
                    {0,0,0,0,1}};

    int b[N] = {5,4,3,2,0};

    int row = N, col = N, n = N;

    int x[N];
    // #pragma omp parallel for
    for (row = n-1; row >= 0; row--) {
        x[row] = b[row];
        int aux = x[row];
        #pragma omp shared(A,x,n,row) parallel for reduction(-:aux)
        for (col = row+1; col < n; col++)
            aux -= A[row][col] * x[col];
        // x[row] = aux;    
        x[row] = aux/A[row][row];
    }

    for(int i = 0;i<n;i++) cout << x[i] << " ";
    cout << "\n";
    return 0;
}

