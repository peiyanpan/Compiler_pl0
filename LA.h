#include<iostream>
#include<fstream>
#include<cstdlib> 
#include<string>
using namespace std;
int row, column;
const string keyword[15] = { "program", "const", "var", "procedure", "begin", "if", "end", "while", "call", "read", "write", "then", "odd", "do" ,"else" };
fstream infile;
fstream outfile;
/*void GetChar()
{
	if(GetBC())
	{
		while(GetBC()) infile>>ch;
	}
}*/
/*   whether it is blank   */
bool GetBC(char ch)
{
	if (ch == '\n' || ch == '\r')
	{
		row++;
		column = 1;
		return true;
	}
	else if (ch == '\t')
	{
		column += 4;
		return true;
	}
	else if (ch == ' ')
	{
		column++;
		return true;
	}
	else return false;
}
string Concat(string strToken, char ch)
{
	return strToken + ch;
}
/*   whether it is a letter   */
bool IsLetter(char ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return true;
	else return false;
}
/*   whether it is a number   */
bool IsDigit(char ch)
{
	if (ch >= '0' && ch <= '9') return true;
	else return false;
}
/*   whether it is a keyword   */
int Reserve(string strToken)
{
	for (int i = 0; i < 15; i++)
	{
		if (strToken.compare(keyword[i]) == 0) return i + 1;
	}
	return 0;
}
/*   back one letter   */
void Retract()//回调一个字符位置 
{
	if (!infile.eof())
	{
		infile.seekg(-1, ios::cur);
	}
}
int LA()
{
	infile.open("test2.txt");//open file "test1.txt"
	if (!infile)
	{
		cout << "Can't open test1.txt" << "\n";
		exit(0);
	}
	outfile.open("res1.txt");
	if (!outfile)
	{
		cout << "Can't open res1.txt" << "\n";
		exit(1);
	}
	row = 1;
	column = 1;//row and column initialization

	string strToken = "";
	char ch;

	while (!infile.eof())
	{
		/*   12 cases   */
		ch = infile.get();
		if (GetBC(ch)) strToken = "";//case 1:blank; circle until it is non-blank
		else if (IsLetter(ch))//case 2:word;keyword or id
		{
			strToken = Concat(strToken, ch);
			column++;
			ch = infile.get();
			while (IsLetter(ch) || IsDigit(ch))
			{
				column++;
				strToken = Concat(strToken, ch);
				ch = infile.get();
			}
			if (Reserve(strToken))//关键字 
			{
				outfile << strToken << "\t" << "keyword" << "\t" << row << "\t" << column << endl;
			}
			else
			{
				outfile << strToken << "\t" << "id" << "\t" << row << "\t" << column << endl;
			}
			//cout << strToken << "\n";
			strToken = "";
			Retract();  //返回上一位 
			//outfile << ch << "\n";
		}
		else if (IsDigit(ch))//case 3:number
		{
			strToken = Concat(strToken, ch);
			column++;
			ch = infile.get();
			while (IsDigit(ch))
			{
				column++;
				strToken = Concat(strToken, ch);
				ch = infile.get();
			}
			if (IsLetter(ch))
			{				
				while (IsLetter(ch) || IsDigit(ch))
				{
					strToken = Concat(strToken, ch);
					column++;
					ch = infile.get();
				}
				cout << "[Lexcial ERROR]"<<strToken << "\t" << "[" << row << "," << column << "]" << "\t" << "Invalid id:\n";
				outfile << strToken << "\t" << "id" << "\t" << row << "\t" << column << endl;
			}
			else outfile << strToken << "\t" << "int" << "\t" << row << "\t" << column << endl;
			strToken = "";
			Retract();
		}
		else if (ch == '<')//case 4:<; next is '>' or '=' or ""
		{
			column++;
			ch = infile.get();
			if (ch == '>')
			{
				column++;
				outfile << "<>" << "\t" << "lop" << "\t" << row << "\t" << column << endl;
			}
			else if (ch == '=')
			{
				column++;
				outfile << "<=" << "\t" << "lop" << "\t" << row << "\t" << column << endl;
			}
			else
			{
				outfile << "<" << "\t" << "lop" << "\t" << row << "\t" << column << endl;
				Retract();
			}
		}
		else if (ch == '>')//case 5:>;next is '=' or ""
		{
			column++;
			ch = infile.get();
			if (ch == '=')
			{
				column++;
				outfile << ">=" << "\t" << "lop" << "\t" << row << "\t" << column << endl;
			}
			else
			{
				outfile << ">" << "\t" << "lop" << "\t" << row << "\t" << column << endl;
				Retract();
			}
		}
		else if (ch == ':')//case 6:next must be '='
		{
			column++;
			ch = infile.get();
			if (ch == '=')
			{
				column++;
				outfile << ":=" << "\t" << "..." << "\t" << row << "\t" << column << endl;
			}
			else
			{
				cout << "[Lexcial ERROR]" << "[" << row << "," << column << "] " << "Missing \"=\" near the \":\" " << endl;
				outfile << ":=" << "\t" << "..." << "\t" << row << "\t" << column << endl;
				Retract();
			}
		}
		else if (ch == '=')//case 7:'='
		{
			column++;
			outfile << ch << "\t" << "lop" << "\t" << row << "\t" << column << endl;
		}
		else if (ch == '+' || ch == '-')//case 8:'+' or '-';
		{
			column++;
			outfile << ch << "\t" << "aop" << "\t" << row << "\t" << column << endl;
		}
		else if (ch == '*' || ch == '/')//case 9:'*' or '/';
		{
			column++;
			outfile << ch << "\t" << "mop" << "\t" << row << "\t" << column << endl;
		}
		else if (ch == ';')//case 10:';'
		{
			column++;
			outfile << ch << "\t" << "..." << "\t" << row << "\t" << column << endl;
		}
		else if (ch == '(' || ch == ')' || ch == ',')//case 11:'(' or ')' or ',';
		{
			column++;
			outfile << ch << "\t" << "..." << "\t" << row << "\t" << column << endl;
		}
		else//case 12:else
		{
			column++;
			outfile << ch << "\t" << "UNKOWEN" << "\t" << row << "\t" << column << endl;
		}
	}
	infile.close();//close file "test1.txt"
	outfile.close();
	//cout << "succeefully!" << endl;
	return 0;
}