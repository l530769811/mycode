#include <stdio.h>

#ifdef _WIN32 //wi32 platform
	#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN //for #include <windows.h> 
	#endif
	#include <windows.h>
	#define container_of CONTAINING_RECORD
#else //linux platform
	#define container_of(ptr, type, member) ({\ 
	typeof( ((type *)0)->member ) __mptr = (ptr);\ 
	(type *)( (char *)__mptr - offsetof(type,member) );})
#endif

class one
{
public:
	friend class three;
	one(int *pi):i(pi){}

	void print() const
	{
		printf("one class\n");
	}

	int ** get()
	{
		return &i;
	}
private:
	int *i;
};

class two
{
public:
		friend class three;
	two(int **ppi):i(ppi){}
	void print() const
	{
		printf("two class\n");
	
	}

	int *** get()
	{
		return &i;
	}
private:
	 int **i;
};

class three
{
public:
	three(int ***pppi):i(pppi){}
	void print()
	{
		printf("three class\n");
		two *ptwo = container_of(*i, two, i);
		ptwo->print();
		one *pone = container_of(**i, one, i);
		pone->print();
	}
private:
	int ***i;
};

int main(int argc, char **argv)
{
	int int_val = 0;
	one *pone = new one(&int_val);
	two *ptwo = new two(pone->get());
	three *pthree = new three(ptwo->get());
	pthree->print();

	delete pone;
	delete ptwo;
	delete pthree;

}