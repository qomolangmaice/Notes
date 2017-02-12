// 友元函数是在类的声明中用关键字friend修饰的普通函数或者其他类的成员函数。
// 友元函数虽不是本类的成员函数，但在它的函数体中可以访问该类对象的私有成员和保护成员。
#include <iostream>
using namespace std;

class Data
{
public: // 外部接口
    Data(int xx) { x = xx; }
    int GetX(void) { return x; }
    friend int Add(Data & a, Data & b);

private:    // 私有数据成员
    int x;  
};

int Add(Data & a, Data & b)
{
    return a.x + b.x;
}

int main(int argc, char *argv[])
{
    Data a(1);
    Data b(2);
    int sum = Add(a, b);

    cout << "The sum is: " << sum << endl;

    return 0;
}
