#include <iostream>
using namespace std;

template <typename T>
T abs(T x)
{
    return x < 0 ? -x : x;
}

int main(int argc, char *argv[])
{
    int a = -1;
    float b = -10.1;
    
    cout << "a = " << abs(a) << endl;
    cout << "b = " << abs(b) << endl;
    
    return 0;
}
