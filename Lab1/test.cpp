#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>


using namespace std;

//Наборов - 7

int dimensions[7] = {100, 3125, 6250, 12500, 25000, 50000, 100000};

//Для сортировки rank, name, companyNumber
struct MilitaryOfficer {
    string name;
    string rank;
    int companyNumber;
    int age;

    MilitaryOfficer(string name, string rank, int companyNumber, int age)
    {
        this->name = name;
        this->rank = rank;
        this->companyNumber = companyNumber;
        this->age = age;
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
string forPlot = "points.txt";

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

vector<vector<MilitaryOfficer>> readInput() {
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
            MilitaryOfficer MO(name, rank, stoi(companyNumber), stoi(age));
            v.push_back(MO);
        }
        result.push_back(v);
    }

    return result;
}

vector<MilitaryOfficer> selectSort(vector<MilitaryOfficer> vec) {
    vector<MilitaryOfficer> a = vec;

    int min = 0;

    for (int i = 0; i < a.size(); i++)
    {
        min = i; 
        for (int j = i + 1; j < a.size(); j++){
            if(a[j] < a[min]){
                min = j;
            }
        }
        MilitaryOfficer temp = a[i];
        a[i] = a[min];
        a[min] = temp;
    }
    return a;
}

vector<MilitaryOfficer> insertionSort(vector<MilitaryOfficer> vec) {
    vector<MilitaryOfficer> a = vec;

    for (int i = 1; i < a.size(); i++)
    {
        MilitaryOfficer elem = a[i];
        int j = i-1;
        while (j >= 0 && a[j] > elem) {
            a[j+1] = a[j];
            j -= 1;
        }
        a[j+1] = elem;
    }
    return a;
}
void downHeap(vector<MilitaryOfficer> &vec, long k, long n){
    MilitaryOfficer new_elem = vec[k];
    long child;
    while(k <= n/2){
        child = 2*k;
        if(child < n && vec[child] < vec[child+1]){
            child++;
        }
		if(new_elem >= vec[child]) {
            break;
        } 
        else{
            vec[k] = vec[child];
            k = child;
        }
    }
    vec[k] = new_elem;
}

vector<MilitaryOfficer> heapSort(vector<MilitaryOfficer> vec, int size) {
    vector<MilitaryOfficer> arr = vec;
    for(long i=size/2 - 1; i >= 0; i--){
        downHeap(arr, i, size - 1);
    }
    for(int i = size - 1; i > 0; i--){
        MilitaryOfficer temp = arr[i];
        arr[i] = arr[0];
        arr[0] = temp;
        downHeap(arr, 0, i-1);
    }
    return arr;
}




void makeOutput(string type, vector<vector<MilitaryOfficer>>& sorted) {
    ofstream fout(output, ios::app);

    fout << type << "\n";

    for (int i = 0; i < 7; ++i) {
        fout << "Количество: " << sorted[i].size() << "\n";
        for (int j = 0; j < sorted[i].size(); ++j) {
            fout << sorted[i][j] << "\n";
        }
    }
}



void makePoints(string type, vector<std::chrono::steady_clock::time_point> plotPoints) {
    ofstream fout(forPlot, ios::app);

    fout << type << "\n";

    for (int i = 0; i < 7; ++i)
        fout << "Количество значений: " << dimensions[i] << "; Время для их сортировки, мс: " << chrono::duration_cast<chrono::milliseconds>(plotPoints[i+1] - plotPoints[i]).count() << "\n";

    fout << "\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    makeInput();

    vector<vector<MilitaryOfficer>> initialArray = readInput();
    vector<vector<MilitaryOfficer>> selectSortRes, insertionSortRes, heapSortRes;

    vector<std::chrono::steady_clock::time_point> plotPoints;

    plotPoints.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < 7; ++i) {
        selectSortRes.push_back(selectSort(initialArray[i]));
        plotPoints.push_back(std::chrono::steady_clock::now());
    }

    makeOutput("======= Сортировка выбором: =======\n", selectSortRes);
    makePoints("======= Сортировка выбором: =======\n", plotPoints);
    plotPoints.clear();

    plotPoints.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < 7; ++i) {
        insertionSortRes.push_back(insertionSort(initialArray[i]));
        plotPoints.push_back(std::chrono::steady_clock::now());
    }

    makeOutput("======= Сортировка простыми вставками: =======\n", insertionSortRes);
    makePoints("======= Сортировка простыми вставками: =======\n", plotPoints);
    plotPoints.clear();

    plotPoints.push_back(std::chrono::steady_clock::now());

    for (int i = 0; i < 7; ++i) {
        heapSortRes.push_back(heapSort(initialArray[i], dimensions[i]));
        plotPoints.push_back(std::chrono::steady_clock::now());
    }

    makeOutput("======= Пирамидальная сортировка: =======\n", heapSortRes);
    makePoints("======= Пирамидальная сортировка: =======\n", plotPoints);
    plotPoints.clear(); 


    return 0;
}
