#include <iostream>
using namespace std;

void show(const double & r);

int main(int argc, char *argv[])
{
    double d(9.5);
    show(d);

    return 0;
}

void show(const double & r)
{
    // r = 10.0;    // read-only error
    cout << r << endl;
}
