#include <iostream>
#include <math.h>
//#define INF 2147483647
#define INF 100000000

using namespace std;

int number = 6;

int graph[6][6] = {
	{0,	2,	5,	1,INF,INF},
	{2,	0,	3,	2,INF,INF},
	{5,	3,	0,	3,	1,	5},
	{1,	2,	3,	0,	1,INF},
	{INF,INF,1,	1,	0,	2},
	{INF,INF,5,INF,	2,	0},
};
bool v[6];		// ��� �湮 ���� ���� vist
int	d[6];	// �ִ� �Ÿ� ����	distance

// �ּ� �Ÿ� ������ ���� ��ȯ
int getSmallIndex()
{
	int min = INF;
	int index = 0;

	// ���� Ž��
	for (int i = 0; i < number; i++)
	{
		if (!v[i] && d[i] < min)			// �湮���� ���� ����߿��� �ּ����� ã��
		{
			min = d[i];						// min�� �� ���� d[i]���� ġȯ
			index = i;						// �ش� ��ġ�� ���
		}
	}
	return index;							// �ش� �ε��� ��ȯ
}

// ���ͽ�Ʈ�� ���� �Լ�
void dijkstra(int start)
{
	for (int i = 0; i < number; i++)
	{
		d[i] = graph[start][i];				// ���������κ��� �ʱ� �ִܰŸ� ����
	}
	v[start] = true;						// �������� �湮 ó��

	for (int i = 0; i < number; i++)
	{
		int current = getSmallIndex();		// �湮���� ���� ������ �ּҺ���� ������ index ȹ��
		v[current] = true;					// �ش� ��� �湮 ó��

		for (int j = 0; j < number; j++)
			if (!v[j])
				if (d[current] + graph[current][j] < d[j])		// ��带 ���İ��� ����� ���� ������ �ִܰŸ����� ���� ���
					d[j] = d[current] + graph[current][j];		// �ִܰŸ��� �� ���� ������ ����
	}
}
// ------------------ �ּ� ��� Ž���� ����Ž������ ã���� �ؼ�  �ð� ���⵵�� O(N^2)�̹Ƿ� ����� Ŀ����� ��ȿ����
// Ư�� ������ ������ ������ ���� ��� ġ������ ������ ��ȿ�������� �۵��� �� �ִ�.

// ������ ���� �� ���� Ȱ�� �ð� ���⵵ O(N * log N)���� ����


int main(void)
{
	dijkstra(0);
	for (int i = 0; i < number; i++)
	{
		cout << d[i] << " ";
	}

	cout << endl;
	return 0;
}