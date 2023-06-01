#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>

using namespace std;

//Наборов - 7

int dimensions[7] = {100, 3125, 6250, 12500, 25000, 50000, 100000};

//Первое не числовое поле - name
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
    
    string getName() const {
        return name;
    }

    friend ostream& operator<<(ostream& out, const MilitaryOfficer& MO1) {
        out << MO1.rank << ' ' << MO1.name << ' ' << MO1.companyNumber << '\n';
        return out;
    }
};

vector<vector<MilitaryOfficer>> readInput() {
    ifstream fin("input.txt");

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

//Фильтрация - по имени
void downHeap(vector<MilitaryOfficer> &vec, long k, long n){
    MilitaryOfficer new_elem = vec[k];
    long child;
    while(k <= n/2){
        child = 2*k;
        if(child < n && vec[child].getName() < vec[child+1].getName()){
            child++;
        }
		if(new_elem.getName() >= vec[child].getName()) {
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

vector<int> linearSearch (vector<MilitaryOfficer>& vec, string key) {
    vector<int> result;
    for (int i = 0; i < vec.size(); i++){
        if (vec[i].getName() == key) {
            result.push_back(i);
        }
    }
    return result;
}
pair<int, int> binarySearch(vector<MilitaryOfficer>& vec, string key){
    int start, end;
    int middle = -1;
    int begin = 0;
    int finish = vec.size() - 1;
    bool found = 0;
    while(begin <= finish){
        middle = (begin + finish) / 2;
        found = (vec[middle].getName() == key);
        if ((begin == finish) && !found) break;
        if(found){
            break;
        } else if (vec[middle].getName() < key) {
            begin = middle + 1;
        } else {
            finish = middle - 1;
        }
    }
    if (found){
        start = middle;
        end = middle;
        while (end < vec.size() - 1 && vec[end + 1].getName() == key) end++;
        while (start > 0 && vec[start - 1].getName() == key) start--;
        return make_pair(start, end);
    }
    start = -1;
    end = -1;
    return make_pair(start, end);
}





//Ради тестирования нахождения нескольких элементов в выборку размера 100 был вручную добавлен элемент с повторяющимся именем, а также зафиксирован соответствующий ключ
int main()
{
    setlocale(LC_ALL, "Russian");
    vector<vector<MilitaryOfficer>> initialArray = readInput();
    vector<vector<MilitaryOfficer>> heapSortRes;

    vector<string> keys = {"EngfytbmgjIisMyomjqg"};
    string output = "output.txt";
    for (int i = 1; i < 7; ++i) {
        keys.push_back(initialArray[i][rand() % initialArray[i].size()].getName());
    }

    std::chrono::steady_clock::time_point begin, finish;
    ofstream fout(output);
    fout << "Линейный поиск:\n\n";
    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 7; i++) {
        vector<int> linearResult = linearSearch(initialArray[i], keys[i]);
        finish = std::chrono::steady_clock::now();
        fout << "Искомый элемент: " << keys[i] << endl;
        if (!linearResult.empty()){
            for (int j = 0; j < linearResult.size(); ++j){
                fout << "Найден элемент " << initialArray[i][linearResult[j]] << "на позиции: " << linearResult[j]  << "\n";
            }
        }
        fout << "На линейный поиск в массиве размерности " << dimensions[i] << " ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд" << "\n\n";
        begin = finish;
        
    }
    
    fout << "Двоичный поиск + сортировка:\n\n";

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 7; i++) {
        vector<MilitaryOfficer> temp = heapSort(initialArray[i], dimensions[i]);
        pair<int, int> binaryResult = binarySearch(temp, keys[i]);
        finish = std::chrono::steady_clock::now();
        heapSortRes.push_back(temp);
        fout << "Искомый элемент: " << keys[i] << endl;
        if (binaryResult.first != -1){
            fout << "Найден элемент " << heapSortRes[i][binaryResult.first] << "на позицииях с " << binaryResult.first << " по " << binaryResult.second << "\n";
        }
        fout << "На двоичный поиск, вместе с сортировкой, в массиве размерности " << dimensions[i] << " ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд"  << "\n\n";
        begin = finish;
    }

    fout << "Двоичный поиск в уже отсортированном массиве:\n\n";

    begin = std::chrono::steady_clock::now();
    for (int i = 0; i < 7; ++i) {
        pair<int, int> binaryResult = binarySearch(heapSortRes[i], keys[i]);
      
        finish = std::chrono::steady_clock::now();
        fout << "Искомый элемент: " << keys[i] << endl;
        if (binaryResult.first != -1){
            fout << "Найден элемент " << heapSortRes[i][binaryResult.first] << "на позицииях с " << binaryResult.first << " по " << binaryResult.second << "\n";
        }
        fout << "На двоичный поиск в уже отсортированном массиве размерности " << dimensions[i] << " ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд"  << "\n\n";
        begin = finish;
    }
    vector<multimap<string, MilitaryOfficer>> MOs;

    for (int i = 0; i < 7; ++i) {
        multimap<string, MilitaryOfficer> temp;
        for (int j = 0; j < initialArray[i].size(); ++j)
            temp.insert({ initialArray[i][j].getName(), initialArray[i][j]});
        MOs.push_back(temp);
    }

    fout << "Поиск в ассоциативном массиве:\n\n";

    begin = std::chrono::steady_clock::now();

    for (int i = 0; i < 7; ++i) {
        auto bounds = MOs[i].equal_range(keys[i]);
        finish = std::chrono::steady_clock::now();
        fout << "Искомый элемент: " << keys[i] << "\n";
        if (bounds.first != MOs[i].end())
        for (auto it = bounds.first; it != bounds.second; ++it)
            fout << "Найден элемент: " << it->second;
        fout << "На поиск в ассоциативном массиве размерности " << dimensions[i] << " ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд"  << "\n\n";
        begin = finish;
    }


    return 0;
}
