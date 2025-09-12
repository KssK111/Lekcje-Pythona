#include <iostream>
#include <string>
#include <vector>
#include <variant>
#include <fstream>
#include <print>
#include <sstream>
#include <cassert>

using namespace std;

bool czy_nalezy(pair<int, int> C, variant<vector<pair<int, int>>, pair<int, int>> Figura)
{
    return visit([C](auto&& figura) -> bool
    {
        using T = decay_t<decltype(figura)>;
        if constexpr(is_same_v<pair<int, int>, T>)
            return C == figura;
        else if constexpr(is_same_v<vector<pair<int, int>>, T>)
        {
            if(figura.size() == 1)
                return C == figura[0];
            auto czy_nalezzzy = [C](pair<int, int> A, pair<int, int> B)
            { return (B.first - A.first) * (C.second - A.second) == (B.second - A.second) * (C.first - A.first); };
            for(size_t i = 0; i < figura.size() - 1; i++)
            {
                auto A = figura[i];
                auto B = figura[i + 1];
                if(czy_nalezzzy(A, B))
                    return true;
            }
            return false;
        }
    }, Figura);
}

vector<vector<pair<int, int>>> wczytaj_figury(string plik)
{
    vector<vector<pair<int, int>>> wynik;
    string linia;
    ifstream pliczek(plik);
    vector<pair<int, int>> wynik_temp;
    while(getline(pliczek, linia))
    {
        auto start = std::find_if_not(linia.begin(), linia.end(), [](unsigned char c){ return std::isspace(c); }); //Może można to zrobić używając istringstream, tak jak niżej
        auto end = std::find_if_not(linia.rbegin(), linia.rend(), [](unsigned char c){ return std::isspace(c); }).base(); //ale trudno, już niech będzie tak
        linia = string(start, end);
        if(linia.contains(" "))
        {
            istringstream stream(linia);
            int A, B;
            stream >> A >> B;
            wynik_temp.push_back(make_pair(A, B));
        }
        else if(!wynik_temp.empty())
        {
            wynik.push_back(wynik_temp);
            wynik_temp.clear();
        }
    }
    if(!wynik_temp.empty())
        wynik.push_back(wynik_temp);
    return wynik;
}

int main()
{
    auto figury = wczytaj_figury("dane.txt");
    for(auto figura : figury)
    {
        for(auto punkt : figura)
        {
            println("A = {}, B = {}", punkt.first, punkt.second);
        }
        print("\n");
    }
    for(auto figura : figury)
        println("{}", czy_nalezy(make_pair(3, 2), figura));
    assert(czy_nalezy(make_pair(5, 5), make_pair(5, 5)));
    assert(czy_nalezy(make_pair(5, 5), vector<pair<int,int>> { { 5, 5 } }));
    return EXIT_SUCCESS;
}