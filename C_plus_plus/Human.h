#ifndef __HUMAN_H__
#define __HUMAN_H__

// 继承的概念

// 若在逻辑上B 是A 的“一种”（a kind of ），则允许B 继承A 的功能。
// 如男人（Man）是人（Human）的一种，男孩（Boy）是男人的一种。
// 那么类Man 可以从类Human 派生，类Boy 可以从类Man 派生。示例程序如下
class Human
{
public:
	void think(void);
	void talk(void);
	void walk(void);
	void run(void);
	void eat(void);
	void drink(void);
	void sleep(void);
};

class Man : public Human
{
private:
	int gender;
};

class Boy : public Man
{
private:
	int age;
};

#endif
