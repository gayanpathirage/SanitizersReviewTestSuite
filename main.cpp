/*
This app is writen to compare different memory/thread error detection tools by
simulating various errors.

More info can be found in following confluence link

Author : gayancp
*/
#include <stdio.h>
#include <iostream>
#include <cstdlib>
#include <list>
#include <sys/time.h>

int global_array[100] = { -1 };
int global_array2[10] = { -1 }; //Space to corrupt

int useAfterFreeRead()
{
	printf("Testing Use after free UAF read \n");
	int* array = new int[100];
	array[0] = 0;
	delete[] array;
	return array[0]; //Read after delete
}

void useAfterFreeWrite()
{
	printf("Testing Use after free UAF write \n");
	int* array = new int[100];
	array[0] = 0;
	delete[] array;
	array[0] = 1; //Write after delete
}

int heapOutOfBoundRead()
{
	printf("Testing heap out of bound read \n");
	int* array = new int[100];
	array[0] = 0;
	int res = array[100]; //Out-of-bound index
	delete[] array;
	return res;
}

void heapOutOfBoundWrite()
{
	printf("Testing heap out of bound write \n");
	int* array = new int[100];
	int* array2 = new int[200]; //Keeping space for Corruption
	array2[0] = -1;
	array[100] = 100; //Out-of-bound index
	delete[] array;
	delete[] array2;
}

void stackOutOfBoundRead()
{
	printf("Testing stack out of bound read");
	char stack_array[100];
	stack_array[0] = '\0';
	printf("100 %c - %s \n", stack_array[100], stack_array);  //Out-of-bound index
}

void stackOutOfBoundReadLarge()
{
	printf("Testing stack out of bound read large");
	char stack_array[100];
	stack_array[0] = '\0';
	printf("100 %c - %s \n", stack_array[100000], stack_array);  //Out-of-bound index
}

void stackOutOfBoundReadInteger()
{
	printf("Testing stack out of bound read integer");
	int stack_array[100];
	stack_array[0] = 0;
	printf("100 %d \n", stack_array[100]);  //Out-of-bound index
}

void stackOutOfBoundWrite()
{
	printf("Testing stack out of bound write ");
	char stack_array[100];
	int i = 33; //should corrupt this
	stack_array[100] = '\0'; //Out-of-bound index
	printf("i : %d  %s\n", i, stack_array);
}

void stackOutOfBoundWriteLarge()
{
	printf("Testing stack out of bound write large");
	char stack_array[100];
	stack_array[100000] = '\0'; //Out-of-bound index
	printf(" %s \n", stack_array);
}

void stackOutOfBoundWriteInteger()
{
	printf("Testing stack out of bound write integer");
	int stack_array[100];
	int i = 33; //should corrupt this
	stack_array[100] = 0; //Out-of-bound index
	printf("i : %d %d \n", i, stack_array[0]);
}

int globalOutOfBoundRead()
{
	printf("Testing global out of bound read \n");
	return global_array[100]; //Out-of-bound global access
}

void globalOutOfBoundWrite()
{
	printf("Testing global out of bound write \n");
	global_array[100] = 0; //Out-of-bound global access
}

int *ptr;
__attribute__((noinline))
void functionThatEscapesLocalObject() 
{
	int local[100];
	local[0] = 1;
	ptr = &local[0];
}

int useAfterReturn()
{
	printf("Testing use after return \n");
	functionThatEscapesLocalObject();
	return ptr[0]; //Accessing function stack pointer outside.
}

void memoryLeakSimple()
{
	printf("Testing simple memory leak \n");
	int* array = new int[100];
	for (int i = 0; i < 100; i++) //Initialize
		array[i] = 0;
}

void memoryLeakIndirectly()
{
	printf("Testing indirect memory leak \n");
	int* array = new int[100];
	for (int i = 0; i < 100; i++) //Initialize
		array[i] = 0;
	array = new int[10]; //Re-define array, causing the leak
	delete[] array;
}

void useOfUninitialized(int x)
{
	printf("Testing use of uninitialized stack : ");
	printf("x is %d\n", x);
}

void useOfUninitializedHeap(char* zBuf)
{
	printf("Testing use of uninitialized heap array : ");
	printf("Char* is [%s] \n", zBuf);
}

void conditionalUseOfUninitialized(int x)
{
	printf("Testing conditional use of uninitialized stack value >>> ");
	if (x < 1)
		printf("condition (x < 1) met based on uninitialized value : %d \n", x);
	else
		printf("condition (x < 1) not met based on uninitialized value : %d \n", x);
}

void conditionalUseOfUninitializedHeap(int x)
{
	printf("Testing conditional use of uninitialized heap value >>> ");
	if (x < 1)
		printf("condition (x < 1) met based on uninitialized value : %d \n", x);
	else
		printf("condition (x < 1) not met based on uninitialized value : %d \n", x);
}

void charBufferOverflow(char* zBuf/*Input someting more than 2 chars long*/)
{
	printf("Testing buffer overflow \n");
	char* zHeapBuf = new char[2];
	zHeapBuf = zBuf;
	delete[] zHeapBuf;
}

void invalidFree()
{
	printf("Testing invalid free \n");
	int* i = new int[10];
	delete[] i;
	delete[] i; //Double deletion 
}

void mismatchedFree()
{
	printf("Testing mismatched free \n");
	int* i = new int[10];
	void* p = (void*)i;
	delete p; //Array delete should be here
}

void speedTest()
{
	int big_array_size = 1024000;
	int char_array_size = 1024;
	printf("Testing speed : ");
	struct timeval tim;
	gettimeofday(&tim, NULL);
	double t1 = tim.tv_sec + (tim.tv_usec / 1000000.0);
	//printf("Start: %.61f ", t1);

	//Allocate set of large buffers
	char** zBuf = new char*[big_array_size];
	for (int i = 0; i < big_array_size; ++i)
	{
		char* zSubBuf = new char[char_array_size];
		zSubBuf[0] = '\0';
		zBuf[i] = zSubBuf;
	}

	//Delete array
	for (int i = 0; i < big_array_size; ++i)
		delete[] zBuf[i];

	delete[] zBuf;

	gettimeofday(&tim, NULL);
	double t2 = tim.tv_sec + (tim.tv_usec / 1000000.0);
	//printf("Stop: %.61f ", t2);
	printf("%.6lf seconds elapsed\n", t2 - t1);

}

void threadRaiseTest()
{
	
}

int heapOutOfBoundReadLarge()
{
	printf("Testing heap out of bound read large\n");
	int* array = new int[100];
	array[0] = 0;
	int res = array[100000]; //Large Out-of-bound index
	delete[] array;
	return res;
}

void heapOutOfBoundWriteLarge()
{
	printf("Testing heap out of bound write large\n");
	int* array = new int[100];
	int* array2 = new int[200000]; //Keeping space for Corruption
	array2[0] = -1;
	array[100000] = 100; //Large Out-of-bound index
	delete[] array;
	delete[] array2;
}

#define TOTAL_TESTS 26
void generateTestList(int argc, char** argv, std::list<int>* lst_Tests)
{
	for (int n = 1; n <= TOTAL_TESTS; ++n)
	{
		if (argc == 1) //Run all tests
		{
			lst_Tests->push_back(n);
		}		
		else if (argc == 2) //Start runing tests from given number e.g. 6
		{
			if (n >= atoi(argv[1]))
				lst_Tests->push_back(n);
		}
		else if (argc == 3) //Run tests for given range e.g. 5 10
		{
			if (n >= atoi(argv[1]) && n <= atoi(argv[2]))
				lst_Tests->push_back(n);
		}
		else //Run only specified tests e.g. 3 5 6 9 12
		{
			for (int c = 1; c < argc; ++c)
			{
				if (n == atoi(argv[c]))
					lst_Tests->push_back(n);
			}
		}
	}
}

int main(int argc, char** argv)
{
	if (argc == 2 && atoi(argv[1]) == 0) //Return number of tests 
		return TOTAL_TESTS;

	std::list<int> lst_Tests;
	generateTestList(argc, argv, &lst_Tests);

	lst_Tests.sort();
	std::list<int>::iterator ite = lst_Tests.begin();
	for (; ite != lst_Tests.end(); ++ite)
	{
		printf("Executing : %d : ", *ite);
		switch(*ite)
		{
			case 1:  { useAfterFreeRead(); } break;
			case 2:  { useAfterFreeWrite(); } break;
			case 3:  { heapOutOfBoundRead(); } break;
			case 4:  { heapOutOfBoundWrite(); } break;
			case 5:  { stackOutOfBoundRead(); } break;
			case 6:  { stackOutOfBoundWrite(); } break;
			case 7:  { globalOutOfBoundRead(); } break;
			case 8:  { globalOutOfBoundWrite(); } break;
			case 9:	 { useAfterReturn(); } break;
			case 10: { memoryLeakSimple(); } break;
			case 11: { memoryLeakIndirectly(); } break;
			case 12: { 	
				int x;
					useOfUninitialized(x);
			} break;
			case 13: { 	
				char* zBuf = new char[10];
				useOfUninitializedHeap(zBuf);
				delete[] zBuf; 
			} break;
			case 14: { 	
				int x;
				conditionalUseOfUninitialized(x); 
			} break;
			case 15: {
				int* y = new int[10];
				conditionalUseOfUninitializedHeap(y[0]);
				delete[] y; 
			} break;
			case 16: {
				char* zLenBuf = new char[5];
				for (int i = 0; i < 5; i++)
					zLenBuf[i] = i;
				charBufferOverflow(zLenBuf);
			} break;
			case 17: { invalidFree();} break;
			case 18: { mismatchedFree();} break;
			case 19: { speedTest(); } break;
			case 20: { threadRaiseTest(); } break;
			case 21: { heapOutOfBoundReadLarge(); } break;			
			case 22: { heapOutOfBoundWriteLarge(); } break;
			case 23: { stackOutOfBoundReadLarge(); } break;
			case 24: { stackOutOfBoundWriteLarge();	} break;
			case 25: { stackOutOfBoundReadInteger(); } break;
			case 26: { stackOutOfBoundWriteInteger(); } break;
			default:
					 {
						 printf("No test written yet for %d \n", *ite);
					 }
		}
	}

	return 0;
}


