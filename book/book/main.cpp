/*
#include<iostream>
#include<algorithm>
#include<queue>
using namespace std;
*/
/*
#define MAX 20
int n;			//��Ʒ����
int W;			//��������
class Ini		//��Ʒ��Ϣ
{
public:
	int value;	//��ֵ
	int weight;	//��ǰ��
	int i;	//��ʼ���
public:
	bool operator<(const Ini& x)const {return value/weight > x.value/x.weight;}
};

Ini ini[MAX] = {};	//��������Ʒ

class Node		//�����Ϣ
{
public:
	int sum_value;	//��ǰ�ܼ�ֵ
	int sum_weight;	//��ǰ������
	int max;	//������ж��ټ�ֵ
	int i;	//�������Ժ�ı�ţ���Ʒi�����������ini�е�λ�ã�
	int res[MAX];	//��ǰ������Ϊ�˷��㣬ÿ���ڵ㶼��¼�˵�ǰ�ĵķ����������е��˷ѿռ�
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
void input();			//��������
void mysort();			//������������
void search();			//����
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
	int max_value = 0;			//��ǰ�õ������Ž�
	int result[MAX] = {0,};		//���ŷ���

	priority_queue<Node> q;		//���ȶ���
	Node first;					//��һ����㣬һ������װ
	first.i = 1;				//��һ��Ҫװ��װ����Ʒ��ini�е�˳��
	first.sum_weight = 0;		//��ǰ����
	first.sum_value = 0;		//��ǰ��ֵ
	first.max = (W-first.sum_weight)*ini[first.i].value+first.sum_value;	//�Ͻ纯����Ҫ���ӵ�ǰ��㿪ʼ���ܵ��µ�����ֵ
	q.push(first);				//��һ��������
	while(!q.empty())
	{
		Node node = q.top();
		q.pop();
		if(node.i == n+1)		//���н����������
		{
			if(max_value < node.sum_value)	//����ֵ�������е�����ֵ����������ֵ�������ŷ���
			{
				max_value = node.sum_value;
				memcpy(result,node.res,sizeof(int)*4);
			}
			continue;
		}
		if(((node.sum_weight+ini[node.i].weight) < W) && (node.max > max_value))	//�������ж����Ͻ纯����������װ
		{
			Node add1;
			memcpy(add1.res,node.res,sizeof(int)*4);	//������һ�����ķ������Լ�(��һ��������������i-1����ȷ��ǰi-1����Ʒ�Ż򲻷�)
			add1.res[node.i] = 1;						//��i����Ʒ��
			add1.i = node.i+1;							//��һ����Ʒ��ini�е�λ��
			add1.sum_weight = node.sum_weight+ini[node.i].weight;	//�޸ĵ�ǰ��������ͬ
			add1.sum_value = node.sum_value+ini[node.i].value;		//�޸ĵ�ǰ��ֵ����ͬ
			add1.max = add1.sum_value+(W-add1.sum_weight)*ini[add1.i].value;	//�޸Ŀ��ܵ��µ����ż�ֵ����ͬ
			q.push(add1);								//���
		}
		//��װ
		Node add0;
		memcpy(add0.res,node.res,sizeof(int)*4);
		add0.res[node.i] = 0;
		add0.i = node.i+1;
		add0.sum_weight = node.sum_weight;
		add0.sum_value = node.sum_value;
		add0.max = add0.sum_value+(W-add0.sum_weight)*ini[add0.i].value;
		q.push(add0);
	}

	//������
	cout <<"optimal value:" <<max_value <<endl;

	int r[MAX] = {0,};
	for(int i=1;i <= n;i++)			//����ʱ�Ź�������Ҫ��ԭ��Ʒ˳��Ȼ�����
		r[ini[i].i] = result[i];
	for(int i=1;i <= n;i++)
		cout <<r[i] <<" ";
	cout <<endl;
}
*/
/*
int solve()
{
	int n,m;				//n���������ֳ�m��
	cin >>n >>m;
	if(n < m || n == 0)		//��������С�ڶ�����������󷵻�
	{
		cout <<"error" <<endl;
		return 1;
	}
	int* t = new int[n+1];		//���������洢
	int** f = new int*[n+1];	//�������洢
	for(int i=0;i <= n;i++)
		f[i] = new int[m+1];

	//��ʼ��
	for(int i=1; i <= n;i++) cin >>t[i];
	for(int i=0;i <= n;i++) f[i][0] = 0;
	for(int i=0;i <= m;i++)f[0][i] = 0;

	int i,j,k,temp,maxt;
	for(i=1;i <= n;i++)
		f[i][1]=f[i-1][1]+t[i];		//ǰi�����ֳ�һ�Σ�����ǰi�����ĺ�
	for(j=2;j <= m;j++)				//j��2��ʼ,������
	{
		for(i=j;i <= n;i++)			//��f[2][2]��ʼ��f[n][2]�����ϵ��£�[i,j],j>i����������
		{
			for(k=1,temp=INT_MAX;k < i;k++)		//kλ��1��i֮��
			{
				//���ֵ����1��k�ֳ�j-1��ʱ�Ӷκ����ֵ����Сֵ����k+1��i���ܺ�
				maxt = max(f[i][1]-f[k][1],f[k][j-1]);
				if(temp > maxt)			//ȡ��С
					temp = maxt;
			}
			f[i][j] = temp;				//��Сֵ����f[i][j]
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