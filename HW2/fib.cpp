#include <iostream>
#include <string>
#include <vector>

using namespace std;

template< class T > // Template of class Fib
class Fib {
#define _PB push_back
    private:
        const static int ub=90;  // upper-bound of terms of Fib
        vector< T > data;       // Use vector to store the result
    public:
        T fetch(int n) {        // input i, output i-th term of Fib
            return (n>ub||n<0)?-1:data[n]; // boundary check
        }
        vector< T > fetch(int from, int ends) { // Just like C++ programming-style, return [from, ends) terms.
            if(ends>ub) ends=ub; //Boundary check
            if(from<0) from=0;
            return vector< T > ( data.begin()+from, data.begin()+ends );
        }
        vector< T > fetch() {   // fetch all content
            return vector< T > (data.begin(), data.end());
        }
        Fib(T a, T b) { // Constructor of class Fib
            data._PB(a); // Initial values
            data._PB(b);
            for(int i=data.size(); i<=ub; ++i) {
                data._PB( *(data.end()-1)+*(data.end()-2) ); // $F_{n} = F_{n-1} + F_{n-2}$
            }
        }
#undef _PB
};

int main(void) {
    Fib<long long int> fibObj(0LL,1LL); // Use long long int to prevent overflow
    // Print all terms.
    for(int i=0; i<=90; ++i) cout<<"Fib_"<<i<<":\t"<<fibObj.fetch(i)<<endl;
    return 0;
}
