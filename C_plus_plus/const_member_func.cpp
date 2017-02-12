// 类中用const声明的成员函数就是常成员函数。
// a.常成员函数在声明和实现时都要带const关键字；
// b.常成员函数不能修改对象的数据成员，也不能访问类中没有用const声明的非常成员函数；
// c.常对象只能调用它的常成员函数，不能调用其他的普通成员函数；
// d.const关键字可以被用于参与对重载函数的区分

#include <iostream>
using namespace std;

class R
{
public:
    R(int r1, int r2) { R1 = r1; R2 = r2; }
    void print();
    void print() const;

private:
    int R1;
    int R2;
};

void R::print()
{
    cout << R1 << ":" << R2 << endl;
}

void R::print() const
{
    cout << R1 << ";" << R2 << endl;
}

int main(int argc, char *argv[])
{
    R a(4, 5);
    a.print();  // 调用print()

    const R b(20, 21);
    b.print();  // 调用print() const

    return 0;
}
