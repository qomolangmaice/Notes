#include <iostream>
using namespace std;

class CStudent
{
public:
    CStudent()  // 无餐的构造函数
    {   
        cout << "Default constructor called." << endl;
    }
    CStudent(int nAge)  // 带参的构造函数
    {
        m_nAge = nAge;
        cout << "Constructor called." << endl;
    }
    ~CStudent() // 析构函数
    {
        cout << "Destructor called." << endl;
    }
    int GetAge() { return m_nAge; }

private:
    int m_nAge;
};

int main(int argc, char *argv[])
{
    CStudent *p = new CStudent; // 动态创建对象，没有给出初值列表，所以无参数的构造函数
    delete p;   // 删除对象，自动调用析构函数
    p = new CStudent(16);   // 动态创建对象,给出了初值，所以调用带参数的构造函数
    delete p;   // 删除对象，自动调用析构函数

    CStudent *q = new CStudent[2];  // 动态创建对象数组，为每个对象元素调用无参的构造函数
    delete [] q;    // 删除对象数组，自动为每个对象调用析构函数

    return 0;
}
