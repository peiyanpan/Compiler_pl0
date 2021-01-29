#include<fstream>
#include<iostream>
#include<string>
#include<stack>
#include<sstream> //istringstream 
using namespace std;

fstream ga_in;
fstream ga_out;

struct Unit
{
	string value; //字符串
	string key; //类型
	int row;
	int column;
};
/*符号表*/
struct SymTable
{
	string name;   //标识符名字
	int type;		//标识符类型 
	/*
		0 代表常量
		1 代表变量
		2 代表过程
	*/
	int value;		//标识符的值
	int level;		//标识符所处的层次
	int addr;    //标识符所处的相对偏移量
	int size;       //标识符大小，默认为4  若标识符为过程定义标识符，此处记录形参个数
};
SymTable symtable[100]; // 符号表数组
int tx = 0; // 符号表下标指针
int dx = 0; // 相对本过程基地址的偏移量
int lev; // 当前层次，全局变量
int number; // 存放数字
/*pcode*/
struct Instr
{
	string instruction;// 操作指令
	int len;//层差
	int par;
}; 
Instr ins[200]; // 存放pcode
int cx; // pcode的下标指针

void errortype(int type);//错误类型
void OPENFILE();
void Readline();
void factor();
void term();
void condecl();
void exp();
void lexp();
void block();
void vardecl();
void proc();
void statement();
void body();
void CLOSEFILE();

void entervar(string name, int level, int dx); // 符号表中填入变量标识符
void enterconst(string name, int level, int value, int dx); // 符号表中填入常量标识符
void enterproc(string name, int level, int dx); // 符号表中填入过程
int position_insymtable(string name); // 查找在符号表中的位置
int searchProc();
bool isPreDefine(string name, int level); // 查看标识符是否已定义，同层或外层
bool isExistDefine(string name, int level); // 是否之前已定义过，同层次的
void gen(string instruction, int l, int a);//生成pcode

Unit unit; // 当前扫描
Unit last_unit; // 上一次扫描 
int errorStack[1000] = { -1 };//存放错误个数 
int* errorType = errorStack;//指针变量 
bool error = false;
/*
	* spell error 拼写错误
	* Missing identifier 缺少标识符
	*
*/
void errortype(int type)//错误类型 
{
	error = true;
	*errorType = type;//存储当前的错误 
	errorType++;//指向下一个错误 
	switch (type)
	{
	case 0:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Spell error \"program\"" << endl;
		break;
	case 1:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"program\"" << endl;
		break;
	case 2:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "]" << "Missing \";\" after \"id\"" << endl;
		break;
	case 3:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Cannot resolve error" << endl;
		break;
	case 4:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"begin\"" << endl;
		break;
	case 5:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"end\"" << endl;
		break;
	case 6:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing assignment operation" << endl;
		break;
	case 7:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"then\"" << endl;
		break;
	case 8:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing word \"do\"" << endl;
		break;
	case 9:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier in \"call\"" << endl;
		break;
	case 10:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"call\"" << endl;
		break;
	case 11:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier in \"read\"" << endl;
		break;
	case 12:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"read\"" << endl;
		break;
	case 13:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses in \"write\"" << endl;
		break;
	case 14:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Wrong factor" << endl;
		break;
	case 15:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses" << endl;
		break;
	case 16:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"const\"" << endl;
		break;
	case 17:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing operator" << endl;
		break;
	case 18:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\" in the end" << endl;
		break;
	case 19:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing integer" << endl;
		break;
	case 20:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing the compare operator" << endl;
		break;
	case 21:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"var\"" << endl;
		break;
	case 22:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing identifier after \"procedure\"" << endl;
		break;
	case 23:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing parentheses" << endl;
		break;
	case 24:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\" in the end" << endl;
		break;
	case 25:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing comma" << endl;
		break;
	case 26:cout << "[Grammar error] " << " [" << unit.row << "," << unit.column << "] " << "Missing \";\"" << endl;
		break;
	case 27:cout << "[Grammar error] " << " [" << last_unit.row << "," << last_unit.column << "] " << "Extra semicolon \";\" before \"end\"" << endl;
		break;
	default:cout << "[Grammar ERROR] " << " [" << unit.row << "," << unit.column << "] " << "Unknown error" << endl;
		break;
	}
}
void OPENFILE()//打开文件操作 
{
	//res1:词法分析的结果,用result.txt存储语法分析的结果 
	ga_in.open("res1.txt");
	if (!ga_in)
	{
		cout << "Can't open res1.txt" << "\n";
		exit(0);
	}
	ga_out.open("out.txt");
	if (!ga_out)
	{
		cout << "Can't open out.txt" << "\n";
		exit(0);
	}
}
string line;
void Readline()//读一行内容的操作 
{
	getline(ga_in, line);
	istringstream iss(line);//从已读的一行中抽取各个元素 
	last_unit = unit;
	iss >> unit.value;//具体的单词 
	iss >> unit.key;//单词类型 
	iss >> unit.row;
	iss >> unit.column;
}
/*
	* <factor> -> <id>|<integer>|(<exp>)
*/
void factor()
{
	if (unit.key == "id")
	{
		string name = unit.value;
		int i = position_insymtable(name);
		if (symtable[i].type == 1) // 变量
			gen("lod", lev - symtable[i].level, symtable[i].addr);
		else if (symtable[i].type == 0) //常量
			gen("lit", 0, symtable[i].value);	

		Readline();
	}
	else if (unit.key == "int")
	{
		//cout << unit.value << endl;
		int sum = 0;
		for (int ii = 0; ii < unit.value.size(); ii++)
		{
			sum = sum * 10 + unit.value[ii] - '0';
		}
		gen("lit", 0, sum);
		Readline();
	}
	else if ((unit.value == "(" && unit.key == "...") || *errorType == 14)
	{
		if (*errorType == 14) errorType++;
		Readline();
		exp();
		if ((unit.value == ")" && unit.key == "...") || *errorType == 15)
		{
			if (*errorType == 15) errorType++;
			else Readline();
		}
		else errortype(15);//缺括号 
	}
	else errortype(14);//错误的factor 
}
/*
	* <term> -> <factor>{<mop><factor>}
*/
void term()
{
	factor();
	while (unit.value == "*" || unit.value == "/")
	{
		string str_temp = unit.value;
		Readline();
		factor();
		if (str_temp == "*")
		{
			gen("opr", 0, 4);
		}
		else if (str_temp == "/")
		{
			gen("opr", 0, 5);
		}
	}
}
/*
	*<condecl> -> const <const> {,<const>}
*/
void condecl()
{
	string name;
	int value;
	if (unit.key == "id" || *errorType == 16)
	{
		name = unit.value;
		if (*errorType == 16) errorType++;
		else Readline();
		if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
		{
			if (*errorType == 17) errorType++;
			else Readline();
			if (unit.key == "int" || *errorType == 19)
			{
				int sum = 0;
				for (int ii = 0; ii < unit.value.size(); ii++) // 把一个字符串转换为整数
				{
					sum = sum * 10 + unit.value[ii] - '0';
				}
				value = sum;
				enterconst(name, lev, value, dx); // 常量存入到符号表中
				//dx++;
				if (*errorType == 19)
				{
					errorType++;
					/*while (unit.value != "," && unit.value != ";")
					{
						Readline();
						if (unit.key == "id") break;//缺少
					}*/
				}
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id" || *errorType == 16)
					{
						name = unit.value;
						if (*errorType == 16) errorType++;
						else Readline();
						if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
						{
							if (*errorType == 17) errorType++;
							else Readline();
							if (unit.key == "int" || *errorType == 19)
							{					
								int sum = 0;
								for (int ii = 0; ii < unit.value.size(); ii++) // 把一个字符串转换为整数
								{
									sum = sum * 10 + unit.value[ii] - '0';
								}
								value = sum;
								enterconst(name, lev, value, dx); // 常量存入到符号表中
								// dx++;
								if (*errorType == 19)
								{
									errorType++;
									/*while (unit.value != "," && unit.value != ";")
									{
										Readline();
										if (unit.key == "id") break;//缺少 ，
									}*/
								}
								else Readline();
							}
							else errortype(19);//缺少<const>——><id>:=<integer> integer	
						}
						else errortype(17); //缺少<const>—— > <id>: = <integer> : =
					}
					else errortype(16); //缺少<const>—— > <id>: = <integer> id
				}
				if ((unit.value == ";" && unit.key == "...") || *errorType == 18)
				{
					if (*errorType == 18) errorType++;
					else Readline();
				}
				else errortype(18);//缺少分号
			}
			else errortype(19);//缺少<const>——><id>:=<integer> integer	
		}
		else errortype(17);//缺少<const>——><id>:=<integer> :=
	}
	else errortype(16);// 缺少<const>——><id>:=<integer> id
}
/*
	* <exp> -> [+|-] <term>{<aop><term>}
*/
void exp()
{
	string str_temp = unit.value;
	if (unit.value == "+" || unit.value == "-")
	{
		Readline();
	}
	term();
	if (str_temp == "-")
	{
		gen("opr", 0, 1);
	}
	while ((unit.value == "+" || unit.value == "-") && unit.key == "aop")
	{
		str_temp = unit.value;
		Readline();
		term();
		if (str_temp == "+") gen("opr", 0, 2);
		else if (str_temp == "-") gen("opr", 0, 3);
	}
}
/*
	* <lexp> -> <exp><lop><exp>   |  odd <exp>
*/
void lexp()
{
	if (unit.value == "odd" && unit.key == "keyword")
	{
		Readline();
		exp();
		gen("opr", 0, 6);
	}
	else
	{
		exp();
		if (((unit.value == "=" || unit.value == "<>" || unit.value == "<" || unit.value == "<=" || unit.value == ">" || unit.value == ">=") && unit.key == "lop") || *errorType == 20)
		{
			string str_temp = unit.value;
			if (*errorType == 20)
			{
				errorType++;
			}
			else Readline();
			exp();
			if (str_temp == "=") gen("opr", 0, 8);
			else if(str_temp == "<>") gen("opr", 0, 9);
			else if(str_temp == "<") gen("opr", 0, 10);
			else if(str_temp == "<=") gen("opr", 0, 13);
			else if(str_temp == ">") gen("opr", 0, 12);
			else if (str_temp == ">=") gen("opr", 0, 11);
		}
		else errortype(20);//缺少比较符号 
	}
}
/*
	*<block> -> [<condecl>][<vardecl>][<proc>]<body>
*/
void block()
{
	int dx0 = dx;            // 记录当前的层次，以便恢复时使用
	int tx_wait = tx + 1;        // 记录符号表当前等待填入的位置
	int n = 0;
	if (tx_wait > 1)
	{
		n = searchProc();                 //寻找过程标识符
		tx_wait = tx_wait - symtable[n].size;     //减去形参的个数，恢复时形参也不保留
	}
	if (tx_wait == 1)
		dx = 3;    // dx初始化，静态链、动态链、返回地址
	else
		dx = 3 + symtable[n].size;   // + 3 :形参置于静态链、动态链、返回地址和变量、常量之前
	int cx_temp;
	cx_temp = cx;    // 记录jmp当前的位置(当前下标)
	gen("jmp", 0, 0);
	if (unit.value == "const" && unit.key == "keyword")
	{
		Readline();
		condecl();
	}
	/*else if(unit.key == "id")
	{
		Readline();
		errortype(3);//无法判断错误类型
	}*/
	if (unit.value == "var" && unit.key == "keyword")
	{
		Readline();
		vardecl();
	}
	if (unit.value == "procedure" && unit.key == "keyword")
	{
		Readline();
		proc();
		lev--; // 出嵌套，层次 -1
	}
	if (tx_wait > 1)
	{
		int i;
		n = searchProc(); // 先寻找到该过程
		for (i = 0; i < symtable[n].size; i++) // 参数传递
		{
			gen("sto", 0, symtable[n].size + 3 - 1 - i);   // 将实参的值传给形参
		}
	}
	ins[cx_temp].par = cx;      // 回填jmp指令
	gen("int", 0, dx);        // 开辟dx个空间
	if (tx != 1)
		symtable[n].value = cx - 1 - symtable[n].size;    //将过程入口地址填入过程定义的value，以便call指令使用
	/*for (int i = 0; i <= cx; i++)
		cout << symtable[i].name << " " << symtable[i].level << endl;*/
	//cout << symtable[6].name << endl;
	body();
	gen("opr", 0, 0);
	tx = tx_wait;                       //清符号表，将符号表指针往前移至tx0处
	dx = dx0;
}
/*
	*<prog> -> program<id>;<block>
*/
void prog()
{
	Readline();
	if ((unit.value == "program" && unit.key == "keyword") || *errorType == 0)
	{
		if (*errorType == 0)  errorType++;
		Readline();
		if (unit.key == "id" || *errorType == 1)
		{
			if (*errorType == 1) errorType++;
			else Readline();
			if ((unit.value == ";" && unit.key == "...") || *errorType == 2)
			{
				if (*errorType == 2 && unit.value != ";") errorType++;
				else Readline();
				block();
			}
			else errortype(2);// 缺少 ";" 
		}
		else errortype(1);//不是id，缺少标识符
	}
	else errortype(0);//program拼写错误 
}
/*
	*<vardecl> -> var <id> {,<id>};
*/
void vardecl()
{
	string name;
	if (unit.key == "id" || *errorType == 21)
	{
		name = unit.value;
		int value;
		if (*errorType == 21) errorType++;
		else Readline();
		entervar(name, lev, dx);
		dx++;
		while ((unit.value == "," && unit.key == "...") || *errorType == 25)
		{
			if (*errorType == 25) errorType++;
			else Readline();
			if (unit.key == "id" || *errorType == 21)
			{
				if (unit.key == "id")
				{
					// cout << unit.value << endl;
					name = unit.value;
					entervar(name, lev, dx);
					dx++;
				}
				if (*errorType == 21) errorType++;
				else Readline();
			}
			else errortype(21); //缺少id 
		}
		if (unit.key == "id") errortype(25);//缺少逗号 
		if ((unit.value == ";" && unit.key == "...") || *errorType == 26) //缺少分号
		{
			if (*errorType == 26) errorType++;
			else Readline();
		}
		else errortype(26);
	}
	else errortype(21);
}
/*
	* <proc> -> procedure <id> ([<id>{,<id>}]);<block>{;<proc>}
*/
void proc()
{
	int count = 0; // 记录形参个数
	int tx_temp;
	if (unit.key == "id" || *errorType == 22)
	{
		string name = unit.value;
		if (isExistDefine(name, lev))
		{

		}
		tx_temp = tx + 1; // 符号表指针+1
		enterproc(name, lev, dx); // 将过程存入符号表
		lev++; // 层数+1
		if (*errorType == 22) errorType++;
		else Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id")
			{
				entervar(unit.value, lev, 3 + count);  
				count++;                             
				symtable[tx_temp].size = count; // 用来记录形参个数
				Readline();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					if (unit.key == "id")
					{
						entervar(unit.value, lev, 3 + count); // 将参数存入符号表 
						count++;                             // 形参个数加+1
						symtable[tx_temp].size = count; // 符号表重新赋值
						Readline();
					}
					//else errortype(22);
				}
			}
			if ((unit.value == ")" && unit.key == "...") || *errorType == 23)
			{
				if (*errorType == 23) errorType++;
				else Readline();
				if ((unit.value == ";" && unit.key == "...") || *errorType == 24)
				{
					if (*errorType == 24) errorType++;
					else Readline();
					block();
					while (unit.value == ";" && unit.key == "...")
					{
						Readline();
						proc();
					}
				}
				else errortype(24);
			}
			else errortype(23);//缺少右括号
		}
	}
	else errortype(22);//缺少id
}
/*
	*<statement> ->  <id> := <exp>
									|if <lexp> then <statement> [else <statement>]
									|while <lexp> do <statement>
									|call <id> ([<exp>{,<exp>}])
									|<body>
									|read (<id>{,<id>})
									|write(<exp>{,<exp>})
*/
void statement()
{
	if (unit.key == "id")
	{// <id> := <exp>
		string name = unit.value;
		Readline();
		if ((unit.value == ":=" && unit.key == "...") || *errorType == 6)
		{
			if (*errorType == 6) errorType++;
			else Readline();
			exp();
			/*if (!isPreDefine(name,lev))
			{
				//cout << "ok" << endl;
				return;
			}
			else
			{	*/
				//cout << name << "\n";
			int i = position_insymtable(name);
			if (symtable[i].type == 1)
			{
				gen("sto", lev - symtable[i].level, symtable[i].addr);
			}
			//}
		}
		else errortype(6);// 缺少赋值运算符 
	}
	else if (unit.value == "if" && unit.key == "keyword")
	{// if <lexp> then <statement> [else <statement>]
		int cx_temp1;
		Readline();
		lexp();
		if (unit.key == "int") Readline();
		if ((unit.value == "then" && unit.key == "keyword") || *errorType == 7)
		{
			cx_temp1 = cx;
			gen("jpc", 0, 0);
			if (*errorType == 7) errorType++;
			else Readline();
			statement();
			int cx_temp2 = cx;
			gen("jmp", 0, 0);
			ins[cx_temp1].par = cx;
			ins[cx_temp2].par = cx;
			if (unit.value == "else" && unit.key == "keyword")
			{
				Readline();
				statement();
				ins[cx_temp2].par = cx;
			}
		}
		else errortype(7); //缺少then 
	}
	else if (unit.value == "while" && unit.key == "keyword")
	{// while <lexp> do <statement>
		Readline();
		int cx_temp1 = cx;
		lexp();
		if ((unit.value == "do" && unit.key == "keyword") || *errorType == 8)
		{
			int cx_temp2 = cx;
			gen("jpc", 0, 0);
			if (*errorType == 8) errorType++;
			else Readline();
			statement();
			gen("jmp", 0, cx_temp1);
			ins[cx_temp2].par = cx;
		}
		else errortype(8);//缺少do 
	}
	else if (unit.value == "call" && unit.key == "keyword")
	{// call <id> ([<exp>{,<exp>}])
		Readline();
		int i;
		int count = 0;
		if (unit.key == "id" || *errorType == 9)
		{			
			i = position_insymtable(unit.value);
			if (symtable[i].type == 2)
			{

			}
			else
			{

			}
			if (*errorType == 9) errorType++;
			else Readline();
			if (unit.value == "(" && unit.key == "...")
			{
				Readline();
				if (unit.value == ")" && unit.key == "...")
				{
					Readline();
					gen("cal", lev - symtable[i].level, symtable[i].value);
				}
				else
				{
					exp();
					count++;
					while (unit.key == "..." && unit.value == ",")
					{
						Readline();
						exp();
						count++;
					}
					if (count != symtable[i].size)
					{

					}
					gen("cal", lev - symtable[i].level, symtable[i].value);
					if ((unit.key == "..." && unit.value == ")") || *errorType == 10)
					{
						if (*errorType == 10) errorType++;
						else Readline();
					}
					else errortype(10);
				}	
			}
		}
		else errortype(9); // 缺少call的标识符 
	}
	else if (unit.value == "read" && unit.key == "keyword")
	{// read (<id>{,<id>})
		Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id" || *errorType == 11)
			{
				if (unit.key == "id")
				{
					int i = position_insymtable(unit.value);
					if (symtable[i].type == 1)
					{
						gen("opr", 0 , 16);
						gen("sto", lev - symtable[i].level, symtable[i].addr);
					}
				}
				if (*errorType == 11) errorType++;
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id")
					{
						int i = position_insymtable(unit.value);
						if (symtable[i].type == 1)
						{
							gen("opr", 0, 16);
							gen("sto", lev - symtable[i].level, symtable[i].addr);
						}
						Readline();
					}
				}
				if ((unit.value == ")" && unit.key == "...") || *errorType == 12)
				{
					if (*errorType == 12) errorType++;
					else Readline();
				}
				else errortype(12);
			}
			else errortype(11);
		}
	}
	else if (unit.value == "write" && unit.key == "keyword")
	{// write(<exp>{,<exp>})
		Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			exp();
			gen("wrt", 0, 0);
			while (unit.value == "," && unit.key == "...")
			{
				Readline();
				exp();
				gen("wrt", 0, 0);
			}
			gen("opr", 0, 15);
			if ((unit.value == ")" && unit.key == "...") || *errorType == 13)
			{
				if (*errorType == 13) errorType++;
				else Readline();
			}
			else errortype(13);
		}
	}
	else body();
}
/*
	*  <body> -> begin <statement>{;<statement>} end
*/
void body()
{
	if ((unit.key == "keyword" && unit.value == "begin") || *errorType == 4)
	{
		if (*errorType == 4) errorType++;
		else Readline();
		statement();
		while (unit.value == ";" && unit.key == "..." && !error)
		{
			Readline();
			if (unit.value == "end" && unit.key == "keyword")
			{
				if (*errorType != 27) errortype(27);//多出一个分号
				break;
				return;
			}
			statement();
		}
		/*if(*errorType != 27)
		{
			errorType++;
			Readline();
		}*/
		if ((unit.value == "end" && unit.key == "keyword") || *errorType == 5 || error)
		{
			if (*errorType == 5) errorType++;
			Readline();
		}
		else errortype(5);//缺少end 
	}
	else errortype(4);//缺少begin 
}
void CLOSEFILE()//关闭文件操作 
{
	ga_in.close();
	ga_out.close();
}
int GAmeaning()
{
	OPENFILE();
	prog();
	CLOSEFILE();
	if (!error) cout << "无语法错误！" << endl;
	while (error)
	{
		error = false;
		errorType = errorStack;
		OPENFILE();
		prog();
		CLOSEFILE();
	}
	fstream outfile;
	outfile.open("out.txt");
	for (int i = 0; i < cx; i++)
	{
		cout << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;
		outfile << ins[i].instruction << " " << ins[i].len << " " << ins[i].par << endl;
	}
	outfile.close();
	return 0;
}
/*符号表中填入变量标识符，类型为 1 */
void entervar(string name,int level,int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 1;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*符号表中填入常量标识符*/
void enterconst(string name,int level,int value,int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 0; // 0代表常量
	symtable[tx].value = value;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*符号表中填入过程标识符*/
void enterproc(string name, int level, int dx)
{
	tx++;
	symtable[tx].name = name;
	symtable[tx].type = 2;
	symtable[tx].level = level;
	symtable[tx].addr = dx;
}
/*查找符号表中标识符的位置*/
int position_insymtable(string name)
{
	int i;
	for (i = tx; i > 0; i--)
	{
		//cout << symtable[i].name << endl;
		if (name == symtable[i].name) break;
	}
	return i;
}
/*查找过程定义的标识符*/
int searchProc()
{
	int i;
	//cout << tx << endl;
	for (i = tx; i >= 1; i--)
	{
		//cout << symtable[i].type << endl;
		if (symtable[i].type == 2)
		{
			return i;
		}
	}
	return -1;
}
/*查找标识符是否在本层或外层定义过*/
bool isPreDefine(string name, int level)
{
	int i;
	for (i = 1; i <= tx; i++)
	{
		if (symtable[i].name == name)
		{
			if (symtable[i].level == level) return true; // 查找本层的
		}
		else
		{
			do
			{
				level--;
				if (symtable[i].level == level) return true; // 查找外层的
			} while (level >= 0);
		}
	}
	return false;
}
/*兄弟层*/
bool isExistDefine(string name, int level)
{
	int i;
	for (i = 1; i <= tx; i++)
	{
		if (symtable[i].name == name && symtable[i].level == level) return true;
	}
	return false;
}
/*生成pcode*/
void gen(string f, int l, int a)
{
	ins[cx].instruction = f;
	ins[cx].len = l;
	ins[cx].par = a;
	cx++;
}
