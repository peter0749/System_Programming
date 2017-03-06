#include <iostream>
#include <string>
#include <vector>

using namespace std;

template< class T >
class Fib {
#define _PB push_back
    private:
        const static int ub=90;
        vector< T > data;
    public:
        T fetch(int n) {
            return (n>ub||n<0)?-1:data[n];
        }
        vector< T > fetch(int from, int ends) {
            if(ends>ub) ends=ub;
            if(from<0) from=0;
            return vector< T > ( data.begin()+from, data.begin()+ends );
        }
        vector< T > fetch() {
            return vector< T > (data.begin(), data.end());
        }
        Fib(T a, T b) {
            data._PB(a);
            data._PB(b);
            for(int i=data.size(); i<=ub; ++i) {
                data._PB( *(data.end()-1)+*(data.end()-2) );
            }
        }
};

int main(void) {
    Fib<long long int> fibObj(0LL,1LL);
    for(int i=0; i<=90; ++i) cout<<"Fib_"<<i<<":\t"<<fibObj.fetch(i)<<endl;
    return 0;
}
