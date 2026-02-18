#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;

random_device rd;
mt19937 gen(rd());

int random_int(int a, int b) {
    uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

double random_double(double a, double b) {
    uniform_real_distribution<> dist(a, b);
    return dist(gen);
}

void task6(int trials) {
    cout << "\n=== Задача 6: Куб 10x10x10 ===\n";
    int c1 = 0, c2 = 0, c3 = 0;
    for (int t = 0; t < trials; ++t) {
        int x = random_int(0, 9);
        int y = random_int(0, 9);
        int z = random_int(0, 9);
        int painted = (x == 0 || x == 9) + (y == 0 || y == 9) + (z == 0 || z == 9);
        
        cout << "Испытание " << t+1 << ": кубик (" << x << "," << y << "," << z 
             << ") окрашенных граней: " << painted << "\n";
        
        if (painted == 1) c1++;
        else if (painted == 2) c2++;
        else if (painted == 3) c3++;
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "одна грань: " << c1 << " раз, вероятность " << (double)c1/trials << "\n";
    cout << "две грани:  " << c2 << " раз, вероятность " << (double)c2/trials << "\n";
    cout << "три грани:  " << c3 << " раз, вероятность " << (double)c3/trials << "\n";
    cout << "теоретически: 0.384, 0.096, 0.008\n";
}

void task8(int trials) {
    cout << "\n=== Задача 8: 6 кубиков с номерами ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> v = {1,2,3,4,5,6};
        shuffle(v.begin(), v.end(), gen);
        
        cout << "Испытание " << t+1 << ": последовательность ";
        for (int i = 0; i < 6; ++i) cout << v[i] << " ";
        
        bool ok = true;
        for (int i = 0; i < 6; ++i) if (v[i] != i+1) { ok = false; break; }
        if (ok) {
            cout << "✓ ПОРЯДОК 1 2 3 4 5 6\n";
            succ++;
        } else {
            cout << "✗ не тот порядок\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "совпадений: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 1/720 ≈ 0.001389\n";
}

void task13(int trials) {
    cout << "\n=== Задача 13: 100 карточек, одна нужная ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> cards(100);
        for (int i = 0; i < 100; ++i) cards[i] = i;
        shuffle(cards.begin(), cards.end(), gen);
        
        cout << "Испытание " << t+1 << ": выбраны карточки ";
        bool found = false;
        for (int i = 0; i < 10; ++i) {
            cout << cards[i] << " ";
            if (cards[i] == 0) found = true;
        }
        if (found) {
            cout << "✓ (нужная есть)\n";
            succ++;
        } else {
            cout << "✗ (нужной нет)\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "нужная попалась: " << succ << " раз, вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 0.1\n";
}

void task15(int trials) {
    cout << "\n=== Задача 15: 5 элементов, 2 изношены ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> parts = {0,0,1,1,1};
        shuffle(parts.begin(), parts.end(), gen);
        
        cout << "Испытание " << t+1 << ": выбраны элементы " << parts[0] << " и " << parts[1];
        if (parts[0] == 1 && parts[1] == 1) {
            cout << " ✓ (оба неизношены)\n";
            succ++;
        } else {
            cout << " ✗ (есть изношенный)\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "оба неизношены: " << succ << " раз, вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 0.3\n";
}

void task19(int trials) {
    cout << "\n=== Задача 19: 15 кинескопов, 10 львовских ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> kines = vector<int>(10, 1);
        kines.insert(kines.end(), 5, 0);
        shuffle(kines.begin(), kines.end(), gen);
        
        int lviv = 0;
        cout << "Испытание " << t+1 << ": выбраны ";
        for (int i = 0; i < 5; ++i) {
            cout << kines[i] << " ";
            if (kines[i] == 1) lviv++;
        }
        if (lviv == 3) {
            cout << "✓ (ровно 3 львовских)\n";
            succ++;
        } else {
            cout << "✗ (львовских " << lviv << ")\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "ровно 3 львовских: " << succ << " раз, вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 400/1001 ≈ 0.3996\n";
}

void task20(int trials) {
    cout << "\n=== Задача 20: 12 студентов, 8 отличников ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> students = vector<int>(8, 1);
        students.insert(students.end(), 4, 0);
        shuffle(students.begin(), students.end(), gen);
        
        int excell = 0;
        cout << "Испытание " << t+1 << ": выбраны ";
        for (int i = 0; i < 9; ++i) {
            cout << students[i] << " ";
            if (students[i] == 1) excell++;
        }
        if (excell == 5) {
            cout << "✓ (ровно 5 отличников)\n";
            succ++;
        } else {
            cout << "✗ (отличников " << excell << ")\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "ровно 5 отличников: " << succ << " раз, вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 14/55 ≈ 0.2545\n";
}

void task21(int trials) {
    cout << "\n=== Задача 21: 5 изделий, 3 окрашены ===\n";
    int one = 0, two = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> items = {1,1,1,0,0};
        shuffle(items.begin(), items.end(), gen);
        
        int colored = items[0] + items[1];
        cout << "Испытание " << t+1 << ": выбраны " << items[0] << " и " << items[1]
             << " (окрашенных " << colored << ")";
        if (colored == 1) {
            cout << " — одно окрашенное\n";
            one++;
        } else if (colored == 2) {
            cout << " — два окрашенных\n";
            two++;
        } else {
            cout << " — ни одного\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "одно окрашенное: " << one << " раз, вероятность " << (double)one/trials << "\n";
    cout << "два окрашенных:  " << two << " раз, вероятность " << (double)two/trials << "\n";
    cout << "хотя бы одно:     " << (one+two) << " раз, вероятность " << (double)(one+two)/trials << "\n";
    cout << "теоретически: 0.6, 0.3, 0.9\n";
}

void task22(int trials) {
    cout << "\n=== Задача 22: 4 диска по 5 цифр ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        cout << "Испытание " << t+1 << ": код ";
        int guess = 0;
        for (int disk = 0; disk < 4; ++disk) {
            int digit = random_int(0, 4);
            cout << digit;
            guess = guess * 10 + digit;
        }
        if (guess == 1234) {
            cout << " ✓ (угадано!)\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "угадано: " << succ << " раз, вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 1/625 = 0.0016\n";
}

void task26(int trials) {
    cout << "\n=== Задача 26: отрезок 20 см ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        double point = random_double(0, 20);
        cout << "Испытание " << t+1 << ": точка " << fixed << setprecision(2) << point << " см";
        if (point <= 10) {
            cout << " ✓ (попала в меньший отрезок)\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "попаданий: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 0.5\n";
}

void task27(int trials) {
    cout << "\n=== Задача 27: отрезок [0,1] ===\n";
    int succ = 0;
    double L = 1.0;
    for (int t = 0; t < trials; ++t) {
        double x = random_double(0, L);
        double m = min(x, L - x);
        cout << "Испытание " << t+1 << ": x = " << fixed << setprecision(3) << x
             << ", min = " << m;
        if (m > L/3) {
            cout << " ✓\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "успехов: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 1/3 ≈ 0.3333\n";
}

void task28(int trials) {
    cout << "\n=== Задача 28: круг R=10, малый круг r=3 ===\n";
    int succ = 0;
    double R = 10.0, r = 3.0;
    for (int t = 0; t < trials; ++t) {
        double theta = random_double(0, 2 * M_PI);
        double rad = random_double(0, R);
        double x = rad * cos(theta);
        double y = rad * sin(theta);
        double dist = sqrt(x*x + y*y);
        cout << "Испытание " << t+1 << ": точка на расстоянии " << fixed << setprecision(2) << dist;
        if (dist <= r) {
            cout << " ✓ (попала в малый круг)\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "попаданий: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: " << (r*r)/(R*R) << "\n";
}

void task29(int trials) {
    cout << "\n=== Задача 29: прямые через 2a, монета r<a ===\n";
    int succ = 0;
    double a = 1.0, r = 0.4;
    for (int t = 0; t < trials; ++t) {
        double x = random_double(0, 2 * a);
        cout << "Испытание " << t+1 << ": центр на x = " << fixed << setprecision(3) << x;
        if (x > r && x < 2 * a - r) {
            cout << " ✓ (не пересекает)\n";
            succ++;
        } else {
            cout << " ✗ (пересекает)\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "не пересекает: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: " << (a - r)/a << "\n";
}

void task30(int trials) {
    cout << "\n=== Задача 30: сетка квадратов a, монета r<a/2 ===\n";
    int succ = 0;
    double a = 1.0, r = 0.4;
    for (int t = 0; t < trials; ++t) {
        double x = random_double(0, a);
        double y = random_double(0, a);
        cout << "Испытание " << t+1 << ": центр (" << fixed << setprecision(3) << x << ", " << y << ")";
        if (x > r && x < a - r && y > r && y < a - r) {
            cout << " ✓ (не пересекает)\n";
            succ++;
        } else {
            cout << " ✗ (пересекает)\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "не пересекает: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: " << pow(1 - 2 * r / a, 2) << "\n";
}

void task32(int trials) {
    cout << "\n=== Задача 32: кольцо R=10, r=5 ===\n";
    int succ = 0;
    double Rbig = 10.0, Rsmall = 5.0;
    for (int t = 0; t < trials; ++t) {
        double theta = random_double(0, 2 * M_PI);
        double rad = random_double(0, Rbig);
        double x = rad * cos(theta);
        double y = rad * sin(theta);
        double dist = sqrt(x*x + y*y);
        cout << "Испытание " << t+1 << ": точка на расстоянии " << fixed << setprecision(2) << dist;
        if (dist > Rsmall && dist < Rbig) {
            cout << " ✓ (попала в кольцо)\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "попаданий в кольцо: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: 0.75\n";
}

void task45(int trials) {
    cout << "\n=== Задача 45: x,y∈[0,1], x+y≤1, xy≥0.09 ===\n";
    int succ = 0;
    for (int t = 0; t < trials; ++t) {
        double x = random_double(0, 1);
        double y = random_double(0, 1);
        cout << "Испытание " << t+1 << ": x = " << fixed << setprecision(3) << x
             << ", y = " << y << ", x+y=" << x+y << ", xy=" << x*y;
        if (x + y <= 1 && x * y >= 0.09) {
            cout << " ✓\n";
            succ++;
        } else {
            cout << " ✗\n";
        }
    }
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "успехов: " << succ << ", вероятность " << (double)succ/trials << "\n";
    cout << "теоретически: " << 0.4 - 0.09 * log(9) << "\n";
}

int main() {
    int choice, trials;
    cout << "Сколько испытаний провести? ";
    cin >> trials;
    cout << "choice task (6,8,13,15,19,20,21,22,26,27,28,29,30,32,45): ";
    cin >> choice;
    
    switch (choice) {
        case 6: task6(trials); break;
        case 8: task8(trials); break;
        case 13: task13(trials); break;
        case 15: task15(trials); break;
        case 19: task19(trials); break;
        case 20: task20(trials); break;
        case 21: task21(trials); break;
        case 22: task22(trials); break;
        case 26: task26(trials); break;
        case 27: task27(trials); break;
        case 28: task28(trials); break;
        case 29: task29(trials); break;
        case 30: task30(trials); break;
        case 32: task32(trials); break;
        case 45: task45(trials); break;
        default: cout << "wrong task number\n";
    }
    return 0;
}