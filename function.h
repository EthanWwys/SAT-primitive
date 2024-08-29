#include "DS.h"
int index = 0;
int ans[MAX_LITERAL] = {-1};
Literal *isSingle(ProblemSet &problem)
{
    Clause *it = problem.head;
    Literal *L;
    while(it!= nullptr)
    {
        if(it->length == 1)
        {
            L = it->right;
            while(L)
            {
                if(L->assign == 0)//未被赋值，就是那个单子句的文字
                {
                    L->assign = 1;
                    return L;
                }
            }
        }
        it = it->down;
    }
    return NULL;
}
void Delet_CLause(ProblemSet &problem, Literal &L)
{
    Reference *start = problem.reference[L.var + problem.num_literals].next;
    if(L.var > 0)
    {
        problem.ans[L.var] = 1;
    }
    else
        problem.ans[-L.var] = -1;//赋值为假
    while(start)
    {
        if((start->clause_address)->length != -1)
            problem.num_clauses--;//对应的，子句数量减少，用于后续判断问题集是否为空
        (start->clause_address)->length = -1;//删除子句
        start = start->next;
    }
}
void Delet_Literal(ProblemSet &problem, Literal &L)
{
    Reference *start = problem.reference[-L.var + problem.num_literals].next;
    while(start)
    {
        (start->address)->assign = -1;
        (start->clause_address)->length--;
        start = start->next;
    }
}
bool isEmpty(ProblemSet &problem)
{
    Clause *it = problem.head;
    while(it)
    {
        if(it->length == 0)
            return true;
        it = it->down;
    }
    return false;
}
int ChooseVar(ProblemSet &problem)//根据出现频率选择变元
{
    int test = abs(problem.literal_freq[index].var);
    while(index <=problem.num_literals &&problem.ans[abs(problem.literal_freq[index].var)]!=0)
        {index++;}
    return problem.literal_freq[index++].var;//返回被选择的变元，并指向下一个可能的变元
}
void postulate(ProblemSet &problem, int L)//将选取的变元在数据结构中赋值
{
    Reference *start = problem.reference[L + problem.num_literals].next;
    if(L > 0)
    {
        problem.ans[L] = 1;
    }
    else
        problem.ans[-L] = -1;//赋值为假
    while(start)
    {
        if((start->clause_address)->length != -1)
            problem.num_clauses--;//对应的，子句数量减少，用于后续判断问题集是否为空
        (start->clause_address)->length = -1;
        start = start->next;

    }
}
bool DPLL(ProblemSet &problem)
{
    if(problem.num_clauses!=0)
    {
        Literal *L;
        while(L = isSingle(problem))
        {
            Delet_CLause(problem, *L);
            Delet_Literal(problem, *L);
            if(problem.num_clauses == 0)//问题集为空，有解
                return true; 
            else if(isEmpty(problem))//有空子句，返回false
                return false;
        }
        //选择变元
        int var = ChooseVar(problem);
        //cout<<var<<endl;
        postulate(problem, var);
        if(DPLL(problem))   return true;
        return(postulate(problem, -var),DPLL(problem));
    }

}