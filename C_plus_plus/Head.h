#ifndef __HEAD_H__
#define __HEAD_H__

// 组合的概念
class Eye
{
public:
	void look(void);
};

class Ear
{
public:
	void listen(void);
};

class Nose
{
public:
	void smell(void);
};

class Mouth
{
public:
	void eat(void);
};

// 若在逻辑上A 是B 的“一部分”（a part of），则不允许B 继承A 的功能，而是要用A和其它东西组合出B。
// 例如眼（Eye）、鼻（Nose）、口（Mouth）、耳（Ear）是头（Head）的一部分，
// 所以类Head 应该由类Eye、Nose、Mouth、Ear 组合而成，不是派生而成。示例程序如下：
class Head
{
public:
	void Look(void)
	{
		m_eye.look();
	}
	void Listen(void)
	{
		m_ear.listen();
	}
	void Smell(void)
	{
		m_nose.smell();
	}
	void Eat(void)
	{
		m_mouth.eat();
	}

private:
	Eye m_eye;
	Ear m_ear;
	Nose m_nose;
	Mouth m_mouth;
};


#endif 
