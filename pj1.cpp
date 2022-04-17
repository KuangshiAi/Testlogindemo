#include<stdio.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include<stack>
#define MAXSIZE 100
#define resultsize 100
using std::stack;
#include<list>
using std::list;
char* p[MAXSIZE];
typedef struct node{
	float coef[resultsize] = {0};
	char* poly[resultsize] = {'\0'};
	int index[resultsize] = {0};
	int pos = 0;
}Node;
int isp(char* c)//中值表达式化简所用优先级函数 
{
	if (!strcmp(c, "*") ||!strcmp(c,"/"))
		return 5;
	if (!strcmp(c, "+") || !strcmp(c, "-"))
		return 3;
	if (!strcmp(c, ")"))
		return 6;
	if (!strcmp(c, "("))
		return 1;
	if (!strcmp(c, ";"))
		return 0;
	return false;
}
int icp(char* c)
{
	if (!strcmp(c, "*") || !strcmp(c, "/"))
		return 4;
	if (!strcmp(c, "+") || !strcmp(c, "-"))
		return 2;
	if (!strcmp(c, ")"))
		return 1;
	if (!strcmp(c, "("))
		return 6;
	if (!strcmp(c, ";"))
		return 0;
	return false;
}
int find(char**evalvars, char* x,int num)//用于查找变量是否在求值映射中，并返回位置
{
	int i;
	for (i = 0; i<num&&strcmp(evalvars[i], x); i++);
	if (i >= num)
		return 0;
	else
		return i+1;
}
void transfer(char** p, char* expression, char** evalvars, char** evalints,int num)//将输入的表达式转化为相应的char型指针数组
{
	int i = 0,j=0;
	while (expression[i])
	{
		if (expression[i] != ' ' && !isalpha(expression[i])&&!isdigit(expression[i]))
		{
			char* temp = new char(2);
			temp[0] = expression[i];
			temp[1] = '\0';
			i++;
			p[j] = temp;
			j++;
		}
		else if (expression[i]==' ')
		{
			i++;
		}
		else if (isalpha(expression[i]))
		{
			if (isalpha(expression[i + 1]))
			{
				int k;
				for (k = 1; isalpha(expression[i + 1]); k++) //获得变量的长度k
				{
					i++;
				}
				char *temp = new char(k+1);
				for (int m = i - k + 1; m <= i; m++)
				{
					temp[m - i + k - 1] = expression[m];
				}
				temp[k] = '\0';
				i++;
				if (int pos = find(evalvars, temp, num))
				{
					p[j] = evalints[pos - 1];
					j++;
				}
				else
				{
					p[j] = temp;
					j++;
				}
			}
			else
			{
				char*temp=new char(2);
				temp[0] = expression[i];
				temp[1] = '\0';
				i++;
				if (int pos = find(evalvars, temp, num))
				{
					p[j] = evalints[pos - 1];
					j++;
				}
				else
				{
					p[j] = temp;
					j++;
				}
			}
		}
		else if (isdigit(expression[i]))
		{
			if (isdigit(expression[i + 1]))
			{
				int k;
				for (k = 1; isdigit(expression[i + 1]); k++) //获得变量的长度k
				{
					i++;
				}
				char* temp = new char(k+1);
				for (int m = i - k + 1; m <= i; m++)
				{
					temp[m - i + k - 1] = expression[m];
				}
				temp[k] = '\0';
				p[j] = temp;
				j++;
				i++;
			}
			else
			{
				char* temp = new char(2);
				temp[0] = expression[i];
				temp[1] = '\0';
				p[j] = temp;
				j++;
				i++;
			}
		}
		
	}
}
void clearify(Node& a1)//将a1中系数为0的项去掉，化简当前结果 
{
	for (int i = 0; i < a1.pos; i++)
	{
		if (a1.coef[i] == 0)
		{
			for (int j = i; j < a1.pos-1; j++)
			{
				a1.coef[j] = a1.coef[j + 1];
				a1.poly[j] = a1.poly[j + 1];
				a1.index[j] = a1.index[j + 1];
			}
			a1.coef[a1.pos-1] = 0;
			a1.poly[a1.pos-1] = 0;
			a1.index[a1.pos-1] = 0;
			a1.pos--;
		}
	}
}
void sortpoly(Node& a1,int k)//对Node中某一项按照字典顺序进行排序 
{
	if (a1.index[k] > 1)
	{
		if(strlen(a1.poly[k])==2*a1.index[k]-1)
		{
		for(int i=0;i<a1.index[k];i++)
			for (int j = i; j < a1.index[k]-1; j++)
			{
				if (a1.poly[k][2 * j] > a1.poly[k][2 * j + 2])
				{
					char temp = a1.poly[k][2 * j];
					a1.poly[k][2 * j] = a1.poly[k][2 * j + 2];
					a1.poly[k][2 * j + 2] = temp;
				}

			}
		}
	}
}
void addition(Node& a1, Node& a2)//将a2加到a1 
{
	for (int i = 0; i < a2.pos; i++)
	{
		int tag = 0;
		for (int j = 0; j < a1.pos; j++)
		{
			if (strcmp(a2.poly[i], a1.poly[j]) == 0)
			{
				a1.coef[j] += a2.coef[i];
				tag = 1;
				break;
			}
		}
		if (!tag)
		{
			a1.coef[a1.pos] = a2.coef[i];
			a1.poly[a1.pos] = a2.poly[i];
			a1.index[a1.pos] = a2.index[i];
			a1.pos++;
		}
	}
	clearify(a1);
}
void clear(Node& a1)//将a1归零 
{
	for (int i = 0; i < a1.pos; i++)
	{
		a1.coef[i] = 0;
		a1.index[i] = 0;
		a1.poly[i] = 0;
	}
	a1.pos = 0;
}
void calculate(stack<Node> &OPND, char op)//给定运算符和运算数栈，进行计算 
{
	Node a2 = OPND.top();
	OPND.pop();
	Node a1 = OPND.top();
	OPND.pop();
	if (op == '+')
	{
		for (int i = 0; i < a2.pos; i++)
		{
			int tag = 0;
			for (int j = 0; j < a1.pos; j++)
			{
				if (strcmp(a2.poly[i], a1.poly[j]) == 0)
				{
					a1.coef[j] += a2.coef[i];
					tag = 1;
					break;
				}
			}
			if (!tag)
			{
				a1.coef[a1.pos] = a2.coef[i];
				a1.poly[a1.pos] = a2.poly[i];
				a1.index[a1.pos] = a2.index[i];
				a1.pos++;
			}
		}
		clearify(a1);
	}
	if (op == '-')
	{
		for (int i = 0; i < a2.pos; i++)
		{
			int tag = 0;
			for (int j = 0; j < a1.pos; j++)
			{
				if (strcmp(a2.poly[i], a1.poly[j]) == 0)
				{
					a1.coef[j] -= a2.coef[i];
					tag = 1;
					break;
				}
			}
			if (!tag)
			{
				a1.coef[a1.pos] = -a2.coef[i];
				a1.poly[a1.pos] = a2.poly[i];
				a1.index[a1.pos] = a2.index[i];
				a1.pos++;
			}
		}
		clearify(a1);
	}
	if (op == '*')
	{
		Node copy = a1;
		clear(a1);
		for (int i = 0; i < a2.pos; i++)
		{
			Node temp;
			clear(temp);
			if (a2.index[i]==0)
			{
				for (int j = 0; j < copy.pos; j++)
				{
					temp.poly[j] = copy.poly[j];
					temp.coef[j] = copy.coef[j] * a2.coef[i];
					temp.index[j] = copy.index[j];
					temp.pos++;
				}
				clearify(temp);
				addition(a1, temp);
			}
			else
			{
				for (int j = 0; j < copy.pos; j++)
				{
					temp.coef[j] = copy.coef[j] * a2.coef[i];
					char* p = new char(2);
					p[0] = '*';
					p[1] = '\0';
					temp.poly[j]=new char(resultsize);
					temp.poly[j][0]='\0';
					if(copy.index[j])
					{
					strcat(temp.poly[j],copy.poly[j]);
					strcat(temp.poly[j],p);
					strcat(temp.poly[j],a2.poly[i]);
				    }
				    else
				    {
				    	strcat(temp.poly[j],a2.poly[i]);
					}
					temp.index[j] = copy.index[j] + a2.index[i];
					sortpoly(temp, j);
					temp.pos++;
				}
				clearify(temp);
				addition(a1, temp);
				
			}
		}
		clearify(a1);
	}
	OPND.push(a1);
}
bool Allisnum(char* p)//判断字符串是否全部是数字 
{
	for (int i = 0; i < strlen(p); i++)
	{
		if (p[i] >= 48 && p[i] <= 57)
			continue;
		else
			return false;
	}
	return true;
}
Node nodify(char* p)//首次初始化字符串为可计算的变量
{
	Node temp;
	if (Allisnum(p))
	{
		temp.coef[0] = atof(p);
		char* q=new char(2);
		q[0] = '1';
		q[1] = '\0';
		temp.poly[0] =q ;
		temp.index[0] = 0;
		temp.pos=1;
	}
	else
	{
		temp.coef[0] = 1;
		temp.poly[0] = p;
		temp.index[0] = 1;
		temp.pos=1;
	}
	return temp;
}
bool isOperand(char* ch)//判断是否是操作数
{
	if (strcmp(ch, "+") && strcmp(ch, "-") && strcmp(ch, "*")  && strcmp(ch, "(") && strcmp(ch, ")")&&strcmp(ch,";")&&strcmp(ch,"/"))
		return true;
	else
		return false;
}
Node Infixrun(char**p)//中值表达式求值函数 
{
	stack <char*> OPTR;
	stack<Node>OPND;
	char* op;
	char* temp = new char(2);
	temp[0] = ';';
	temp[1] = '\0';
	OPTR.push(temp);
	int i = 0;
	op = temp;
	while (strcmp(p[i], ";") || strcmp(OPTR.top(),";"))
	{
		if (isOperand(p[i]))
		{
			OPND.push(nodify(p[i]));
			i++;
		}
		else
		{
			op=OPTR.top();
			if (icp(p[i]) > isp(op))
			{
				OPTR.push(p[i]);
				i++;
			}
			else if (icp(p[i]) < isp(op))
			{
				op = OPTR.top();
				OPTR.pop();
				calculate(OPND, op[0]);
			}
			else if (!strcmp(p[i], ")"))
			{
				OPTR.pop();
				i++;
			}
		}
	}
	return OPND.top();
}
void output(Node result)//输出结果 
{
	printf("{");
	clearify(result);
	int maxindex=0;
	for (int i = 0; i < result.pos; i++)
	{
		if (result.index[i] > maxindex)
			maxindex = result.index[i];
	}
	for (int i = maxindex; i > 0; i--)
	{
		int position[resultsize];
		int k=0;
		for (int j = 0; j < result.pos; j++)
		{
			if (result.index[j] == i)
			{
				position[k] = j;
				k++;
			}
		}
		if (k == 1)
		{
			printf("\"%.1f*%s\",", result.coef[position[0]], result.poly[position[0]]);//按照一位的精度输出系数 
		}
		if (k > 1)
		{
			for(int i=0;i<k;i++)
				for (int j = i; j < k - 1; j++)
				{
					if (strcmp(result.poly[position[j]], result.poly[position[j + 1]]) > 0)
					{
						int temp = position[j];
						position[j] = position[j + 1];
						position[j + 1] = temp;
					}
				}
			for(int i=0;i<k;i++)
				printf("\"%.1f*%s\",", result.coef[position[i]], result.poly[position[i]]);
		}
	}
	for (int j = 0; j < result.pos; j++)
	{
		if (result.index[j] == 0)
		{
			printf("\"%.1f\",", result.coef[j]);
		}
	}
	printf("\b");
	printf("}");
}
int main()
{
	printf("请输入表达式\n");
	char expression[MAXSIZE];
	gets(expression);
	expression[strlen(expression)+1] = '\0';
	expression[strlen(expression) ] = ';';
	char temp1[] = { 'e','x','p','r','e','s','s','i','o','n','\0'};
	char temp2[] = { 't','e','m','p','\0'};
	char* evalvars[] = {temp1,temp2};//求值映射中的变量，可以通过修改num、evalvars和evalints来更改求值映射 
	char temp3[] = { '1','\0'};
	char temp4[] = { '2','\0'};
	char *evalints[] = { temp3,temp4,};//对应变量的值 
	int num = 2;//求值映射元素个数
	transfer(p, expression, evalvars,evalints,num);
	output(Infixrun(p));
	return 0;
}
