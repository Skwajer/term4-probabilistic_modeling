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

// 109. Две лампы отказали из четырех
void task109(int trials) {
    cout << "\n=== Задача 109: Две лампы отказали, найти P(отказали 1 и 2) ===\n";
    double p1 = 0.1, p2 = 0.2, p3 = 0.3, p4 = 0.4;
    int count_both_failed = 0;
    int count_two_failed = 0;
    
    for (int t = 0; t < trials; ++t) {
        bool fail1 = random_double(0, 1) < p1;
        bool fail2 = random_double(0, 1) < p2;
        bool fail3 = random_double(0, 1) < p3;
        bool fail4 = random_double(0, 1) < p4;
        
        int failed = fail1 + fail2 + fail3 + fail4;
        if (failed == 2) {
            count_two_failed++;
            if (fail1 && fail2) count_both_failed++;
        }
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "ровно две лампы отказали: " << count_two_failed << " раз\n";
    if (count_two_failed > 0) {
        double prob = (double)count_both_failed / count_two_failed;
        cout << "условная вероятность (отказали 1 и 2 | отказали ровно две): " << prob << "\n";
    } else {
        cout << "событие 'ровно две отказали' не произошло ни разу\n";
    }
    cout << "теоретически: ≈ 0.0392\n";
}

// 111. Два равносильных противника
void task111(int trials) {
    cout << "\n=== Задача 111: Шахматы (равносильные противники) ===\n";
    double p = 0.5;
    
    int win1_of_2 = 0, win2_of_4 = 0;
    int win_ge2_of_4 = 0, win_ge3_of_5 = 0;
    
    for (int t = 0; t < trials; ++t) {
        int wins2 = 0;
        for (int i = 0; i < 2; ++i) if (random_double(0,1) < p) wins2++;
        if (wins2 == 1) win1_of_2++;
        
        int wins4 = 0;
        for (int i = 0; i < 4; ++i) if (random_double(0,1) < p) wins4++;
        if (wins4 == 2) win2_of_4++;
        
        if (wins4 >= 2) win_ge2_of_4++;
        
        int wins5 = 0;
        for (int i = 0; i < 5; ++i) if (random_double(0,1) < p) wins5++;
        if (wins5 >= 3) win_ge3_of_5++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "а) выиграть ровно 1 из 2: " << (double)win1_of_2 / trials << " (теор. 0.5)\n";
    cout << "   выиграть ровно 2 из 4: " << (double)win2_of_4 / trials << " (теор. 0.375)\n";
    cout << "б) не менее 2 из 4: " << (double)win_ge2_of_4 / trials << " (теор. 0.6875)\n";
    cout << "   не менее 3 из 5: " << (double)win_ge3_of_5 / trials << " (теор. 0.5)\n";
}

// 112. Монету бросают 5 раз
void task112(int trials) {
    cout << "\n=== Задача 112: Монета 5 раз ===\n";
    double p = 0.5;
    int less2 = 0, ge2 = 0;
    
    for (int t = 0; t < trials; ++t) {
        int heads = 0;
        for (int i = 0; i < 5; ++i) if (random_double(0,1) < p) heads++;
        if (heads < 2) less2++;
        if (heads >= 2) ge2++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "а) менее двух раз: " << (double)less2 / trials << " (теор. 0.1875)\n";
    cout << "б) не менее двух раз: " << (double)ge2 / trials << " (теор. 0.8125)\n";
}

// 113. Событие A и B
void task113(int trials) {
    cout << "\n=== Задача 113: Событие A и B ===\n";
    
    int ge3 = 0;
    for (int t = 0; t < trials; ++t) {
        int count = 0;
        for (int i = 0; i < 4; ++i) if (random_double(0,1) < 0.4) count++;
        if (count >= 3) ge3++;
    }
    cout << "а) не менее трех раз из четырех (p=0.4): " << (double)ge3 / trials << " (теор. 0.1792)\n";
    
    int ge4 = 0;
    for (int t = 0; t < trials; ++t) {
        int count = 0;
        for (int i = 0; i < 5; ++i) if (random_double(0,1) < 0.8) count++;
        if (count >= 4) ge4++;
    }
    cout << "б) не менее четырех раз из пяти (p=0.8): " << (double)ge4 / trials << " (теор. 0.73728)\n";
}

// 114. Устройство с резервными элементами
void task114(int trials) {
    cout << "\n=== Задача 114: Устройство с резервом ===\n";
    double p = 0.9;
    
    int work3 = 0, work4 = 0, work5 = 0;
    
    for (int t = 0; t < trials; ++t) {
        int working3 = 0;
        for (int i = 0; i < 3; ++i) if (random_double(0,1) < p) working3++;
        if (working3 == 3) work3++;
        
        int working4 = 0;
        for (int i = 0; i < 4; ++i) if (random_double(0,1) < p) working4++;
        if (working4 >= 3) work4++;
        
        int working5 = 0;
        for (int i = 0; i < 5; ++i) if (random_double(0,1) < p) working5++;
        if (working5 >= 3) work5++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "а) 3 основных (все работают): " << (double)work3 / trials << " (теор. 0.729)\n";
    cout << "б) 4 элемента (≥3 работают): " << (double)work4 / trials << " (теор. 0.9477)\n";
    cout << "в) 5 элементов (≥3 работают): " << (double)work5 / trials << " (теор. 0.99144)\n";
}

// 115. Семья с пятью детьми
void task115(int trials) {
    cout << "\n=== Задача 115: Пять детей, вероятность мальчика 0.51 ===\n";
    double p = 0.51;
    
    int boys2 = 0, boys_le2 = 0, boys_gt2 = 0, boys_2to3 = 0;
    
    for (int t = 0; t < trials; ++t) {
        int boys = 0;
        for (int i = 0; i < 5; ++i) if (random_double(0,1) < p) boys++;
        
        if (boys == 2) boys2++;
        if (boys <= 2) boys_le2++;
        if (boys > 2) boys_gt2++;
        if (boys >= 2 && boys <= 3) boys_2to3++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "а) два мальчика: " << (double)boys2 / trials << " (теор. 0.306)\n";
    cout << "б) не более двух мальчиков: " << (double)boys_le2 / trials << " (теор. 0.481)\n";
    cout << "в) более двух мальчиков: " << (double)boys_gt2 / trials << " (теор. 0.519)\n";
    cout << "г) от двух до трех мальчиков: " << (double)boys_2to3 / trials << " (теор. 0.624)\n";
}

// 116. Отрезок AB с точкой C в отношении 2:1
void task116(int trials) {
    cout << "\n=== Задача 116: Отрезок AB, точка C в отношении 2:1 ===\n";
    double p_left = 2.0 / 3.0;
    
    int count = 0;
    for (int t = 0; t < trials; ++t) {
        int left = 0;
        for (int i = 0; i < 4; ++i) if (random_double(0,1) < p_left) left++;
        if (left == 2) count++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "ровно две точки левее C: " << (double)count / trials << " (теор. 8/27 ≈ 0.2963)\n";
}

// 117. 100 лампочек из 1000 (равновозможное число испорченных от 0 до 5)
void task117(int trials) {
    cout << "\n=== Задача 117: 100 лампочек из 1000 ===\n";
    int count = 0;
    
    for (int t = 0; t < trials; ++t) {
        int defective_total = random_int(0, 5);
        int good_total = 1000 - defective_total;

        
        bool all_good = true;
        if (defective_total > 0) {
            vector<int> batch(1000);
            for (int i = 0; i < 1000; ++i) batch[i] = (i < defective_total) ? 0 : 1;
            shuffle(batch.begin(), batch.end(), gen);
            for (int i = 0; i < 100; ++i) {
                if (batch[i] == 0) {
                    all_good = false;
                    break;
                }
            }
        }
        if (all_good) count++;
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "все 100 исправны: " << (double)count / trials << "\n";
    cout << "теоретически: (1/6)*∑_{k=0}^{5} C(1000-k,100)/C(1000,100) ≈ 0.95-0.96\n";
}

// 118. Отрезок на 4 равные части, 8 точек
void task118(int trials) {
    cout << "\n=== Задача 118: Отрезок на 4 части, 8 точек ===\n";
    double p = 0.25;
    
    int count = 0;
    for (int t = 0; t < trials; ++t) {
        vector<int> parts(4, 0);
        for (int i = 0; i < 8; ++i) {
            double r = random_double(0, 1);
            int part = (int)(r * 4);
            if (part == 4) part = 3;
            parts[part]++;
        }
        if (parts[0] == 2 && parts[1] == 2 && parts[2] == 2 && parts[3] == 2) {
            count++;
        }
    }
    
    cout << "Результаты после " << trials << " испытаний:\n";
    cout << "по две точки в каждой части: " << (double)count / trials << " (теор. 315/8192 ≈ 0.03845)\n";
}

int main() {
    int trials, task_num;
    cout << "Введите количество испытаний: ";
    cin >> trials;
    cout << "Введите номер задачи (109,111,112,113,114,115,116,117,118): ";
    cin >> task_num;
    
    switch (task_num) {
        case 109: task109(trials); break;
        case 111: task111(trials); break;
        case 112: task112(trials); break;
        case 113: task113(trials); break;
        case 114: task114(trials); break;
        case 115: task115(trials); break;
        case 116: task116(trials); break;
        case 117: task117(trials); break;
        case 118: task118(trials); break;
        default: cout << "Неверный номер задачи\n";
    }
    
    return 0;
}