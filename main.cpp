#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

using namespace std;

string vardurinkejas(){
    vector<string> vardai = {
        "Jonas", "Antanas", "Petras", "Mantas", "Dainius", "Vytautas", "Tomas", 
        "Giedrius", "Algirdas", "Mindaugas", "Rokas", "Andrius", "Arunas", 
        "Kestutis", "Lukas", "Darius", "Rimantas", "Julius", "Saulius", 
        "Justinas", "Simas", "Aivaras", "Arvydas", "Dominykas", "Laurynas",
        "Raimundas", "Martynas", "Paulius", "Egidijus", "Ignas", "Marius", 
        "Rytis", "Vytenis", "Arturas", "Eimantas", "Evaldas", "Gediminas",
        "Irmantas", "Kornelijus", "Nerijus", "Sarunas", "Sigitas", "Tautvydas", 
        "Tautvydas", "Vaclovas", "Vilius", "Vygintas", "Zygimantas", "Erikas", 
        "Vaidas", "Jokubas", "Karolis", "Kristijonas", "Vainius", "Arunas",
        "Jurgis", "Kazimieras", "Kazys", "Maksas", "Norbertas", "Romas",
        "Silvestras", "Vincas", "Vytis", "Dovydas", "Eugenijus", "Gabrielius",
        "Henrikas", "Ivanas", "Juozas", "Kazimieras", "Leonardas", "Laurynas", 
        "Mykolas", "Povilas", "Romanas", "Titas", "Vaidotas", "Viktoras", 
        "Adomas", "Benas", "Deividas", "Eimantas", "Gintas", "Haroldas", 
        "Irmandas", "Kipras", "Leonas", "Mindaugas", "Petras", "Raimundas",
        "Simonas", "Tadas", "Uosis", "Valentas", "Valdas", "Zydrunas"
    };
    int randomIndex = rand() % vardai.size();
    return vardai[randomIndex];
};

class user{
    public:
    string vardas;
    string public_key;
    double balansas;

    user()
        : vardas(""), public_key(""), balansas(0.0){} 

    double balansokurimas(){
        balansas = 100 + (rand() % 1000001);
        return balansas;
    };

    string raktugeneratorius(){
        stringstream ss;
        for(int i = 0; i<64; i++){
            int val = rand() % 16;
            ss << hex << val;
        }
        return ss.str();
    }

    void kurtiVartotojus(){
    vardas = vardurinkejas();
    public_key = raktugeneratorius();
    balansas = balansokurimas();
    }
};

class transakcija{
    public:
    string id;
    string siuntejoRaktas;
    string gavejoRaktas;
    double suma;
    transakcija()
        : id(""), siuntejoRaktas(""), gavejoRaktas(""), suma(0.0){} 
};

int main(){
    srand(time(0));
    vector<user> vartotojai;
    vector<transakcija> transakcijos;
    for(int i = 0; i<1000; i++)
    {
        user naujasvartotojas;
        naujasvartotojas.kurtiVartotojus();
        vartotojai.push_back(naujasvartotojas);
    }
    for(int i = 0; i<10000; i++)
    {
        int randomIndex1 = rand() % vartotojai.size();
        int randomIndex2 = rand() % vartotojai.size();
        while (randomIndex1 == randomIndex2) {
            randomIndex2 = rand() % vartotojai.size();
        }

        int siuntejoIndeksas = (vartotojai[randomIndex1].balansas > vartotojai[randomIndex2].balansas) ? randomIndex1 : randomIndex2;
        int gavejoindeksas = (siuntejoIndeksas == randomIndex1) ? randomIndex2 : randomIndex1;

        if(vartotojai[siuntejoIndeksas].balansas > 0)
        {
            transakcija nauja_transakcija;

            nauja_transakcija.siuntejoRaktas = vartotojai[siuntejoIndeksas].public_key;
            nauja_transakcija.gavejoRaktas = vartotojai[gavejoindeksas].public_key;
            nauja_transakcija.suma = (rand() % static_cast<int> (vartotojai[siuntejoIndeksas].balansas)) + 1;

            vartotojai[siuntejoIndeksas].balansas -= nauja_transakcija.suma;
            vartotojai[gavejoindeksas].balansas += nauja_transakcija.suma;

            transakcijos.push_back(nauja_transakcija);
        }
    }
    cout << vartotojai[2].vardas << endl;
    cout << vartotojai[2].public_key << endl;
    cout << vartotojai[2].balansas << endl;
    return 0;
}
