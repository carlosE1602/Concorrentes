#include <omp.h>
#include <iostream>

using namespace std;

int main(){
    int A[3][3] = { {2,-3, 0},
                    {4,-5, 1},
                    {2,-1, -3}};

    int b[3] = {3,1,0};

    int row = 3, col = 3, n = 3;

    int x[3];
    // #pragma omp parallel for
    for (row = n-1; row >= 0; row--) {
        x[row] = b[row];
        // #pragma omp parallel for
        for (col = row+1; col < n; col++)
            x[row] -= A[row][col] * x[col];
        x[row] /= A[row][row];
    }

    for(int i = 0;i<n;i++) cout << x[i] << " ";
    cout << "\n";
    return 0;
}

