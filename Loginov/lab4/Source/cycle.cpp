#include <iostream>
#include <string>
#include <vector>

#define SHOW_DEBUG
//cycle
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
            std::cout << "Symbol "<< str[i] << " not equal to " << str[0] <<" and j = 0 "<< std::endl;
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


int sycleShift(std::string str1, std::string str2) {

    int ind_str1=0, ind_str2 =0;
    if (str1.length() != str2.length()) { //длины строк отличаются
#ifdef SHOW_DEBUG
        std::cout << "Lengths not equal: " << str1.length() << "!=" << str2.length() << std::endl;
#endif
        return -1;
    }
    int lap = 0;
    std::vector<int> p = prefix(str2);

    while (true) {
        if (str1[ind_str1] == str2[ind_str2]) { //символы совпадают
#ifdef SHOW_DEBUG
            std::cout << "Symbol " << str1[ind_str1] << " is equal to " <<str2[ind_str2] <<"\n";
#endif
            ind_str1++;
            ind_str2++;
        }
        if (ind_str1 == str1.length()) {
            ind_str1 = 0;
            lap++;
        }
        if (ind_str2 == str2.length()) {
#ifdef SHOW_DEBUG
            std::cout << "Cycle was founded\n";
#endif
            return ind_str1;
        }
        else if(str1[ind_str1] != str2[ind_str2] && ind_str2 < str1.length()){
#ifdef SHOW_DEBUG
            std::cout << "Symbol " << str1[ind_str1] << " is not equal to " <<str2[ind_str2] <<"\n";
#endif
            if(ind_str2 == 0)
                ind_str1++;
            else
                ind_str2 = p[ind_str2 - 1];
        }
        if(lap > 1)
            break;
    }
    return -1;
}

//defabc abcdef
//ababab bababa
int main() {
    std::string str1, str2;
    std::cin>>str1>>str2;
    std::cout<<sycleShift(str1, str2);
    // auto p = prefix("efefeftef");

    return 0;

}




