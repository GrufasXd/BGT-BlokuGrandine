#include <string>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <chrono>
#include <set>

using namespace std;

string intihex(int num, int width = 8)
{
    stringstream ss;
    ss << hex << setw(width) << setfill('0') << num;
    return ss.str();
}

unsigned int mix(unsigned int a, unsigned int b)
{
    b *= 0xcc9e2d51;
    b = (b ^ (b >> 15));
    b *= 0x1b873593;

    a ^= b;
    a = (a ^ (a >> 16));
    a = a * 5 + 0xe6546b64;
    return a;
}

string hashfunction(const string& input)
{
    const int rnum = 4;
    unsigned int seedas = 0xdeadbeef;
    unsigned int temp = 0;

    for(size_t i = 0; i < input.size(); i++)
    {
        temp = input[i] * (i + 1);
        seedas = mix(seedas, temp);
    }

    for(int i = 0; i < rnum; i++)
    {
        seedas = mix(seedas, temp);
    }

    string hstring;
    for(int i = 0; i < 8; i++)
    {
        seedas = mix(seedas, temp);
        hstring += intihex(seedas);
    }

    return hstring.substr(0, 64);
}

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
    string generuotiID(){
        stringstream ss;
        ss << siuntejoRaktas << gavejoRaktas << suma;
        string sujungti = ss.str();

        return hashfunction(sujungti);
    }
};

class blokas{
    public:
    string prevblockhash, timestamp, merkle_root_hash, difficulty_target;
    int nonce;
    int versija;
    vector<transakcija> turinys;

    blokas(string prevhash, vector<transakcija> tur, int vers)
    {
        prevblockhash = prevhash;
        turinys = tur;
        versija = vers;
        timestamp = gautiLaika();
        difficulty_target = "00";
        nonce = 0;
        merkle_root_hash = rastiMerkleRoot();
    };

    string gautiLaika(){
        auto now = chrono::system_clock::now();
        auto in_time_t = chrono::system_clock::to_time_t(now);
        stringstream ss;
        ss << put_time(localtime(&in_time_t), "%Y-%m-%d %X");
        return ss.str();
    };

    string rastiMerkleRoot() {
        if (turinys.empty()) {
            return " ";
        }
        vector<string> hashai;
        for(int i = 0; i<turinys.size(); i++)
        {
            hashai.push_back(turinys[i].id);
        }

        while(hashai.size() > 1)
        {
            if(hashai.size() % 2 != 0)
            hashai.push_back(hashai.back());

            vector<string> naujihashai;
            for(size_t j = 0; j < hashai.size(); j+=2)
            {
                string combo = hashfunction(hashai[j] + hashai[j+1]);
                naujihashai.push_back(combo); 
            }
            hashai = naujihashai;
        }
        return hashai[0];
    }
    string kastiBloka()
    {
        string hashas;
        do{
            nonce++;
            stringstream ss;
            ss << prevblockhash << timestamp << versija << merkle_root_hash << difficulty_target << nonce;
            hashas = hashfunction(ss.str());
        }while (hashas.substr(0, difficulty_target.size()) != difficulty_target);
        return hashas;
    }

    void atnaujintBalansa(vector<user>& vartotojai) {
    for (auto& trans : turinys) {
        for (auto& user : vartotojai) {
            if (user.public_key == trans.siuntejoRaktas && user.balansas >= trans.suma) {
                user.balansas -= trans.suma; 
            }
            if (user.public_key == trans.gavejoRaktas) {
                user.balansas += trans.suma; 
            }
        }
    }
}

void rodytbloka() const
{
    cout << "Bloko hashas - " << prevblockhash << endl;
    cout << "Bloko versija - " << versija << endl;
    cout << "Laikas kada sukurtas blokas - " << timestamp << endl;
    cout << "Merkle root hashas - " << merkle_root_hash << endl;
    cout << "Bloko sudetingumas - " << difficulty_target << endl;
    cout << "Nonce reiksme - " << nonce << endl << endl;

};
};

void transakcijosInfo(const vector<transakcija>& visostransakcijos, const string transakcijosId)
{
    for (const auto& t : visostransakcijos) {
        if (t.id == transakcijosId) {
            cout << "Transakcijos ID - " << t.id << endl;
            cout << "Siuntejo Raktas - " << t.siuntejoRaktas << endl;
            cout << "Gavejo Raktas - " << t.gavejoRaktas << endl;
            cout << "Suma - " << t.suma << endl << endl;
            return;
        }
    }
    cout << "Transakcija su ID - " << transakcijosId << " nerasta." << endl;
};

int main(){
    srand(time(0));
    vector<user> vartotojai;
    vector<transakcija> transakcijos;
    vector<transakcija> visostransakcijos;
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

            nauja_transakcija.id = nauja_transakcija.generuotiID();
            transakcijos.push_back(nauja_transakcija);
            visostransakcijos.push_back(nauja_transakcija);
        }
    }

    vector<transakcija> genesisTr;
    vector<blokas> blokuGrandine;
    blokas genesisBlock("0", genesisTr, 0);
    genesisBlock.kastiBloka();
    blokuGrandine.push_back(genesisBlock);

    for(int i = 0; i<transakcijos.size(); i++)
    {
        vector<transakcija> blokuTransakcijos;
        set<int> pridetiIn;

        //renkam transakcijas
        for (int j = 0; j < 100 && !transakcijos.empty(); j++) {
            int randIndex = rand() % transakcijos.size();
            blokuTransakcijos.push_back(transakcijos[randIndex]);
            pridetiIn.insert(randIndex);
        }

        // naujo bloko hashas
        string prevblockhash = blokuGrandine.back().kastiBloka();
        // bloko kurimas
        blokas naujasBlokas(prevblockhash, blokuTransakcijos, i+1);
        naujasBlokas.kastiBloka();
        naujasBlokas.atnaujintBalansa(vartotojai);

        vector<transakcija> naujasVec;
        for (size_t j = 0; j < transakcijos.size(); j++) {
            if (pridetiIn.find(j) == pridetiIn.end()) {
                naujasVec.push_back(transakcijos[j]);
                }
            }
        transakcijos = naujasVec;
        blokuGrandine.push_back(naujasBlokas);
    }
    blokuGrandine[1].rodytbloka(); //skaiciu 1 ir 55 galima keisti norint perziureti kito bloko info
    blokuGrandine[55].rodytbloka();

    string tranakcijosID = visostransakcijos[555].id; //skaiciu 1 galima keisti norint patikrinti kita transakcija
    transakcijosInfo(visostransakcijos, tranakcijosID);
    
    return 0;
}
