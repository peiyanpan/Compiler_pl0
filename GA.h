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
	string value;
	string key;
	int row;
	int column;
};
Unit unit;
Unit last_unit;//上一个 
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
	//if(unit.value == "")
}
void exp();
/*
	* <factor> -> <id>|<integer>|(<exp>)
*/
void factor()
{
	if (unit.key == "id") Readline();
	else if (unit.key == "int")
	{
		//cout << unit.value;
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
		Readline();
		factor();
	}
}
/*
	*<condecl> -> const <const> {,<const>}
*/
void condecl()
{
	if (unit.key == "id" || *errorType == 16)
	{
		if (*errorType == 16) errorType++;
		else Readline();
		if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
		{
			if (*errorType == 17) errorType++; 
			else Readline();
			if (unit.key == "int" || *errorType == 19)
			{
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
				while (unit.value == "," && unit.key == "..." )
				{
					Readline();
					if (unit.key == "id" || *errorType == 16)
					{
						if (*errorType == 16) errorType++;
						else Readline();
						if ((unit.key == "..." && unit.value == ":=") || *errorType == 17)
						{
							if (*errorType == 17) errorType++;
							else Readline();
							if (unit.key == "int" || *errorType == 19)
							{
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
	if (unit.value == "+" || unit.value == "-")
	{
		Readline();
	}
	term();
	while ((unit.value == "+" || unit.value == "-") && unit.key == "aop")
	{
		Readline();
		term();
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
	}
	else
	{
		exp();
		if (((unit.value == "=" || unit.value == "<>" || unit.value == "<" || unit.value == "<=" || unit.value == ">" || unit.value == ">=") && unit.key == "lop") || *errorType == 20)
		{
			if (*errorType == 20)
			{
				errorType++;
			}
			else Readline();
			exp();
		}
		else errortype(20);//缺少比较符号 
	}
}
/*
	*<block> -> [<condecl>][<vardecl>][<proc>]<body>
*/
void vardecl();
void proc();
void body();
void block()
{
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
	}
	body();
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
	if (unit.key == "id" || *errorType == 21)
	{
		if (*errorType == 21) errorType++;
		else Readline();
		while ((unit.value == "," && unit.key == "..." )|| *errorType == 25)
		{
			if (*errorType == 25) errorType++;
			else Readline();
			if (unit.key == "id" || *errorType == 21)
			{
				if (*errorType == 21) errorType++;
				else Readline();
			}
			else errortype(21); //缺少id 
		}
		if (unit.key == "id") errortype(25);//缺少逗号 
		if ((unit.value == ";" && unit.key == "..." )|| *errorType == 26)
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
	if (unit.key == "id"||*errorType == 22)
	{
		if (*errorType == 22) errorType++;
		else Readline();
		if (unit.value == "(" && unit.key == "...")
		{
			Readline();
			if (unit.key == "id")
			{
				Readline();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					if (unit.key == "id")
					{
						Readline();
					}
					//else errortype(22);
				}
			}
			if ((unit.value == ")" && unit.key == "...")||*errorType == 23)
			{
				if (*errorType == 23) errorType++;
				else Readline();
				if ((unit.value == ";" && unit.key == "...")||*errorType == 24)
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
		Readline();
		if ((unit.value == ":=" && unit.key == "...") || *errorType == 6)
		{
			if (*errorType == 6) errorType++;
			else Readline();
			exp();
		}
		else errortype(6);// 缺少赋值运算符 
	}
	else if (unit.value == "if" && unit.key == "keyword")
	{// if <lexp> then <statement> [else <statement>]
		Readline();
		lexp();
		if (unit.key == "int") Readline();
		if ((unit.value == "then" && unit.key == "keyword") || *errorType == 7)
		{
			if (*errorType == 7) errorType++;
			else Readline();
			statement();
			if (unit.value == "else" && unit.key == "keyword")
			{
				Readline();
				statement();
			}
		}
		else errortype(7); //缺少then 
	}
	else if (unit.value == "while" && unit.key == "keyword")
	{// while <lexp> do <statement>
		Readline();

		lexp();
		if ((unit.value == "do" && unit.key == "keyword") || *errorType == 8)
		{
			if (*errorType == 8) errorType++;
			else Readline();
			statement();
		}
		else errortype(8);//缺少do 
	}
	else if (unit.value == "call" && unit.key == "keyword")
	{// call <id> ([<exp>{,<exp>}])
		Readline();
		if (unit.key == "id" || *errorType == 9)
		{
			if (*errorType == 9) errorType++;
			else Readline();
			if (unit.value == "(" && unit.key == "...")
			{
				Readline();
				exp();
				while (unit.key == "..." && unit.value == ",")
				{
					Readline();
					exp();
				}
				if ((unit.key == "..." && unit.value == ")") || *errorType == 10)
				{
					if (*errorType == 10) errorType++;
					else Readline();
				}
				else errortype(10);
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
				if (*errorType == 11) errorType++;
				else Readline();
				while (unit.value == "," && unit.key == "...")
				{
					Readline();
					if (unit.key == "id") Readline();
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
			while (unit.value == "," && unit.key == "...")
			{
				Readline();
				exp();
			}
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
				return ;
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
int GA()
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
	return 0;
} 
