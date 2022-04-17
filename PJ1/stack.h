#pragma once
#include<stdio.h>
class stack;
class stacknode 
{
	friend class stack;
private:
	char* ele;
	stacknode* link;
public:
	stacknode(char *e=NULL,stacknode* next=NULL):
		ele(e),link(next){}
};
class stack
{
private:
	stacknode* top;
public:
	stack():top(NULL){}
	~stack()
	{
		stacknode* p;
		while (top)
		{
			p = top;
			top = top->link;
			delete p;
		}
	};
	int isempty() { return top == NULL; }
	void push(char* x)
	{
		top = new stacknode(x, top);
	}
	int pop(char*& x)
	{
		if (isempty())
			return 0;
		stacknode* p;
		x = p->ele; top = top->link;
		delete p;
		return 1;
	}
	int gettop(char*& x)
	{
		if (isempty())
			return 0;
		x = top->ele;
		return 1;
	}
};