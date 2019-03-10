#include <iostream>
#include <fstream>		// ���� �����
#include <vector>
#include <queue>		// �켱 ���� queue ���� ���� ������ ������� �����ϱ� ������ �� ��� ��� ����
#include <cstring>		
#define NAME_MAX 100
#define CITY_MAX 100

using namespace std;
int INF = 100000000;

//--------------------------- ���� ���� ���� Ŭ����----------------------------
class city
{
private:


public:
	char * city_name;	// ���� �̸�
	int xpos, ypos;		// ���� ��ǥ
	int num;			// ���� ��ȣ

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
	// ���ʻ����ڷ� ������ ���� ���� ���� �Լ�
	void SetCityInfo(char * name, int x, int y, int n)
	{
		city_name = new char[strlen(name) + 1];
		strcpy(city_name, name);
		xpos = x;
		ypos = y;
		num = n;
	}
	// ������ ���� ���� ��� �Լ�
	void ShowCityData() const								
	{
		cout << num << " " << city_name << " " << xpos << ", " << ypos << endl;
	}
	~city()
	{
		delete [] city_name;
	}
};

/*-----------------------���� ���� ���� �б�---------------------------------------------
@ ���		: ��� �����͸� ���� ������ �о�� �� �׸񺰷� ����
@ PARAMETER : ��� ������ �迭
----------------------------------------------------------------------------------------*/
int ReadCityFile(city * cities)
{
	char cityName[NAME_MAX];				// ���� �̸�
	int xpos, ypos;							// ���� ��ǥ
	char dumy;								// ���ø� x,y���� ���� �о���� ��, "," ���ſ� ����
	int count = 0;							// ���� ���� ī��Ʈ

	ifstream fin;
	fin.open("city.txt");
	while (!fin.eof())
	{
		fin >> cityName;
		fin >> xpos;
		fin >> dumy;
		fin >> ypos;

		cities[count].SetCityInfo(cityName, xpos, ypos, count);		// ������ �̸�,x��ǥ,y��ǥ,���ù�ȣ�� �����Ѵ�
		count++;
	}
	fin.close();

	return count;							// ���� ������ ��ȯ�Ѵ�
}
/*-----------------------���ø��� ��ȣ�� ����---------------------------------------------
@ ���		: ���ø��� �Ķ���ͷ� �޾� �ش� ���� �ε����� ��ȯ
@ PARAMETER : ��� ������ �迭, �� ��� ���� ���� ����, ��� ��
----------------------------------------------------------------------------------------*/
int TransNum(city * cities, int count, char * name)
{
	for (int i = 0; i < count; i++)
	{
		if (strcmp(cities[i].city_name, name) == 0)	// strcmp�� name�� ��ġ�ϴ� ���ø��� ã�� �ε��� ��ȯ
			return i;
	}
}
/*-----------------------����ġ ���� �о����---------------------------------------------
@ ���		: ��尣 ����ġ ���� �о����
@ PARAMETER : ��� ������ �迭, �� ��� ���� ���� ����, ����ġ ���� ����
----------------------------------------------------------------------------------------*/
void ReadEdgeFile(city * cities, int count, vector<pair<int, int>> * a)
{
	char cityName[NAME_MAX];				// 1�� ���� �̸� ���� ����
	char cityName2[NAME_MAX];				// 2�� ���� �̸� ���� ����
	int distance;							// ���ð� �Ÿ�
	int index = 0;							
	int city1, city2;						// ���� ��ȣ ���� ����

	ifstream fin;							
	fin.open("cityEdge.txt");
	while (!fin.eof())
	{
		fin >> cityName;
		fin >> cityName2;
		fin >> distance;

		// ���ø��� ���Ͽ� ���� ��ȣ ��ȯ
		city1 = TransNum(cities, count, cityName);
		city2 = TransNum(cities, count, cityName2);

		// ���� �߰� (������ �׷����̹Ƿ� �� ���⿡�� ��� �������ش�)
		a[city1].push_back(make_pair(city2, distance));
		a[city2].push_back(make_pair(city1, distance));
	}
	fin.close();
}

/*-----------------------���ͽ�Ʈ�� �˰���---------------------------------------------
@ ���		: �ִ� ��� Ž��
@ PARAMETER : �������, �� ��尣 �ִܰŸ� ���� �迭, ����ġ ���� ���� , ��� ����� �迭
----------------------------------------------------------------------------------------*/
void dijkstra(int start, int * d, vector<pair<int,int>> * a , int * path)
{
	int index = 0;							// ���� ����� ���� ����

	d[start] = 0;							// �ڱ� �ڽŰ��� �Ÿ��� 0
	path[start] = -1;						// ���� ó������ ���� ���� �������������� ǥ��

	priority_queue<pair<int, int>> pq;		// �켱 ���� queue: �����͵��� �켱������ ������ ������ ���� ������ ���� ������	
	pq.push(make_pair(start, 0));

	while (!pq.empty())						// ť�� ������� �ʴٸ� �ϳ��� �����ִ� ����
	{
		int current = pq.top().first;		// �⺻������ �켱���� queue�� �⺻ ���� ������ less�̱� ������ ū���� top�� ��ġ�Ѵ�.
		int distance = -pq.top().second;	// ���� �Ÿ��� ����ȭ���� �����ϸ� �Ÿ��� ���� ª�� ������ top�� ��ġ�Ѵ�.
		pq.pop();

		// �ִ� �Ÿ��� �ƴ� ��� ��ŵ
		if (d[current] < distance) continue;


		// ���� ��忡 �����ϸ鼭 ���ͽ�Ʈ�� �˰��� ����
		for (int i = 0; i < a[current].size(); i++)
		{
			// ���õ� ����� ���� ���
			int next = a[current][i].first;
			// ���õ� ��带 ���ļ� ���� ���� ���� ���
			int nextDistance = distance + a[current][i].second;
			// ���� �ּ� ���� ���� ��ü �Ǵ�
			if (nextDistance < d[next])
			{
				d[next] = nextDistance;
				pq.push(make_pair(next, -nextDistance));	// ����ȭ �ؼ� �־���� ���� ª�� �Ÿ��� top�� ��ġ
				path[next] = current;						// ������ ��忡 ��� ���κ��� �Դ����� ����Ѵ�
			}
		}
		
	}
}

/*-----------------------��� ������-------------------------------------
@ ���		: ���ͽ�Ʈ�� �˰��򿡼� ��ϵ� path�� �������� ��� ������
@ PARAMETER : �������, ��������, ���, ���� ������
-----------------------------------------------------------------------*/
void FindPath(int start, int end, int * path, city * cities)
{
	int vertex = end;					// ���κп��� �������ϹǷ� �������� ���� �����Ѵ�.
	int stack[CITY_MAX];				// ������������ ������ ���� ��½� ���� �������� ������ ����Ѵ�
	int top = 0;

	stack[top++] = vertex;				// ���ÿ� �������� ����

	while (true)
	{
		vertex = path[vertex];			// path���� �ش� ��忡 ���� ���� ��尡 ����Ǿ� �ִ�.
		stack[top++] = vertex;			// ���� ��带 ���ÿ� ����
		if (vertex == start) break;		// ���� ��尡 ��������� �����ϸ� ����
	}

	while (--top >= 0)					// ���ÿ��� ���������� ���ָ鼭 �湮 ���ø� ����Ѵ�.
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
	int count = 0;							// ���� ���� ī��Ʈ
	city cities [CITY_MAX];					// ���� ��ü �迭
	
	// ���� ���� ������ �а� ���� ���� ��ȯ, ���� ������ citys�� ����
	count = ReadCityFile(cities);			

	// ���� ���� ���
	for (int i = 0; i < count; i++)
		cities[i].ShowCityData();

	vector<pair<int, int>> Edge [CITY_MAX];	// ���� ���� ���� ���� city1.(city2 , dist)
	int * dist = new int[count];			// ��尣 �ּҰŸ� ���� �迭

	// �ּҰŸ� �ʱ�ȭ
	for (int i = 0; i < count; i++)
	{
		dist[i] = INF;
	}

	// ���ð� ���� ���� ȹ��
	ReadEdgeFile(cities, count, Edge);

	// ���,�������� �Է�
	char start[NAME_MAX];
	char end[NAME_MAX];
	cout << endl;
	cout << "��� ������ ���� ������ �Է����ּ���." << endl;
	cin >> start >> end;

	// ���, �������� ���ù�ȣ�� ��ȯ
	int start_int, end_int;						
	start_int = TransNum(cities, count, start);
	end_int = TransNum(cities, count, end);

	// ���� ����� �迭
	int path[CITY_MAX];							// ���� �� ���Ŀ� �� ������ �ִ� ���ü���ŭ ũ�⸦ ��´�
	memset(path, -1, sizeof(int)*CITY_MAX);		// �迭 -1�� �ʱ�ȭ

	// ��Ű��Ʈ�� �˰��� ����
	dijkstra(start_int, dist, Edge, path);

/********************** ������ ��� ********************************
	// ��� ������ ���
	cout << "������ ������ �����ϴ�" << endl;
	for (int i = 0; i < count; i++)
	{
		cout << "path[" << i << ']' << " = " << path[i] << endl;
	}

	// �����������κ����� �� ��� �ִܰŸ� ���
	for (int i = 0; i < count; i++)
		cout << dist[i] << " ";
	cout << endl;
*********************************************************************/

	// ���� ���
	cout << endl << endl;
	cout << "�ִ� �Ÿ��� " << dist[end_int] << "�Դϴ�." << endl << endl;

	cout << "��δ� ������ �����ϴ�" << endl;
	FindPath(start_int, end_int, path , cities);

	cout << endl;

	return 0;
}

