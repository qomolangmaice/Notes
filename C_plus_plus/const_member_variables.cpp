#include <iostream>
using namespace std;

class A
{
public:
    A(int i);
    void print();
    const int & r;

private:
    const int a;
    static const int b; // 静态常数据成员
};

const int A::b = 20;

A::A(int i):a(i), r(a)
{

}

void A::print()
{
    cout << a << ":" << b << ":" << r << endl;
}

int main(int argc, char *argv[])
{
    A a1(50), a2(10);
    a1.print();
    a2.print();

    return 0;
}
