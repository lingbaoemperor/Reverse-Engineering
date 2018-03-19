/*
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;
*/
/*
#define MAX 20
int n;			//物品数量
int W;			//背包容量
class Ini		//物品信息
{
public:
	int value;	//价值
	int weight;	//当前物
	int i;	//初始编号
public:
	bool operator<(const Ini& x)const {return value/weight > x.value/x.weight;}
};

Ini ini[MAX] = {};	//排序后的物品

class Node		//结点信息
{
public:
	int sum_value;	//当前总价值
	int sum_weight;	//当前总重量
	int max;	//最大能有多少价值
	int i;	//排完序以后的编号（物品i在排序后数组ini中的位置）
	int res[MAX];	//当前方案，为了方便，每个节点都记录了当前的的方案，可能有点浪费空间
public:
	bool operator<(const Node& x)const {return max < x.max;}
	Node()
	{
		sum_value = 0;
		sum_weight = 0;
		max = 0;
		i = 1;
		memset(res,0,sizeof(int)*MAX);
	}
};
//bool compare(Node a,Node b){return (a.value/a.weight)>(b.value/b.weight);}
void input();			//接受输入
void mysort();			//输入数据排序
void search();			//搜索
int main()
{
	input();
	search();;
	system("pause");
	return 0;
}

void input()
{
	cout <<"How many???" <<endl;
	cin >>n;
	cout <<"contain:";
	cin >> W;
	cout <<"value:";
	for(int i=1;i <= n;i++)
	{
		cin >>ini[i].value;
		ini[i].i = i;
	}
	cout <<"weight:";
	for(int i=1;i <= n;i++)
		cin >>ini[i].weight;
	mysort();
}

void mysort()
{
	sort(ini+1,ini+n+1);
}

void search()
{
	int max_value = 0;			//当前得到的最优解
	int result[MAX] = {0,};		//最优方案

	priority_queue<Node> q;		//优先队列
	Node first;					//第一个结点，一个都不装
	first.i = 1;				//下一个要装或不装的物品在ini中的顺序
	first.sum_weight = 0;		//当前重量
	first.sum_value = 0;		//当前价值
	first.max = (W-first.sum_weight)*ini[first.i].value+first.sum_value;	//上界函数需要，从当前结点开始可能导致的最大价值
	q.push(first);				//第一个结点入队
	while(!q.empty())
	{
		Node node = q.top();
		q.pop();
		if(node.i == n+1)		//所有结点搜索完了
		{
			if(max_value < node.sum_value)	//最优值大于已有的最优值，更改最优值和最最优方案
			{
				max_value = node.sum_value;
				memcpy(result,node.res,sizeof(int)*4);
			}
			continue;
		}
		if(((node.sum_weight+ini[node.i].weight) < W) && (node.max > max_value))	//可行性判定和上界函数，满足则装
		{
			Node add1;
			memcpy(add1.res,node.res,sizeof(int)*4);	//复制上一个结点的方案给自己(上一个结点加入搜索到i-1，则确定前i-1个物品放或不放)
			add1.res[node.i] = 1;						//第i个物品放
			add1.i = node.i+1;							//下一个物品在ini中的位置
			add1.sum_weight = node.sum_weight+ini[node.i].weight;	//修改当前重量，下同
			add1.sum_value = node.sum_value+ini[node.i].value;		//修改当前价值，下同
			add1.max = add1.sum_value+(W-add1.sum_weight)*ini[add1.i].value;	//修改可能导致的最优价值，下同
			q.push(add1);								//入队
		}
		//不装
		Node add0;
		memcpy(add0.res,node.res,sizeof(int)*4);
		add0.res[node.i] = 0;
		add0.i = node.i+1;
		add0.sum_weight = node.sum_weight;
		add0.sum_value = node.sum_value;
		add0.max = add0.sum_value+(W-add0.sum_weight)*ini[add0.i].value;
		q.push(add0);
	}

	//结果输出
	cout <<"optimal value:" <<max_value <<endl;

	int r[MAX] = {0,};
	for(int i=1;i <= n;i++)			//输入时排过序，所以要还原物品顺序，然后输出
		r[ini[i].i] = result[i];
	for(int i=1;i <= n;i++)
		cout <<r[i] <<" ";
	cout <<endl;
}
*/
/*
int solve()
{
	int n,m;				//n个整数，分成m段
	cin >>n >>m;
	if(n < m || n == 0)		//整数个数小于段数，输入错误返回
	{
		cout <<"error" <<endl;
		return 1;
	}
	int* t = new int[n+1];		//输入整数存储
	int** f = new int*[n+1];	//计算结果存储
	for(int i=0;i <= n;i++)
		f[i] = new int[m+1];

	//初始化
	for(int i=1; i <= n;i++) cin >>t[i];
	for(int i=0;i <= n;i++) f[i][0] = 0;
	for(int i=0;i <= m;i++)f[0][i] = 0;

	int i,j,k,temp,maxt;
	for(i=1;i <= n;i++)
		f[i][1]=f[i-1][1]+t[i];		//前i个数分成一段，等于前i个数的和
	for(j=2;j <= m;j++)				//j从2开始,从左到右
	{
		for(i=j;i <= n;i++)			//从f[2][2]开始到f[n][2]，由上到下，[i,j],j>i不满足条件
		{
			for(k=1,temp=INT_MAX;k < i;k++)		//k位于1到i之间
			{
				//最大值等于1到k分成j-1段时子段和最大值的最小值，或k+1到i的总和
				maxt = max(f[i][1]-f[k][1],f[k][j-1]);
				if(temp > maxt)			//取最小
					temp = maxt;
			}
			f[i][j] = temp;				//最小值赋给f[i][j]
		}
	}

	cout <<f[n][m] <<endl;
	delete [] t;
	for(int i=0;i <= n;i++)
		delete [] f[i];
	delete [] f;
	system("pause");
	return 0;
}
int main()
{
	solve();
}
*/