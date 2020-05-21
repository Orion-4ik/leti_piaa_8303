#include <iostream>
#include <map>
#include <vector>

#define show_debug

class Vertex {
public:
    std::map<char,int> nextV; //множество ребер бора
    std::map<char,int> machineSt; // состояния автомата
    bool terminalTop; // флаг для проверки терминальной вершины
    int parentI; // индекс родительской вершины
    char parentSymb; // символ родительской вершины
    int link;  // суффиксная ссылка
    std::vector<int> listPatterns;  // множество индексов шаблонов
};

class Trie
{
    std::vector<Vertex> vertexes; //контейнер для хранения бор
    std::vector<std::string> patterns; // множество подстрок
    std::vector<int> result;  //результат
    std::vector<int> patternsI; //индексы вхождения подстрок в строке с джокером
public:

    void init() { // иницализация бора
        Vertex head;
        head.parentI = head.link = -1;
        head.terminalTop = false;
        vertexes.push_back(head);
    }

    void addStr (std::string s) {
#ifdef show_debug
        std::cout<<std::endl;
        std::cout << "<added new str to trie: " << s << ">"<< std::endl;
#endif
        patterns.push_back(s);
        int i = 0;
        for (auto c : s) {
#ifdef show_debug
            std::cout << "symbol " << c << std::endl;
#endif
            if (vertexes[i].nextV.find(c) == vertexes[i].nextV.end()) {  // если вершины в боре нет, добавляем
#ifdef show_debug
                std::cout << "add new vertex " << c << "\n";
#endif
                Vertex buffer;
                buffer.terminalTop = false;
                buffer.link = -1;
                buffer.parentI = i;
                buffer.parentSymb = c;

                vertexes.push_back(buffer);
                vertexes[i].nextV[c] = vertexes.size() - 1;
            }
            i = vertexes[i].nextV[c];
        }
        vertexes[i].terminalTop = true;  // помечаем вершну терминальной
        vertexes[i].listPatterns.push_back(patterns.size() - 1); // добавляем индекс шаблона
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
                else
                    vertexes[curSt].machineSt[symb] = nextSt(getLink(curSt), symb); //переход по ссылке
#ifdef show_debug
                std::cout << "go through reference: " << vertexes[curSt].machineSt[symb] << std::endl;
#endif
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

    int getLink (int curTop) {                                                   // метод получения суффиксной ссылки
        if (vertexes[curTop].link == -1)                                         // при отстутствии сслыки вычисляем ее
        {
            if (curTop == 0 || vertexes[curTop].parentI == 0)                          // добавлем ссылку на корень
            {
#ifdef show_debug
                std::cout<<std::endl;
                std::cout << "get reference to root: " <<" \n";
#endif
                vertexes[curTop].link = 0;
            }
            else                                                            // вычисление ссылки через родительскую вершину
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

    void ahoCorasickJ(std::string& text, std::string& pattern)
    {
        int state = 0;
        result.resize(text.size()); //вектор результата
        for(int i = 0; i < text.size();i++) {
            state = nextSt(state, text[i]); // переход в новое состояние
#ifdef show_debug
            std::cout << "state: " << state <<" \n";
#endif
            size_t tmp = state;
            while (tmp!=0){
                if(vertexes[tmp].terminalTop)
                {
#ifdef show_debug
                    std::cout << "found pattern "  << std::endl;
#endif
                    for(auto Li : vertexes[tmp].listPatterns) // вычисление индексов вхождений
                    {
                        int buffer = i + 1 - patterns[Li].size() - patternsI[Li];
                        if(buffer >=0 && buffer <= text.size() - pattern.size())
                        {
#ifdef show_debug
                            std::cout << "coincides in position: " << buffer << std::endl;
#endif
                            result[buffer]++;
                        }
                    }
                }
                tmp = getLink(tmp);
            }
        }
    }

    void splitStr(std::string& pattern, char symbJ) //метод разбиения строки
    {
        size_t currentPos, prevPos;
        for(size_t i = 0; i < pattern.size() && currentPos != std::string::npos;)
        {
            std::string buffer;
            while(pattern[i] == symbJ) i++;  // пропуск джокера в строке
            prevPos = i;
            currentPos = pattern.find(symbJ, i); //поиск следующего джокера
            if(currentPos == std::string::npos) //создание подстроки
                buffer = pattern.substr(i, pattern.size() - i);
            else
                buffer = pattern.substr( prevPos,currentPos - prevPos);
            if(!buffer.empty())
            {
                patternsI.push_back(prevPos); // запись индекса подстроки и добавление ее в бор
                addStr(buffer);
            }
            i = currentPos;
        }
    }

    void print(std::string pattern)
    {
#ifdef show_debug
        std::cout << "Result of programm: " <<std::endl;
#endif
        for(size_t i = 0; i < result.size(); i++)
        {
            if(result[i] == patterns.size())
                std::cout << i + 1<< "\n";
        }
    }
};

/*
ACTANCA
A$$A$
$

axaxax
a$a
$
*/

int main() {

    Trie trie;
    trie.init();
    std::string text, pattern;
    char joker;
    std::cin >> text>> pattern>> joker;

    trie.splitStr(pattern, joker);
    trie.ahoCorasickJ(text, pattern);
    trie.print(pattern);

    return 0;
}

