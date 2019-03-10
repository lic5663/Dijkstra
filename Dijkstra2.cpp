#include <iostream>
#include <fstream>		// 파일 입출력
#include <vector>
#include <queue>		// 우선 순위 queue 사용시 힙과 유사한 방식으로 구동하기 때문에 힙 대신 사용 가능
#include <cstring>		
#define NAME_MAX 100
#define CITY_MAX 100

using namespace std;
int INF = 100000000;

//--------------------------- 도시 정보 저장 클래스----------------------------
class city
{
private:


public:
	char * city_name;	// 도시 이름
	int xpos, ypos;		// 도시 좌표
	int num;			// 도시 번호

	city()
	{
		city_name = NULL;
		xpos = 0;
		ypos = 0;
		num = 0;
	}
	city(char * name, int x, int y, int n) : xpos(x), ypos(y), num(n)
	{
		city_name = new char[strlen(name) + 1];
		strcpy(city_name, name);
	}
	// 기초생성자로 생성된 도시 설정 변경 함수
	void SetCityInfo(char * name, int x, int y, int n)
	{
		city_name = new char[strlen(name) + 1];
		strcpy(city_name, name);
		xpos = x;
		ypos = y;
		num = n;
	}
	// 디버깅용 도시 정보 출력 함수
	void ShowCityData() const								
	{
		cout << num << " " << city_name << " " << xpos << ", " << ypos << endl;
	}
	~city()
	{
		delete [] city_name;
	}
};


//------------------------------ 도시 정보 파일 읽기-----------------------------------------
int ReadCityFile(city * cities)
{
	char cityName[NAME_MAX];				// 도시 이름
	int xpos, ypos;							// 도시 좌표
	char dumy;								// 도시명 x,y에서 파일 읽어오기 때, "," 제거용 더미
	int count = 0;							// 도시 갯수 카운트

	ifstream fin;
	fin.open("city.txt");
	while (!fin.eof())
	{
		fin >> cityName;
		fin >> xpos;
		fin >> dumy;
		fin >> ypos;

		cities[count].SetCityInfo(cityName, xpos, ypos, count);
		count++;
	}
	fin.close();

	return count;
}

//----------------------------- 도시 이름 비교해서 도시 번호 반환---------------------------
int TransNum(city * cities, int count, char * name)
{
	for (int i = 0; i < count; i++)
	{
		if (strcmp(cities[i].city_name, name) == 0)
			return i;
	}
}

//------------------------------ 도시 간선 파일 읽기-----------------------------------------
void ReadEdgeFile(city * cities, int count , int * d, vector<pair<int, int>> * a)
{
	char cityName[NAME_MAX];				
	char cityName2[NAME_MAX];
	int distance;							// 도시간 거리
	int index = 0;
	int city1, city2;						// 도시 번호 저장 변수

	ifstream fin;
	fin.open("cityEdge.txt");
	while (!fin.eof())
	{
		fin >> cityName;
		fin >> cityName2;
		fin >> distance;

		// 도시명을 비교하여 도시 번호 반환
		city1 = TransNum(cities, count, cityName);
		city2 = TransNum(cities, count, cityName2);

		// 간선 추가
		a[city1].push_back(make_pair(city2, distance));
		a[city2].push_back(make_pair(city1, distance));
	}
	fin.close();
}



void dijkstra(int start, int * d, vector<pair<int,int>> * a)
{
	d[start] = 0;							// 자기 자신과의 거리는 0
	priority_queue<pair<int, int>> pq;		// 우선 순위 queue: 데이터들이 우선순위를 가지며 순위가 높은 데이터 먼저 나간다	
	pq.push(make_pair(start, 0));

	while (!pq.empty())						// 큐가 비어있지 않다면 하나씩 꺼내주는 형태
	{
		int current = pq.top().first;		// 기본적으로 우선순위 queue는 기본 정렬 기준이 less이기 때문에 큰값이 top에 위치한다.
		int distance = -pq.top().second;	// 따라서 거리를 음수화시켜 저장하면 거리가 가장 짧은 지점이 top에 위치한다.
		pq.pop();

		// 최단 거리가 아닌 경우 스킵
		if (d[current] < distance) continue;


		// 인접 노드에 접근하면서 다익스트라 알고리즘 적용
		for (int i = 0; i < a[current].size(); i++)
		{
			// 선택된 노드의 인접 노드
			int next = a[current][i].first;
			// 선택된 노드를 거쳐서 인접 노드로 가는 비용
			int nextDistance = distance + a[current][i].second;
			// 기존 최소 비용과 비교후 교체 판단
			if (nextDistance < d[next])
			{
				d[next] = nextDistance;
				pq.push(make_pair(next, -nextDistance));	// 음수화 해서 넣어줘야 가장 짧은 거리가 top에 위치
			}
		}
	}
}

int main(void)
{
	int count = 0;							// 도시 갯수 카운트
	city cities [CITY_MAX];					// 도시 객체 배열
	
	count = ReadCityFile(cities);			// 도시 정보 파일을 읽고 도시 갯수 반환, 도시 정보를 citys에 저장

	for (int i = 0; i < count; i++)
		cities[i].ShowCityData();

	vector<pair<int, int>> Edge [CITY_MAX];	// 간선 정보 저장 벡터 city1.(city2 , dist)
	int * dist = new int[count];			// 노드간 최소거리 저장 배열

	// 최소거리 초기화
	for (int i = 0; i < count; i++)
	{
		dist[i] = INF;
	}

	// 도시간 간선 정보 획득
	ReadEdgeFile(cities, count, dist, Edge);

	char start[NAME_MAX];
	char end[NAME_MAX];
	cout << "출발 지점과 종료 지점을 입력해주세요." << endl;
	cin >> start >> end;
	cout << start << " " << end << endl;

	int start_int, end_int;						// 출발지점 도착지점 int값 변환값 저장 변수
	
	start_int = TransNum(cities, count, start);
	end_int = TransNum(cities, count, end);

	cout << start_int << " " << end_int << endl;

	// 다키스트라 알고리즘 구동
	dijkstra(start_int, dist, Edge);

	// 시작지점으로부터의 각 노드 최단거리 출력
	for (int i = 0; i < count; i++)
		cout << dist[i] << " ";
	cout << endl;

	// 최종 출력
	cout << "최단 거리는 " << dist[end_int] << "입니다." << endl;



	/*

	int number = 6;
	vector<pair<int, int>> a[7];		// 간선 정보
	int d[7];							// 최소 비용

	// 모두 연결되지 않은 상태로 초기화
	for (int i = 0; i <= number; i++)
	{
		d[i] = INF;
	}

	a[1].push_back(make_pair(2, 2));		// push_back : container 뒷부분에 삽입
	a[1].push_back(make_pair(3, 5));
	a[1].push_back(make_pair(4, 1));

	a[2].push_back(make_pair(1, 2));
	a[2].push_back(make_pair(3, 3));
	a[2].push_back(make_pair(4, 2));

	a[3].push_back(make_pair(1, 5));
	a[3].push_back(make_pair(2, 3));
	a[3].push_back(make_pair(4, 3));
	a[3].push_back(make_pair(5, 1));
	a[3].push_back(make_pair(6, 5));

	a[4].push_back(make_pair(1, 1));
	a[4].push_back(make_pair(2, 2));
	a[4].push_back(make_pair(3, 3));
	a[4].push_back(make_pair(5, 1));

	a[5].push_back(make_pair(3, 1));
	a[5].push_back(make_pair(4, 1));
	a[5].push_back(make_pair(6, 2));

	a[6].push_back(make_pair(3, 5));
	a[6].push_back(make_pair(5, 2));

	dijkstra(1,d,a);

	for (int i = 1; i <= number; i++)
	cout << dist[i] << " ";

	*/

	
	
}