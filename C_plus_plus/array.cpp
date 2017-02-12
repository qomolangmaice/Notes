#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    int a[5] = {0};
    int b[5] = {0};

    for (int i = 0; i < 5; i++)
    {
        a[i] = 2 * i;
        b[5 - i - 1] = a[i];
    }

    for (int j = 0; j < 5; j++)
    {
        cout << a[j] << ",";
    }

    cout << endl;

    for (int j = 0; j < 5; j++)
    {
        cout << b[j] << ",";
    }

    return 0;
}
