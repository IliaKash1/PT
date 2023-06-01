#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <cmath>

using namespace std;
int dimensions[10] = {1000, 3906, 7812, 15625, 31250, 62500, 125000, 250000, 500000, 1000000};
size_t maxValue = 1 << 25;
//Использует сразу несколькo комбинаций популярных параметров
class ModifiedLCG {
    public:
    ModifiedLCG(size_t seed) {
        x = seed;
    }
    size_t next() {
        x = (a * x + c) % m;
        x = (134775813 * x + 1) % ((size_t)1 << 32);
        x = (1103515245 * x + 12345) % ((size_t)1 << 31);
        return x % maxValue;
}

private:
    size_t a = 25214903917;
    size_t c = 11;
    size_t m = (size_t)1 << 48;
    size_t x;
};

//Комбинация двух алгоритмов
class PhiloxAndRANLUX{
    public:
    PhiloxAndRANLUX(size_t seed) {
        x = seed;
        y = seed ^ 0x9e3779b97f4a7c15;
        z = seed ^ 0x9e3779b97f4a7c15 ^ 0x9e3779b97f4a7c15;
    }
    size_t next() {
        //Philox4x32
        size_t temp = x ^ (x << 11);
        x = y;
        y = z;
        z = temp ^ (temp >> 19) ^ (y >> 8);
        //RANLUX
        size_t a, b, c,d;
        a = z;
        b = z + 1;
        c = z + 2;
        d = z + 3; 

        size_t a_new = a * 25214903917 + 11;
        size_t b_new = b * 1566083941 + 12345;
        size_t c_new = c * 18014398509481984 + 123456789;
        size_t d_new = d * 6364136261768736481 + 1234567890123456789;

        a = b;
        b = c;
        c = d;
        d = a_new ^ (b_new << 11) ^ (c_new << 17) ^ (d_new << 13);
        return d % maxValue; 
}

private:
    size_t x = 25214903917;
    size_t y = 11;
    size_t z = (size_t)1 << 48;
};

double mean(vector<size_t>& vec){
    double sum = 0;
    for (double number : vec){
        sum += number;        
    }
    return sum / vec.size();
}

double deviation(size_t mean, vector<size_t>& vec) {
    double sum = 0;

    for (double number : vec){
        sum += (number - mean) * (number - mean);
    }

    return sqrt(sum / vec.size());
}

double variationCoeff(double deviation, double mean) {
    return deviation / mean;
}

double chiSquare(vector<size_t>& vec) {
    //Нормируем числа
    vector<double> norm;
    for (int i = 0; i < vec.size(); ++i)
        norm.push_back(vec[i] / static_cast<double>(maxValue - 1));

    const size_t n = norm.size();
    const size_t k = 1 + 3.322 * log(n);
    const double p = 1 / static_cast<double>(k);

    vector<unsigned int> intervalCount(k, 0);

    for (auto number : norm)
        for (int j = 0; j < k; ++j)
            if ((p * j <= number) && (p * (j + 1) > number))
                intervalCount[j] += 1;

    double chi = 0;
//    for(auto elem : intervalCount){
//        cout << elem << ' ';    
//    }
    //суммирование
    for (auto count : intervalCount)
        chi += ((static_cast<double>(count) - p * n) * (static_cast<double>(count) - p * n)) / (p * n);
    return chi;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((unsigned)time(NULL));
    vector<int> seeds = {rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand(), rand()};
    vector<vector<size_t>> rand1;
    vector<vector<size_t>> rand2;
    vector<vector<size_t>> rand3;
    std::chrono::steady_clock::time_point begin, finish;
    //Засекаем время генерации
    for (int i = 0; i < 10; i++) {
        ModifiedLCG random(seeds[i]);
        PhiloxAndRANLUX random1(seeds[i]);
        std::vector<size_t> numbers1, numbers2, temp;

        begin = std::chrono::steady_clock::now();
        for(int j = 0; j < dimensions[i]; j++){
            numbers1.push_back(random.next());
        }
        finish = std::chrono::steady_clock::now();
        cout << "На генерацию " << dimensions[i] << " чисел с помощью ModifiedLCG ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд" << "\n\n";

        begin = std::chrono::steady_clock::now();
        for(int j = 0; j < dimensions[i]; j++){
            numbers2.push_back(random1.next());
        }
        finish = std::chrono::steady_clock::now();
        cout << "На генерацию " << dimensions[i] << " чисел с помощью PhiloxAndRANLUX ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд" << "\n\n";

        begin = std::chrono::steady_clock::now();
        for(int j = 0; j < dimensions[i]; j++){
            temp.push_back(rand() % (1 << 25) + 0);
        }
        finish = std::chrono::steady_clock::now();
        cout << "На генерацию " << dimensions[i] << " чисел с помощью rand() (т.е. LCG) ушло: " << chrono::duration_cast<chrono::microseconds>(finish - begin).count() << ", микросекунд" << "\n\n\n";
        rand1.push_back(numbers1);
        rand2.push_back(numbers2);
    }
    cout << "Метод, количество элементов, среднее, отклонение, коэффициент вариации, Хи-квадрат\n\n";
    for (int i = 0; i < rand1.size(); i++) {
        cout << "ModifiedLCG,     " << dimensions[i] << ", " << mean(rand1[i]) << ", " << deviation(mean(rand1[i]), rand1[i]) << ", " << variationCoeff(deviation(mean(rand1[i]), rand1[i]), mean(rand1[i])) << ", " << chiSquare(rand1[i]) << "\n";
        cout << "PhiloxAndRANLUX, " << dimensions[i] << ", " << mean(rand2[i]) << ", " << deviation(mean(rand2[i]), rand2[i]) << ", " << variationCoeff(deviation(mean(rand2[i]), rand2[i]), mean(rand2[i])) << ", " << chiSquare(rand2[i]) << "\n";
    }
    return 0;
}
