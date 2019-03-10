#include <iostream>
#include <fstream>		// ���� �����
#include <vector>
#include <queue>		// �켱 ���� queue ���� ���� ������ ������� �����ϱ� ������ �� ��� ��� ����
#include <cstring>		
#include <string>
#define NAME_MAX 100
#define CITY_MAX 100

using namespace std;

// ���� ���� ���� Ŭ����
class city
{
private:
	char * city_name;	// ���� �̸�
	int xpos, ypos;		// ���� ��ǥ
	int num;			// ���� ��ȣ

public:
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


//------------------------------ ���� ���� ���� �б�-----------------------------------------
int ReadCityFile(city * citys)
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

		citys[count].SetCityInfo(cityName, xpos, ypos, count);
		count++;
	}
	fin.close();

	return count;
}

//------------------------------ ���� ���� ���� �б�-----------------------------------------
void ReadEdgeFile(city * citys, int * d, vector<pair<int, int>> * a)
{
	char cityName[NAME_MAX];				
	char cityName2[NAME_MAX];
	int distance;							// ���ð� �Ÿ�


	ifstream fin;
	fin.open("Test.txt");
	while (!fin.eof())
	{
		fin >> cityName;
		fin >> cityName2;
		fin >> distance;
		cout << cityName << " " << cityName2 << " " << distance << endl;
	}
	fin.close();
}


int number = 6;
int INF = 100000000;




void dijkstra(int start, int * d, vector<pair<int,int>> * a)
{
	d[start] = 0;							// �ڱ� �ڽŰ��� �Ÿ��� 0
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
			}
		}
	}
}




int main(void)
{
	int count = 0;							// ���� ���� ī��Ʈ
	city citys [CITY_MAX];					// ���� ��ü �迭


	count = ReadCityFile(citys);			// ���� ���� ������ �а� ���� ���� ��ȯ, ���� ������ citys�� ����

	for (int i = 0; i < count; i++)
		citys[i].ShowCityData();

	//ReadEdgeFile();


	vector<pair<int, int>> a[7];		// ���� ����
	int d[7];							// �ּ� ���

	vector<pair<int, int>> Edge [CITY_MAX];
	int * arr;



	// ��� ������� ���� ���·� �ʱ�ȭ
	for (int i = 0; i <= number; i++)
	{
		d[i] = INF;
	}

	a[1].push_back(make_pair(2, 2));		// push_back : container �޺κп� ����
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
		cout << d[i] << " ";

	cout << endl;
}