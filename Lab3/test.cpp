#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <map>
#include <set>

using namespace std;

int dimensions[7] = {100, 3125, 6250, 12500, 25000, 50000, 100000};
int maxHash = 750000;
vector<size_t> powers(500);
int goodPrimeNumber = 41;

unsigned int simpleHash(string key) {
    unsigned int result = 1;

    for (int i = 0; i < key.length(); i++)
        result = (result * (key[i] - 'a' + 1)) % maxHash;

    return result % maxHash;
}

unsigned int difficultHash(string key) {
    unsigned int hashValue = 0;
    for (size_t j = 0; j < key.length(); j++)
        hashValue = (hashValue + (key[j] - 'a' + 1) * powers[j]);

    return hashValue % maxHash;
}

struct MilitaryOfficer {
    string name;
    string rank;
    int companyNumber;
    int age;
    unsigned int (*hashFunc)(string);
    unsigned int hashValue;

    MilitaryOfficer(string name, string rank, int companyNumber, int age)
    {
        this->name = name;
        this->rank = rank;
        this->companyNumber = companyNumber;
        this->age = age;
        this->hashFunc = simpleHash;
        this->hashValue = simpleHash(this->name);
    }
    MilitaryOfficer(string name, string rank, int companyNumber, int age, unsigned int (*hashFunc)(string))
    {
        this->name = name;
        this->rank = rank;
        this->companyNumber = companyNumber;
        this->age = age;
        this->hashFunc = hashFunc;
        this->hashValue = hashFunc(name);
    }

    friend bool operator== (const MilitaryOfficer& MO1, const MilitaryOfficer& MO2) {
        if (MO1.rank == MO2.rank && MO1.name == MO2.name && MO1.companyNumber == MO2.companyNumber)
            return true;
        return false;
    }

    friend bool operator< (const MilitaryOfficer& MO1, const MilitaryOfficer& MO2) {
        if (MO1.rank < MO2.rank || ((MO1.rank == MO2.rank) && (MO1.name < MO2.name)) || 
            ((MO1.rank == MO2.rank) && (MO1.name == MO2.name) && (MO1.companyNumber < MO2.companyNumber)))
            return true;
        return false;
    }

    friend bool operator> (const MilitaryOfficer& MO1, const MilitaryOfficer& MO2) {
        if (!(MO1 < MO2) && !(MO1 == MO2))
            return true;
        return false;
    }

    friend bool operator<= (const MilitaryOfficer& MO1, const MilitaryOfficer& MO2) {
        if (MO1 < MO2 || MO1 == MO2)
            return true;
        return false;
    }

    friend bool operator>= (const MilitaryOfficer& MO1, const MilitaryOfficer& MO2) {
        if (!(MO1 < MO2))
            return true;
        return false;
    }


    friend ostream& operator<<(ostream& out, const MilitaryOfficer& MO1) {
        out << MO1.rank << ' ' << MO1.name << ' ' << MO1.companyNumber << '\n';
        return out;
    }

    unsigned int getHashValue() const {
        return this->hashValue;
    }

    string getName() const {
        return this->name;
    }
};

string giveRank(){
    std::string ranks_string[]{"Officer cadet", "Junior lieutenant", "Lieutenant", "Senior lieutenant", "Captain",
                 "Major", "Lieutenant colonel", "Colonel", "Major general", "Lieutenant general",
                 "Colonel general", "Army general", "Marshal of the Russian Federation"};
    return ranks_string[rand() % 13];
}

string giveName() {
    static const char capital[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    static const char notCapital[] = "abcdefghijklmnopqrstuvwxyz";
    string result;
    int number = sizeof(capital) - 1;
    int surnameLength = rand() % 9 + 2;
    int nameLength = rand() % 7 + 2;
    int patronymicLength = rand() % 9 + 2;
    result.reserve(surnameLength + nameLength + patronymicLength);
    for (int i = 0; i < surnameLength; ++i) {
        if (i == 0){
            result += capital[rand() % number];
            continue;
        }
        result += notCapital[rand() % number];
    }
    for (int i = 0; i < nameLength; ++i) {
        if (i == 0){
            result += capital[rand() % number];
            continue;
        }
        result += notCapital[rand() % number];
    }
    for (int i = 0; i < patronymicLength; ++i) {
        if (i == 0){
            result += capital[rand() % number];
            continue;
        }
        result += notCapital[rand() % number];
    }
    return result;
}

string input = "input.txt";
string output = "output.txt";

void makeInput() {
    srand((unsigned)time(NULL));
    ofstream fout(input);

    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < dimensions[i]; ++j) {
            fout << giveName() << "\n";  //ФИО
            fout << giveRank() << "\n";  //Звание
            fout << rand() % (100 + 1)  << "\n";  //Номер роты
            fout << rand() % (100 + 18) << "\n";  //Возраст
        }
    }
}

vector<vector<MilitaryOfficer>> readInput(unsigned int (*hashFunc)(string)) {
    ifstream fin(input);

    vector<vector<MilitaryOfficer>> result;

    string name, rank, companyNumber, age;

    for (int i = 0; i < 7; ++i) {
        vector<MilitaryOfficer> v;
        v.reserve(dimensions[i]);
        for (int j = 0; j < dimensions[i]; ++j) {
            getline(fin, name);
            getline(fin, rank);
            getline(fin, companyNumber);
            getline(fin, age);
            MilitaryOfficer MO(name, rank, stoi(companyNumber), stoi(age), hashFunc);
            v.push_back(MO);
        }
        result.push_back(v);
    }

    return result;
}

class HashTable {
    private:
    vector<vector<string>> table;

    public:
    HashTable() {
        table.resize(maxHash);
    }

    void fill(const MilitaryOfficer& item) {
        unsigned int hashValue = item.getHashValue();

        vector<string>& values = table[hashValue];

        if (std::find(values.begin(), values.end(), item.getName()) != values.end()) {
            return;
        }

        values.push_back(item.getName());
    }

    vector<string> find(const MilitaryOfficer& item) {
        unsigned int hashValue = item.getHashValue();

        vector<string>& values = table[hashValue];

        return values;
    }

    void clear() {

        table.clear();
        table.resize(maxHash);
    }

    unsigned int collisions() {
        unsigned int result = 0;
        for (const auto& values : table) {
            if (values.size() > 1) {
                result += values.size() - 1;
            }
        }
    return result;
}
};

int main()
{
    setlocale(LC_ALL, "Russian");
    ofstream fout(output);
    makeInput();
    powers[0] = 1;
    for (size_t i = 1; i < powers.size(); i++)
        powers[i] = powers[i - 1] * goodPrimeNumber;

    vector<vector<MilitaryOfficer>> simpleArray = readInput(simpleHash);

    vector<MilitaryOfficer> keys;

    for (int i = 0; i < 7; ++i) {
        keys.push_back(simpleArray[i][rand() % simpleArray[i].size()]);
    }
    HashTable simpleTable;

    std::chrono::steady_clock::time_point begin, finish;

    fout << "Простая хэш-функция:\n";

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < simpleArray[i].size(); j++){
            simpleTable.fill(simpleArray[i][j]);
        }

        begin = std::chrono::steady_clock::now();
        vector<string> found = simpleTable.find(keys[i]);
        finish = std::chrono::steady_clock::now();

        fout << "На поиск в таблице с кол-вом элементов " << simpleArray[i].size() << " ушло "  << chrono::duration_cast<chrono::nanoseconds>(finish - begin).count() << " наносекунд\n";
        fout << "Коллизий: " << simpleTable.collisions() << "\n\n"; 
        simpleTable.clear();
        begin = finish;
    }

    simpleArray.clear();

    vector<vector<MilitaryOfficer>> difficultArray = readInput(difficultHash);

    HashTable difficultTable;

    fout << "Сложная хэш-функция:\n";

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < difficultArray[i].size(); j++){
            difficultTable.fill(difficultArray[i][j]);
        }

        begin = std::chrono::steady_clock::now();
        vector<string> found= difficultTable.find(keys[i]);
        finish = std::chrono::steady_clock::now();

        fout << "На поиск в таблице с кол-вом элементов " << difficultArray[i].size() << " ушло "  << chrono::duration_cast<chrono::nanoseconds>(finish - begin).count() << " наносекунд\n";
        fout << "Коллизий: " << difficultTable.collisions() << "\n\n"; 
        difficultTable.clear();
        begin = finish;
    }

    return 0;
}

