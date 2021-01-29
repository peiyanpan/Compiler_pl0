/*解释程序*/
#include<iostream>
#include<fstream>
using namespace std;
fstream infile_second;
struct Instruction
{
	string instruction;// 操作指令
	int len;//层差
	int par;
};
int Stack[1000];
/*根据层差，寻找非局部变量*/
int getBase(int SP, int lev) // 静态链思想，lev为层差
{
	int oldSP = SP;
	while (lev > 0)
	{
		oldSP = Stack[oldSP + 1];
		lev--;
	}
	return oldSP;
}
/*翻译过程*/
int translator() // 翻译pcode
{
	Instruction ins[200];//所有的指令序列
	infile_second.open("out.txt");
	int index = 0;
	while (!infile_second.eof())
	{
		Instruction i;
		infile_second >> i.instruction >> i.len >> i.par;
		ins[index] = i;
		index++;
	}
	/*for (int i = 0; i < index; i++)
		cout << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;*/
	//cout << index;
	/*The length of interpretor is index*/
	int i = 0; // 指令寄存器
	int P = 0; // 存放下一条要执行的指令地址
	int B = 0; // 基地址寄存器
	int Stack_index = 0; //栈顶指示器寄存器
	int temp;
	do
	{
		i = P;
		P++;
		if (ins[i].instruction == "lit")//取常量a放入栈顶
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = ins[i].par;
			Stack_index++;
		}
		else if(ins[i].instruction == "opr") //执行运算,a代表某种运算
		{
			//cout << ins[i].instruction << endl;
			if (ins[i].par == 0)// 过程调用结束后,返回调用点并退栈
			{
				Stack_index = B;
				P = Stack[B + 2]; // 下一条要执行的指令地址
				B = Stack[B];
			}
			else if (ins[i].par == 1)// 取反
			{
				Stack[Stack_index - 1] = Stack[Stack_index - 1] * (-1);
			}
			else if (ins[i].par == 2)// +
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 1] + Stack[Stack_index - 2];
				Stack_index--;
			}
			else if (ins[i].par == 3)//-
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] - Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 4)// *
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] * Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 5)// /
			{
				Stack[Stack_index - 2] = Stack[Stack_index - 2] / Stack[Stack_index - 1];
				Stack_index--;
			}
			else if (ins[i].par == 6)// 判断奇偶性
			{
				Stack[Stack_index - 1] = Stack[Stack_index - 1] % 2;
			}
			else if (ins[i].par == 7)
			{

			}
			else if (ins[i].par == 8)// 判断栈顶和次栈顶是否相等
			{
				if (Stack[Stack_index - 2] == Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 9)// 判断栈顶和次栈顶是否不等
			{
				if (Stack[Stack_index - 2] != Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 10)// 次栈顶是否小于栈顶
			{
				if (Stack[Stack_index - 2] < Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 11)// 次栈顶是否大于等于栈顶
			{
				if (Stack[Stack_index - 2] >= Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 12)// 次栈顶是否大于栈顶
			{
				if (Stack[Stack_index - 2] > Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 13)// 次栈顶是否小于等于栈顶
			{
				if (Stack[Stack_index - 2] <= Stack[Stack_index - 1])
				{
					Stack[Stack_index - 2] = 1;
					Stack_index--;
				}
				else
				{
					Stack[Stack_index - 2] = 0;
					Stack_index--;
				}
			}
			else if (ins[i].par == 14)// 栈顶输出
			{
				cout << Stack[Stack_index - 1] << endl;
			}
			else if (ins[i].par == 15)// 输出换行
			{
				cout << endl;
			}
			else if (ins[i].par == 16)// 命令行读入一个输入置于栈顶
			{
				int temp;
				cin >> temp;
				Stack[Stack_index] = temp;
				Stack_index++;
			}
		}
		else if (ins[i].instruction == "lod")// 取变量(相对地址为a,层差为L)放在数据栈的栈顶
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = Stack[ins[i].par + getBase(B, ins[i].len)];
			Stack_index++;
		}
		else if (ins[i].instruction == "sto")// 将数据栈栈顶的内容存入变量
		{
			//cout << ins[i].instruction << endl;
			Stack[ins[i].par + getBase(B, ins[i].len)] = Stack[Stack_index - 1];
			Stack_index--;
		}
		else if (ins[i].instruction == "cal")// 调用过程
		{
			//cout << ins[i].instruction << endl;
			Stack[Stack_index] = B;//静态链，直接外层过程
			Stack[Stack_index + 1] = getBase(B, ins[i].len);	//动态链，调用前运行过程
			Stack[Stack_index + 2] = P;		//返回地址，下一条要执行的
			B = Stack_index;
			P = ins[i].par;
		}
		else if (ins[i].instruction == "int") // 数据栈栈顶指针增加 a
		{
			//cout << ins[i].instruction << endl;
			Stack_index = Stack_index + ins[i].par;
		}
		else if (ins[i].instruction == "jmp")//无条件转移到地址为a的指令
		{
			//cout << ins[i].instruction << endl;
			P = ins[i].par;
		}
		else if (ins[i].instruction == "jpc")// 有条件跳转到地址为a的指令,只有当循环条件为假的时候才跳出
		{
			//cout << ins[i].instruction << endl;
			if (Stack[Stack_index - 1] == 0) P = ins[i].par;
		}
		else if (ins[i].instruction == "red")// 命令行读入一个置于栈顶
		{
			//cout << ins[i].instruction << endl;
			cin >> temp;
			Stack[ins[i].par + getBase(B, ins[i].len)] = temp;
		}
		else if (ins[i].instruction == "wrt")// 栈顶输出到屏幕上
		{
			//cout << ins[i].instruction << endl;
			cout << Stack[Stack_index - 1] << " ";
		}
	} while (P != 0);
	return 0;
}