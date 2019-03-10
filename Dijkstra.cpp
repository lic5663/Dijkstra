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

/*-----------------------도시 정보 파일 읽기---------------------------------------------
@ 기능		: 노드 데이터를 가진 파일을 읽어와 각 항목별로 저장
@ PARAMETER : 노드 데이터 배열
----------------------------------------------------------------------------------------*/
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

		cities[count].SetCityInfo(cityName, xpos, ypos, count);		// 도시의 이름,x좌표,y좌표,도시번호를 기입한다
		count++;
	}
	fin.close();

	return count;							// 도시 갯수를 반환한다
}
/*-----------------------도시명을 번호로 변경---------------------------------------------
@ 기능		: 도시명을 파라미터로 받아 해당 도시 인덱스로 변환
@ PARAMETER : 노드 데이터 배열, 총 노드 개수 저장 변수, 노드 명
----------------------------------------------------------------------------------------*/
int TransNum(city * cities, int count, char * name)
{
	for (int i = 0; i < count; i++)
	{
		if (strcmp(cities[i].city_name, name) == 0)	// strcmp로 name과 일치하는 도시명을 찾아 인덱스 반환
			return i;
	}
}
/*-----------------------가중치 파일 읽어오기---------------------------------------------
@ 기능		: 노드간 가중치 파일 읽어오기
@ PARAMETER : 노드 데이터 배열, 총 노드 개수 저장 변수, 가중치 저장 벡터
----------------------------------------------------------------------------------------*/
void ReadEdgeFile(city * cities, int count, vector<pair<int, int>> * a)
{
	char cityName[NAME_MAX];				// 1차 도시 이름 저장 변수
	char cityName2[NAME_MAX];				// 2차 도시 이름 저장 변수
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

		// 간선 추가 (무방향 그래프이므로 양 방향에서 모두 연결해준다)
		a[city1].push_back(make_pair(city2, distance));
		a[city2].push_back(make_pair(city1, distance));
	}
	fin.close();
}

/*-----------------------다익스트라 알고리즘---------------------------------------------
@ 기능		: 최단 경로 탐색
@ PARAMETER : 출발지점, 각 노드간 최단거리 저장 배열, 가중치 저장 벡터 , 경로 저장용 배열
----------------------------------------------------------------------------------------*/
void dijkstra(int start, int * d, vector<pair<int,int>> * a , int * path)
{
	int index = 0;							// 궤적 저장용 순서 변수

	d[start] = 0;							// 자기 자신과의 거리는 0
	path[start] = -1;						// 궤적 처음값의 이전 노드는 존재하지않음을 표시

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
				path[next] = current;						// 진행할 노드에 어느 노드로부터 왔는지를 기록한다
			}
		}
		
	}
}

/*-----------------------경로 역추적-------------------------------------
@ 기능		: 다익스트라 알고리즘에서 기록된 path를 바탕으로 경로 역추적
@ PARAMETER : 출발지점, 도착지점, 경로, 도시 데이터
-----------------------------------------------------------------------*/
void FindPath(int start, int end, int * path, city * cities)
{
	int vertex = end;					// 끝부분에서 역추적하므로 최종지점 값을 대입한다.
	int stack[CITY_MAX];				// 스택형식으로 구현해 나중 출력시 시작 지점부터 끝까지 출력한다
	int top = 0;

	stack[top++] = vertex;				// 스택에 도착지점 삽입

	while (true)
	{
		vertex = path[vertex];			// path에는 해당 노드에 가기 이전 노드가 저장되어 있다.
		stack[top++] = vertex;			// 이전 노드를 스택에 삽입
		if (vertex == start) break;		// 이전 노드가 출발지점과 동일하면 종료
	}

	while (--top >= 0)					// 스택에서 순차적으로 빼주면서 방문 도시를 출력한다.
	{
		cout << cities[stack[top]].city_name;
		if (top > 0)
			cout << "->";
	}
	cout << endl;
}
//----------------------------------------------------------------------



int main(void)
{
	int count = 0;							// 도시 갯수 카운트
	city cities [CITY_MAX];					// 도시 객체 배열
	
	// 도시 정보 파일을 읽고 도시 갯수 반환, 도시 정보를 citys에 저장
	count = ReadCityFile(cities);			

	// 도시 정보 출력
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
	ReadEdgeFile(cities, count, Edge);

	// 출발,도착지점 입력
	char start[NAME_MAX];
	char end[NAME_MAX];
	cout << endl;
	cout << "출발 지점과 종료 지점을 입력해주세요." << endl;
	cin >> start >> end;

	// 출발, 도착지점 도시번호로 변환
	int start_int, end_int;						
	start_int = TransNum(cities, count, start);
	end_int = TransNum(cities, count, end);

	// 궤적 저장용 배열
	int path[CITY_MAX];							// 도시 다 거쳐올 수 있으니 최대 도시수만큼 크기를 잡는다
	memset(path, -1, sizeof(int)*CITY_MAX);		// 배열 -1로 초기화

	// 다키스트라 알고리즘 구동
	dijkstra(start_int, dist, Edge, path);

/********************** 디버깅용 출력 ********************************
	// 경로 데이터 출력
	cout << "궤적은 다음과 같습니다" << endl;
	for (int i = 0; i < count; i++)
	{
		cout << "path[" << i << ']' << " = " << path[i] << endl;
	}

	// 시작지점으로부터의 각 노드 최단거리 출력
	for (int i = 0; i < count; i++)
		cout << dist[i] << " ";
	cout << endl;
*********************************************************************/

	// 최종 출력
	cout << endl << endl;
	cout << "최단 거리는 " << dist[end_int] << "입니다." << endl << endl;

	cout << "경로는 다음과 같습니다" << endl;
	FindPath(start_int, end_int, path , cities);

	cout << endl;

	return 0;
}

