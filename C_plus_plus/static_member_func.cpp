#include <iostream>
using namespace std;

class CStudent
{
public:
    CStudent(int nID)
    {
        m_nID = nID;
        m_nCount++;
    }
    CStudent(CStudent & s);

    int GetID(void)
    {
        return m_nID;
    }
    static void GetCount(void)  // 静态成员函数, 输出静态数据成员
    {
        cout << "学生人数为: " << m_nCount << endl;
    }

private:
    int m_nID;
    static int m_nCount;    // 静态数据成员的引用性说明
};

//  静态成员的好处就是内存中只有一份拷贝，可以直接通过类名访问。
int CStudent::m_nCount = 0; // 静态数据成员的定义性说明以及初始化

CStudent::CStudent(CStudent & s)
{
    m_nID = s.m_nID;
    m_nCount++;
}

int main(int argc, char *argv[])
{
    CStudent A(6);
    cout << "学生ID：" << A.GetID();
    A.GetCount();

    CStudent B(A);
    cout << "学生ID：" << B.GetID();
    CStudent::GetCount();

    return 0;
}
