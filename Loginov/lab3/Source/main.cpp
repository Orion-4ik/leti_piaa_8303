#include <iostream>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <stack>
using namespace std;
#define SHOW_DEBUG


struct edge
{
    char first; // начальная вершина
    char second; // вершина назначения
    int weight; // вес ребра

    int forward; // поток в найденом пути
    int back; // поток в противоположном пути
    bool bidirectional; // двунаправленное ребро

};

bool compare(edge a, edge b)
{
    if (a.first == b.first)
        return a.second < b.second;
    return a.first < b.first;
}
class Graph
{
private:

    char start; // исток
    char finish; // сток
    int N; // количество ребер
    vector <edge> edges; // ребра
    vector <char> solutions; // найденный путь
    vector <char> checkedpoint; // просмотренные вершины
public:
    Graph()
    {
        cin >> N;
        cin >> start >> finish;
        for (int i = 0; i < N; i++)
        {
            edge elem;
            cin >> elem.first >> elem.second >> elem.weight; // считывание ребра
            elem.forward = elem.weight;
            elem.back = 0;
            elem.bidirectional = false;

            bool flag = true;
            for (int i = 0; i < edges.size(); i++) // проверка на существование обратного ребра
            {
                if (edges.at(i).first == elem.second && edges.at(i).second == elem.first)
                {
                    edges.at(i).back += elem.forward;
                    flag = false;
                    edges.at(i).bidirectional = true;
                    break;
                }
            }
            if ( !flag) // если получено обратное ребро, то не добавляем
                continue;
            edges.push_back(elem); // добавляем ребро
        }
    }

    bool isChecked(char elem)
    {
        for (int i = 0; i < checkedpoint.size(); i++)
            if (checkedpoint.at(i) == elem)
                return true;
        return false;
    }


    bool Search()
    {
        stack<char> s;
        s.push(start);

        while(!s.empty())
        {
            char elem = s.top();
            checkedpoint.push_back(elem); // добавляем в вектор просмотренных вершин
            s.pop();

            bool found = false;
            for (int i = 0; i < edges.size(); i++)
            {
                if (elem == edges.at(i).first) // прямой ход по ребру
                {
                    if (isChecked(edges.at(i).second) || edges.at(i).forward == 0)
                        continue; // если далее путь просмотрен или пропускная способность равна 0, то не рассмотриваем

                    if(edges.at(i).second == finish)
                    {
                        solutions.push_back(elem);
                        solutions.push_back(finish);
                        return true;
                    }

                    found = true;
                    s.push(edges.at(i).second);
                }

                if (elem == edges.at(i).second) // обратный ход по ребру
                {
                    if (isChecked(edges.at(i).first) || edges.at(i).back == 0)
                        continue; // если далее путь просмотрен или пропускная способность равна 0, то не рассмотриваем

                    if(edges.at(i).first == finish)
                    {
                        solutions.push_back(elem);
                        solutions.push_back(finish);
                        return true;
                    }

                    found = true;
                    s.push(edges.at(i).first);
                }
            }

            if(found) {
                solutions.push_back(elem);
            }
            else
                solutions.pop_back();

        }
        return false;
    }

    int calc_min()
    {
        int min = INT_MAX;
        for (int i = 1; i < solutions.size(); i++)
        {
            for (int j = 0; j < edges.size(); j++)
            {
                if (edges.at(j).first == solutions.at(i - 1) && edges.at(j).second == solutions.at(i))
                {
                    if(edges.at(j).forward < min)
                        min = edges.at(j).forward;
                }
                if (edges.at(j).second == solutions.at(i - 1) && edges.at(j).first == solutions.at(i))
                {
                    if(edges.at(j).back < min)
                        min = edges.at(j).back;
                }
            }
        }
        return min;
    }

    void print(int maxFlow)
    {
        cout << maxFlow << endl; // вывод данных
        sort(edges.begin(), edges.end(), compare);
        for (int i = 0; i < edges.size(); i++)
        {
            int elem = max(edges.at(i).weight - edges.at(i).forward, 0 - edges.at(i).back);
            if (edges.at(i).bidirectional == true)
            {
                if (elem < 0)
                    elem = 0;
                cout << edges.at(i).first << " " << edges.at(i).second << " " << elem << endl;
                swap(edges.at(i).first, edges.at(i).second);
                swap(edges.at(i).back, edges.at(i).forward);
                edges.at(i).bidirectional = false;
                sort(edges.begin(), edges.end(), compare);
                i--;
            }
            else
                cout << edges.at(i).first << " " << edges.at(i).second << " " << elem << endl;
        }
    }

    int FordFulkerson()
    {
        int maxFlow = 0; // максимальный поток
        int min = INT_MAX; // минимальная пропускная способность
        while (Search()) // пока есть путь к стоку
        {

#ifdef SHOW_DEBUG
            cout << "Current path: " << start;
            for (int i = 1; i < solutions.size(); i++) {
                cout << solutions.at(i);
            }
            cout << "(" << maxFlow << ")"<< endl;


            for (int i = 1; i < solutions.size(); i++)
            {
                for (int j = 0; j < edges.size(); j++)
                {
                    if (edges.at(j).first == solutions.at(i - 1) && edges.at(j).second == solutions.at(i))
                    {
                        cout <<"  "<< edges.at(j).first << edges.at(j).second << "(" << edges.at(j).forward<<")" << endl;
                    }
                    if (edges.at(j).second == solutions.at(i - 1) && edges.at(j).first == solutions.at(i))
                    {
                        cout <<"  " << edges.at(j).second << edges.at(j).first << "(" << edges.at(j).back<<") - back" << endl;
                    }
                }
            }
#endif

            int min = calc_min();	// минимальная пропускная способность

#ifdef SHOW_DEBUG
            cout << "Current min: " << min << endl;
            cout << "Recount:" << endl;
#endif
            for (int i = 1; i < solutions.size(); i++)
            {
                for (int j = 0; j < edges.size(); j++)
                {
                    if (edges.at(j).first == solutions.at(i - 1) && edges.at(j).second == solutions.at(i))
                    {
#ifdef SHOW_DEBUG
                        cout << "straight:" << edges.at(j).first << edges.at(j).second <<"(";
                        cout << edges.at(j).forward << "-" << min<<")" << "=" << edges.at(j).forward-min << endl;
#endif
                        edges.at(j).forward -= min;
#ifdef SHOW_DEBUG
                        cout << "back:" << edges.at(j).second << edges.at(j).first <<"(";
                        cout << edges.at(j).forward << "+" << min <<")"<< "=" << edges.at(j).forward+min << endl;
#endif
                        edges.at(j).back += min;
                    }
                    if (edges.at(j).second == solutions.at(i - 1) && edges.at(j).first == solutions.at(i))
                    {
#ifdef SHOW_DEBUG
                        cout << "straight:" << edges.at(j).first << edges.at(j).second <<"(";
                        cout << edges.at(j).forward << "+" << min << "=" << edges.at(j).forward+min << endl;
#endif
                        edges.at(j).forward += min;
#ifdef SHOW_DEBUG
                        cout << "back:" << edges.at(j).second << edges.at(j).first <<"(";
                        cout << edges.at(j).forward << "-" << min << "=" << edges.at(j).forward-min << endl;
#endif
                        edges.at(j).back -= min;
                    }
                }
            }
            maxFlow += min;
#ifdef SHOW_DEBUG
            cout << "Max Flow: " << maxFlow << endl;
#endif
            checkedpoint.clear();
            solutions.clear();
            min = INT_MAX;

            cout<<endl;
        }

        return maxFlow;
    }
};
int main()
{
    Graph task;
    int maxFlow = task.FordFulkerson();
    task.print(maxFlow);

    return 0;
}
