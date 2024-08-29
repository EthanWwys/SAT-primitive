#include<bits/stdc++.h>
using namespace std;
const int MAX_LITERAL = 1000;
const int MAX_CLAUSE = 10000;
// #define DEBUG 
struct Literal{
    int var;//变元
    int assign = 0;//赋值情况
    Literal *pre{};//上一文字
    Literal *next{};//下一文字
};

struct Clause{
    int length = 0;
    Literal *right{};//文字
    Literal *head;
    Literal *tail;
    Clause *up{};//上一子句
    Clause *down{};//下一子句
};

struct Reference{
    Literal *address;//文字关联子句索引
    Clause *clause_address;//文字关联子句索引
    Reference *next{};//下一索引
};

struct Frequency{
    //int index = 0;//用于记录回溯位置
    int var;
    int freq;//用于记录该变元在子句中出现的频次
};

struct ProblemSet{
    int num_clauses = 0;//变元个数
    int num_literals = 0;//子句个数
    Clause *head;//第一个子句
    Reference reference[MAX_LITERAL] = {nullptr};//文字引用数组
    Frequency literal_freq[2 * MAX_LITERAL + 1];
    int ans[MAX_LITERAL] = {0};
};

void AddReference(ProblemSet &problem, Literal &literal, Clause &clause)
{
    Reference* newNode = new Reference;
    newNode->address = &literal,newNode->clause_address = &clause, problem.literal_freq[literal.var + problem.num_literals].freq++;//文字引用频率++
    problem.literal_freq[literal.var + problem.num_literals].var = literal.var;

    //分是否是第一个插入的两种情况，使用头插法进行插入
    if(problem.reference[literal.var + problem.num_literals].next == nullptr)
    {
        problem.reference[literal.var + problem.num_literals].next = newNode;
        newNode->next = nullptr;
    }
    else
    {
        newNode->next = problem.reference[literal.var + problem.num_literals].next;
        problem.reference[literal.var + problem.num_literals].next = newNode;
    }
}

void AddClause(ProblemSet &problem, string &filename)
{
    //读取注释
    ifstream infile;
    infile.open(filename);
    if(!infile)
    {
        cout<<"fail to open file "<<filename<<endl;
        exit(EXIT_FAILURE);
    }
    char start, buffer[200];
    string cnf;
    infile>>start;
    while(start != 'p')
    {
        infile.getline(buffer, 200);
        infile>>start;
    }
    infile>> cnf>> problem.num_literals>> problem.num_clauses;
    infile.get();

    //读取cnf公式
    Literal *tail_Literal, *node;
    Clause *head_Clause, *tail_Clause;
    tail_Literal = new Literal;
    head_Clause = new Clause;
    problem.head = head_Clause;
    head_Clause->up = nullptr;
    for(int i = 1; i<=problem.num_clauses; i++)
    {
        int temp;
        tail_Clause = new Clause;
        tail_Literal = new Literal;
        head_Clause->right = tail_Literal;
        infile>>temp;

        while(temp)
        {
            node = new Literal;
            tail_Literal->var = temp, tail_Literal->assign = 0, head_Clause->length++;//变元创建并初始化
            AddReference(problem, *tail_Literal, *head_Clause);//语法？？？
            //head_Clause->tail = tail_Literal;//子句末尾赋值
            //双向链表连接
            tail_Literal->next = node, node->pre = tail_Literal, tail_Literal = node, tail_Literal->next = NULL;
            infile>>temp;
        }
        node->var = 0, node->assign = 0;//该子句以0为结尾
        head_Clause->head = head_Clause->right, head_Clause->tail = node->pre;

        infile.get();
        head_Clause->down = tail_Clause, tail_Clause->up = head_Clause;
        head_Clause = tail_Clause;
        tail_Clause->down = nullptr;
    }
    tail_Clause->up->down = NULL;

    Clause *it = problem.head;
    Literal *ptr;

    #ifdef DEBUG
    while(it)
    {
        ptr = it->right;
        while(ptr)
        {
            cout<<ptr->var<<","<<ptr->assign<<" ";
            ptr = ptr->next;
        }
        cout<<endl;
        it = it->down;
    }

    for(int i = 0; i<2 * problem.num_literals + 1; i++)
    {
        Reference *it = problem.reference[i].next;
        while(it)
        {
            cout<<((*(it->address)).var)<<" ";
            it = it->next;
        }
    }
    #endif DEBUG
}