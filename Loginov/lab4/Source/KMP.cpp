#include <iostream>
#include <vector>
#define SHOW_DEBUG
//KMP
std::vector<int> prefix(std::string str){
    std::vector<int> p(str.length()); //в массиве p хранится значение префик функции

    p[0] = 0; //начальная инициализация
    int j = 0;
    int i = 1;
#ifdef SHOW_DEBUG
    std::cout<<"<Prefix function:>\n First entry: "<<str[0] << " = 0"<<"\n";
#endif
    while (i < str.length()){ //просмотрт всей строки
        if(str[i] == str[j]){ //символы совпадают
#ifdef SHOW_DEBUG
            std::cout << "Symbol "<< str[i] << " is equal to " << str[j] << std::endl;
#endif
            p[i] = j + 1; //префикс увеличивается
#ifdef SHOW_DEBUG
            std::cout << "Prefix++ "<< str[i] << " = " << p[i] << "\n";
#endif
            i++;
            j++;
        } else if (j == 0){  //совпадений ещё не было
            p[i] = 0;
#ifdef SHOW_DEBUG
            std::cout << "Symbol "<< str[i] << "is not equal to " << str[0] <<" and j = 0 "<< std::endl;
            std::cout << "Prefix "<< str[i] << " = " << p[i] << std::endl;
#endif
            i++;
        } else { //нет совпадений и j != 0
#ifdef SHOW_DEBUG
            std::cout << "Symbol "<< str[i] << " not equal to " << str[j] <<" and j != 0 "<< std::endl;
#endif
            j = p[j - 1];
#ifdef SHOW_DEBUG
            std::cout << "Ind j "<< str[i] << " = " << j << std::endl;
#endif
        }
    }
#ifdef SHOW_DEBUG
    std::cout << "Prefix fynction: ";
    for(const auto& e : p)
        std::cout << e;
    std::cout<<"\n-----------------------" << std::endl<< std::endl;
#endif
    return p;
}

std::vector<int> KMP(std::string str, std::string substr){
    std::vector<int> result;
    std::vector<int> p = prefix(substr); //вычисление префикса для подстроки
    int s = 0; //индекс для строки
    int sb = 0; //индекс для подстроки
#ifdef SHOW_DEBUG
    std::cout<<"<KMP function:>\n";
#endif
    while(s < str.length()) { //поиск по всей строке
        if(str[s] == substr[sb]) { //символы совпадают
#ifdef SHOW_DEBUG
            std::cout << "Symbol " << str[s] << " is equal to " << substr[sb] << std::endl;
#endif
            s++;
            sb++;
            if (sb == substr.length()) // если sb == длине подстроки, то отбраз найден
                result.push_back(s - substr.length()); //возвращем индекс начала вхождения
        } else if (sb != 0) {
#ifdef SHOW_DEBUG
            std::cout << "Symbol " << str[s] << " is not equal to " << substr[sb] << std::endl;
#endif
            sb = p[sb - 1]; //возврат к предыдущему символу прфикса в подстроке
#ifdef SHOW_DEBUG
            std::cout << "Return to previous substr symbol" << std::endl;
#endif
        }
        else { //символы не совпадают
#ifdef SHOW_DEBUG
            std::cout << "Symbol is not equal, string_ind++ \n";
#endif
            s++;
        }
    }

    return result;
}
//abababcab abababababcab
int main() {
    std::string substr, str;
    std::cin >> substr>> str;

    std::vector<int> result = KMP(str, substr);
    if (result.size() == 0) {
#ifdef SHOW_DEBUG
        std::cout<<"\n Substr " <<substr << "is not include in str " << str << "\n";
#endif
        std::cout << "-1";
    }
    for (int i = 0; i < result.size(); i++) {
#ifdef SHOW_DEBUG
        std::cout<<"\nEntry Start Indices " << substr << " in " << str<<"\n";
#endif
        std::cout << result[i];
        if(i != result.size() - 1)
            std::cout << ",";
    }

    return 0;
}
