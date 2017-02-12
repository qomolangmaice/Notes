#include <iostream>
using namespace std;

class CStudent
{
public:
    CStudent(int nID)   // 构造函数
    {
        m_nID = nID;
        m_nCount++;
    }
    CStudent(CStudent & s);     // 拷贝构造函数
    int GetID(void)
    {
        return m_nID;
    }
    void GetCount(void)
    {
        cout << "学生人数:" << m_nCount<< endl;
    }

private:
    int m_nID;
    static int m_nCount;
};

CStudent::CStudent(CStudent &s)
{
    m_nID = s.m_nID;
    m_nCount++;
}

 // 静态成员的好处就是内存中只有一份拷贝，可以直接通过类名访问。
int CStudent::m_nCount = 0;

int main(int argc, char *argv[])
{
    CStudent A(6);  // 定义对象A
    cout << "学生A," << A.GetID();
    A.GetCount();   // 输出此时学生个数
    CStudent B(A);  // 定义对象B,并用A初始化B
    cout << "学生B," << B.GetID();
    B.GetCount();

    return 0;
}
