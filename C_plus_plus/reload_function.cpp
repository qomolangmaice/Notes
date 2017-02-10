#include <iostream>
using namespace std;

int add(int a, int b);
float add(float x, float y);

int main(int argc, char *argv[])
{
    int a, b;
    float x, y;
    
    cout << "Enter interger a, b: " << endl;
    cin >> a >> b;
    cout << "a + b = " << add(a, b) << endl;
    
    cout << "Enter float x, y: " << endl;
    cin >> x >> y;
    cout << "x + y = " << add(x, y) << endl;
    
    return 0;
}

int add(int a, int b)
{
    return a + b;
}

float add(float x, float y)
{
    return x + y;
}
