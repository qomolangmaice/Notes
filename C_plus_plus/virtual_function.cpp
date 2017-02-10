class A
{
public:
	virtual void Func1(void)
	{
		std::cout << "A - Func1()" << std::endl;
	}
};

class B : public A
{
public:
	virtual void Func1(void)
	{
		std::cout << "B - Func1()" << std::endl;
	}
};

class C : public A
{
public:
	virtual void Func1(void)
	{
		std::cout << "C - Func1()" << std::endl;
	}
};

void Test(A *a)
{
	a->Func1();
}

int main(int argc, char *argv[])
{
	A a;
	B b;
	C c;

	Test(&a);
	Test(&b);
	Test(&c);

	for(;;);

	return 0;
}
