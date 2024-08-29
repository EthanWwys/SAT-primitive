
#include "function.h"
#include<fstream>
int main(void)
{
    string file = "D:/WYS/C Programming/WYS's DPLL/Program/u-5cnf_3500_3500_30f1.shuffled-30.cnf";
    ProblemSet problem;
    int x = 0;
    AddClause(problem, file);//读取cnf公式并存储到相应的数据结构中
    sort(problem.literal_freq, problem.literal_freq + 2 * problem.num_literals, [](const Frequency &a, const Frequency &b)
    {
        return a.freq > b.freq;
    });//根据文字出现频率进行排序，用于选择变元
    if(DPLL(problem))
    {
        for(int i = 1; i<=problem.num_literals; i++)
        {
            cout<<i<<" "<<problem.ans[i]<<endl;
        }
    }
    else
        cout<<"no answer";
}