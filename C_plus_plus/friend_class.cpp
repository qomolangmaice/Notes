// 类也可以声明为另一个类的友元，就像友元函数那样，这个作为另一个类的友元的类就叫做友元类。
// 如果一个类B是类A的友元类，则类B的所有成员函数都是类A的友元函数，都能访问类A的私有成员和保护成员。

class A
{
public:
    void Display() { cout << x << endl; }
    friend class B;

private:
    int x;
};

class B
{
public:
    void Set(int i);
    void Display();

private:
    A a;
};

void B::Set(int i)
{
    a.x = i;    // 因为类B是类A的友元类，所以类B的成员函数可以访问类A对象的私有成员
}

void B::Display()
{
    a.Display();
}
