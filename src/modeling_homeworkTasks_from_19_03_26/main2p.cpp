#include <iostream>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

random_device rd;
mt19937 gen(rd());

double random_double(double a, double b) {
    uniform_real_distribution<> dist(a, b);
    return dist(gen);
}

int random_int(int a, int b) {
    uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

bool bernoulli(double p) {
    return random_double(0, 1) < p;
}

// Задача 1: моделирование взрыва бочки
void task1(int trials, int n, double p, double p1) {
    cout << "\n=== Задача 1: Бочка с бензином ===\n";
    cout << "n = " << n << ", p = " << p << ", p1 = " << p1 << "\n";
    
    int explosions = 0;
    for (int t = 0; t < trials; ++t) {
        int hits = 0;
        for (int i = 0; i < n; ++i) {
            if (bernoulli(p)) hits++;
        }
        
        bool explodes = false;
        if (hits == 0) explodes = false;
        else if (hits == 1) explodes = bernoulli(p1);
        else explodes = true;
        
        if (explodes) explosions++;
        
        if (trials <= 20) {
            cout << "Испытание " << t+1 << ": попаданий " << hits;
            if (hits == 0) cout << " → не взорвалась\n";
            else if (hits == 1) cout << " → взрыв с вероятностью " << p1 << ": " << (explodes ? "ВЗРЫВ" : "НЕ ВЗОРВАЛАСЬ") << "\n";
            else cout << " → ВЗОРВАЛАСЬ (≥2 попаданий)\n";
        }
    }
    
    double empirical = (double)explosions / trials;
    double theoretical = 1 - pow(1-p, n) - n * p * pow(1-p, n-1) * (1 - p1);
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "эмпирическая вероятность взрыва: " << empirical << "\n";
    cout << "теоретическая вероятность: " << theoretical << "\n";
}

// Задача 2: монеты с двумя орлами
void task2(int trials, int n, int k) {
    cout << "\n=== Задача 2: Монеты (k двуорловых из n) ===\n";
    cout << "n = " << n << ", k = " << k << "\n";
    
    int success = 0;
    int condition = 0; 
    
    for (int t = 0; t < trials; ++t) {
        bool double_headed = (random_int(1, n) <= k);
        
        vector<bool> flips(4);
        for (int i = 0; i < 4; ++i) {
            if (double_headed) {
                flips[i] = true;
            } else {
                flips[i] = bernoulli(0.5);
            }
        }
        
        bool first_three_heads = flips[0] && flips[1] && flips[2];
        if (first_three_heads) {
            condition++;
            if (flips[3]) success++;
        }
        
        if (trials <= 20) {
            cout << "Испытание " << t+1 << ": монета " << (double_headed ? "двуорловая" : "обычная");
            cout << ", броски: ";
            for (bool f : flips) cout << (f ? "О" : "Р");
            cout << " → первые три " << (first_three_heads ? "орлы" : "не орлы");
            if (first_three_heads) cout << ", 4-й " << (flips[3] ? "орёл" : "решка");
            cout << "\n";
        }
    }
    
    double empirical = (condition > 0) ? (double)success / condition : 0;
    double theoretical = (double)(15*k + n) / (2*(7*k + n));
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "первые три орла: " << condition << " раз\n";
    cout << "условная вероятность (4-й орёл | первые три орла): " << empirical << "\n";
    cout << "теоретическая вероятность: " << theoretical << "\n";
}

int main() {
    int trials, task;
    
    cout << "Введите количество испытаний: ";
    cin >> trials;
    cout << "Выберите задачу (1 или 2): ";
    cin >> task;
    
    if (task == 1) {
        int n;
        double p, p1;
        cout << "Введите n (количество выстрелов): ";
        cin >> n;
        cout << "Введите p (вероятность попадания одной пули): ";
        cin >> p;
        cout << "Введите p1 (вероятность взрыва при одном попадании): ";
        cin >> p1;
        task1(trials, n, p, p1);
    } 
    else if (task == 2) {
        int n, k;
        cout << "Введите n (общее количество монет): ";
        cin >> n;
        cout << "Введите k (количество двуорловых монет): ";
        cin >> k;
        task2(trials, n, k);
    }
    else {
        cout << "Неверный номер задачи\n";
    }
    
    return 0;
}