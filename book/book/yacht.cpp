#include <iostream>
using namespace std;
#define N 20
int a[N][N] = {0,};
int res[N][N]={0,};
int n;
void recv()
{
	cin >>n;
	for(int i=1;i <= n;i++)
		for(int j=i+1;j<= n;j++)
			cin >>res[i][j];
	//for(int i=1;i < n;i++)
	//	res[i][i+1] = a[i][i+1];
}
void pt()
{
	for(int i=1;i <= n;i++)
	{
		for(int j=1;j <= n;j++)
		{
			cout <<res[i][j] <<" ";
		}
		cout <<endl;
	}
}
void solve()
{
	for(int k=1;k < n;k++)
	{
		for(int i=1;i < n-k;i++)
		{
			for(int j=i+1;j < i+k;j++)
			{
				if(res[i][i+k] > res[i][j]+res[j][i+k])
					res[i][i+k] = res[i][j]+res[j][i+k];
			}
		}
	}
	cout <<res[n][n] <<endl;
}
int main()
{
	recv();
	solve();
	system("pause");
	return 0;
}