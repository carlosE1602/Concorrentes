#include <iostream>
#include <cmath>
#include <set>
#include <omp.h>
#include <algorithm>

using namespace std;

// 1 a N

int main() {
    int n, root, count = 0;
    set <int> primes;

    cin >> n;

    #pragma omp parallel for default(none) private(count) shared(n, primes, cout)
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
            primes.insert(i);
        }

        count = 0;
    }
    
    for(auto i : primes)
        cout << i << ' ';

    cout << '\n';

    return 0;
}