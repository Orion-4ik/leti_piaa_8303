#include <vector>
#include <map>
#include <iostream>
#include <algorithm>

#define show_debug

bool cmp(std::pair<int, int> i, std::pair<int, int> j)
{
    if(i.second == j.second)
        return i.first < j.first;
    return i.second < j.second;
}

class Vertex {
public:
    std::map<char,int> nextV; // множество ребер бора
    std::map<char,int> machineSt;   // состояния автомата
    bool terminalTop;  // флаг для проверки терминальной вершины
    int parentI;  //индекс родительской вершины
    char parentSymb; // значение родительской вершины
    int link; // суффиксная ссылка
    int patternNum; //номер шаблона в терминальной вершине

    Vertex() {
    }
    Vertex(bool leaf, int parent, char parentChar, int link):
            terminalTop(leaf), parentI(parent), parentSymb(parentChar), link(link)
    { }
};

class Trie
{
    std::vector<Vertex> vertexes; // контейнер для хранения бор
    std::vector<std::string> patterns; //подстроки
    std::vector<std::pair<int,int>> result; //результат
public:

    void init() {  // иницализация бора
        Vertex head;
        head.parentI = head.link = -1;
        head.terminalTop = false;
        vertexes.push_back(head);
    }

    void addStr (const std::string & s) {
#ifdef show_debug
        std::cout<<std::endl;
        std::cout << "<added new str to trie: " << s << ">"<< std::endl;
#endif
        patterns.push_back(s);
        int v = 0;
        for (auto c : s) {  //просмотр по всему шаблону
#ifdef show_debug
            std::cout << "symbol " << c << std::endl;
#endif
            if (vertexes[v].nextV.find(c) == vertexes[v].nextV.end()) { // если вершины в боре нет, добавляем
#ifdef show_debug
                std::cout << "add new vertex " << c << "\n";
#endif
                Vertex buffer(false, v, c, -1);
                vertexes.push_back(buffer);
                vertexes[v].nextV[c] = vertexes.size() - 1;
            }
            v = vertexes[v].nextV[c];
        }
        vertexes[v].terminalTop = true; // помечаем вершну терминальной
        vertexes[v].patternNum = patterns.size() - 1; //добавляем индекс шаблона
    }

    int nextSt(int curSt, char symb) { //метод перехода в новое состояние автомата
#ifdef show_debug
        std::cout<<std::endl;
        std::cout << "<current symbol: " << symb <<">" << "\n";
#endif
        if (vertexes[curSt].machineSt.find(symb) == vertexes[curSt].machineSt.end()) //переход в новое состояние
        {
            if (vertexes[curSt].nextV.find(symb) == vertexes[curSt].nextV.end())
            {
                if(curSt == 0)
                    vertexes[curSt].machineSt[symb] = 0;
                else {
                    vertexes[curSt].machineSt[symb] = nextSt(getLink(curSt), symb); //переход по ссылке
#ifdef show_debug
                    std::cout << "go through reference: " << vertexes[curSt].machineSt[symb] << std::endl;
#endif
                }
            }
            else
            {
                vertexes[curSt].machineSt[symb] = vertexes[curSt].nextV[symb];
            }
        }
#ifdef show_debug
        std::cout << "next state: " << vertexes[curSt].machineSt[symb] << std::endl;
#endif
        return vertexes[curSt].machineSt[symb];
    }


    int getLink (int curTop) { // метод вычисления суффиксной ссылки
        if (vertexes[curTop].link == -1) // при отстутствии сслыки вычисляем ее
        {
            if (curTop == 0 || vertexes[curTop].parentI == 0) // добавлем ссылку на корень
            {
#ifdef show_debug
                std::cout<<std::endl;
                std::cout << "get reference to root: " <<" \n";
#endif
                vertexes[curTop].link = 0;
            }
            else// вычисление ссылки через родительскую вершину
            {
#ifdef show_debug
                std::cout << "calculate reference through parent: " << "\n";
#endif
                vertexes[curTop].link = nextSt(getLink(vertexes[curTop].parentI), vertexes[curTop].parentSymb);
            }
        }
#ifdef show_debug
        std::cout << "calculate reference to state: " << vertexes[curTop].link <<" \n";
#endif
        return vertexes[curTop].link;
    }

    void print()
    {
        std::sort(result.begin(), result.end(), cmp);
#ifdef show_debug
        std::cout << "Result of programm: " <<std::endl;
#endif
        for(auto iter : result)
        {
            std::cout << iter.second + 2 << " " << iter.first + 1 << std::endl;
        }

#ifdef show_debug
        std::cout << "vertexes: " << vertexes.size() << std::endl;
#endif
        for(size_t i = 0 ; i < result.size() - 1; i++) {
            for(size_t j = i + 1 ; j < result.size(); j++)
            {
                size_t first, second;
                first = patterns[result[i].first].size() - 1 + result[i].second;
                second =  result[j].second;
                if(first >= second)
                {
                    std::cout << "crossing of patterns " << patterns[result[i].first]
                              << " and " << patterns[result[j].first] << ". <" << result[i].first + 1 <<">"
                              << ", <" << result[j].first + 1 <<">" << " at ind " << result[i].second + 2
                              << "," << result[j].second + 2 << "\n";
                }
            }
        }
    }

    void ahoCorasick(std::string& text)
    {
        int state = 0;
        int i = 0;
        for(auto c : text)
        {
            state = nextSt(state, c); // переход в новое состояние
#ifdef show_debug
            std::cout << "state: " << state <<" \n";
#endif
            size_t tmp = state;
            while(tmp!=0) // цикл для поиска всех возможных вхождений
            {
                if(vertexes[tmp].terminalTop)
                {
                    std::pair<int,int> buffer;  // добавление в результат найденного шаблонаи его позиции
                    buffer.first = vertexes[tmp].patternNum;
                    buffer.second = i - patterns[buffer.first].size();
                    result.push_back(buffer);
#ifdef show_debug
                    std::cout << "found pattern " << buffer.second << ": " << patterns[buffer.first] << " \n";
#endif
                }
                tmp = getLink(tmp);
            }
            i++;
        }
    }


};

/*
NTAG
3
TAGT
TAG
T

CCCA
1
CC
*/

int main() {

    Trie trie;
    trie.init();
    std::string text, pattern;
    int n = 0;
    std::cin >> text>>n;
    for(int i = 0; i < n ;i++)
    {
        std::cin >> pattern;
        trie.addStr(pattern);
    }

    trie.ahoCorasick(text);
    trie.print();
    return 0;
}
