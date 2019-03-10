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
bool v[6];		// 노드 방문 여부 저장 vist
int	d[6];	// 최단 거리 저장	distance

// 최소 거리 가지는 정점 반환
int getSmallIndex()
{
	int min = INF;
	int index = 0;

	// 선형 탐색
	for (int i = 0; i < number; i++)
	{
		if (!v[i] && d[i] < min)			// 방문하지 않은 노드중에서 최소지점 찾기
		{
			min = d[i];						// min을 더 작은 d[i]으로 치환
			index = i;						// 해당 위치를 기억
		}
	}
	return index;							// 해당 인덱스 반환
}

// 다익스트라 수행 함수
void dijkstra(int start)
{
	for (int i = 0; i < number; i++)
	{
		d[i] = graph[start][i];				// 시작점으로부터 초기 최단거리 저장
	}
	v[start] = true;						// 시작지점 방문 처리

	for (int i = 0; i < number; i++)
	{
		int current = getSmallIndex();		// 방문하지 않은 지점중 최소비용을 가지는 index 획득
		v[current] = true;					// 해당 노드 방문 처리

		for (int j = 0; j < number; j++)
			if (!v[j])
				if (d[current] + graph[current][j] < d[j])		// 노드를 거쳐가는 경우의 값이 현재의 최단거리보다 작을 경우
					d[j] = d[current] + graph[current][j];		// 최단거리를 더 작은 값으로 갱신
	}
}
// ------------------ 최소 비용 탐색을 선형탐색으로 찾도록 해서  시간 복잡도가 O(N^2)이므로 사이즈가 커질경우 비효율적
// 특히 정점은 많은데 간선이 적을 경우 치명적일 정도로 비효율적으로 작동할 수 있다.

// 개선을 위해 힙 구조 활용 시간 복잡도 O(N * log N)으로 제작


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