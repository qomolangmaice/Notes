#include <iostream>

class A
{
public:
	void Func1(void)
	{
		std::cout << "A - Func1()" << std::endl;
	}
	void Func2(void)
	{
		std::cout << "A - Func2()" << std::endl;
	}
};

class B: public A
{
public:
	void Func3(void)
	{
		std::cout << "B - Func3()" << std::endl;
	}
	void Func4(void)
	{
		std::cout << "B - Func4()" << std::endl;
	}
};

int main(int argc, char *argv[])
{
	B b;
	b.Func1();
	b.Func2();
	b.Func3();
	b.Func4();

	return 0;
}
