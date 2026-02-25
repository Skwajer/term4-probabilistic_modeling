#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>
#include <map>

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

// Задача 57: Вероятности детали в ящиках 0.6, 0.7, 0.8, 0.9
void task57(int trials) {
    cout << "\n=== Задача 57: Деталь в ящиках (p1=0.6, p2=0.7, p3=0.8, p4=0.9) ===\n";
    vector<double> probs = {0.6, 0.7, 0.8, 0.9};
    int count_not_more_than_3 = 0; // не более чем в трех (0,1,2,3)
    int count_not_less_than_2 = 0; // не менее чем в двух (2,3,4)
    
    for (int t = 0; t < trials; ++t) {
        int boxes_with_detail = 0;
        cout << "Испытание " << t+1 << ": ящики [";
        for (int i = 0; i < 4; ++i) {
            double r = random_double(0, 1);
            bool has_detail = (r < probs[i]);
            cout << (has_detail ? "1" : "0") << (i < 3 ? "," : "]");
            if (has_detail) boxes_with_detail++;
        }
        cout << " -> найдено в " << boxes_with_detail << " ящиках";
        
        if (boxes_with_detail <= 3) {
            cout << " (не более чем в 3) ✓";
            count_not_more_than_3++;
        } else {
            cout << " (не более чем в 3) ✗";
        }
        
        if (boxes_with_detail >= 2) {
            cout << " (не менее чем в 2) ✓\n";
            count_not_less_than_2++;
        } else {
            cout << " (не менее чем в 2) ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) не более чем в трех ящиках: " << count_not_more_than_3 
         << " раз, вероятность " << fixed << setprecision(4) << (double)count_not_more_than_3/trials << "\n";
    cout << "б) не менее чем в двух ящиках: " << count_not_less_than_2 
         << " раз, вероятность " << (double)count_not_less_than_2/trials << "\n";
}

// Задача 58: Три игральные кости
void task58(int trials) {
    cout << "\n=== Задача 58: Три игральные кости ===\n";
    int count_all_fives = 0;      // на каждой пятерка
    int count_all_equal = 0;      // на всех одинаковое число
    
    for (int t = 0; t < trials; ++t) {
        int d1 = random_int(1, 6);
        int d2 = random_int(1, 6);
        int d3 = random_int(1, 6);
        
        cout << "Испытание " << t+1 << ": кости [" << d1 << "," << d2 << "," << d3 << "]";
        
        if (d1 == 5 && d2 == 5 && d3 == 5) {
            cout << " — ВСЕ ПЯТЕРКИ ✓";
            count_all_fives++;
        }
        
        if (d1 == d2 && d2 == d3) {
            cout << " — ВСЕ ОДИНАКОВЫЕ ✓";
            count_all_equal++;
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) на каждой грани пять очков: " << count_all_fives 
         << " раз, вероятность " << (double)count_all_fives/trials << "\n";
    cout << "б) на всех гранях одинаковое число: " << count_all_equal 
         << " раз, вероятность " << (double)count_all_equal/trials << "\n";
    cout << "Теоретически: а) 1/216 ≈ 0.00463, б) 6/216 = 1/36 ≈ 0.02778\n";
}

// Задача 59: Три игральные кости (разные условия)
void task59(int trials) {
    cout << "\n=== Задача 59: Три игральные кости (особые случаи) ===\n";
    int count_two_ones_other_diff = 0;  // на двух - 1, на третьей - другое
    int count_two_equal_other_diff = 0; // на двух одинаковые, на третьей - другое
    int count_all_diff = 0;             // все разные
    
    for (int t = 0; t < trials; ++t) {
        int d1 = random_int(1, 6);
        int d2 = random_int(1, 6);
        int d3 = random_int(1, 6);
        
        cout << "Испытание " << t+1 << ": кости [" << d1 << "," << d2 << "," << d3 << "]";
        
        // a) на двух гранях по 1, на третьей другое
        int ones_count = (d1 == 1 ? 1 : 0) + (d2 == 1 ? 1 : 0) + (d3 == 1 ? 1 : 0);
        if (ones_count == 2) {
            int other = (d1 != 1 ? d1 : (d2 != 1 ? d2 : d3));
            if (other != 1) {
                cout << " — две единицы и " << other << " ✓а";
                count_two_ones_other_diff++;
            }
        }
        
        // б) на двух одинаковые, на третьей другое
        if ((d1 == d2 && d2 != d3) || (d1 == d3 && d1 != d2) || (d2 == d3 && d2 != d1)) {
            cout << " — ровно две одинаковые ✓б";
            count_two_equal_other_diff++;
        }
        
        // в) все разные
        if (d1 != d2 && d1 != d3 && d2 != d3) {
            cout << " — все разные ✓в";
            count_all_diff++;
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) две единицы и другое: " << count_two_ones_other_diff 
         << " раз, вероятность " << (double)count_two_ones_other_diff/trials << "\n";
    cout << "б) ровно две одинаковые: " << count_two_equal_other_diff 
         << " раз, вероятность " << (double)count_two_equal_other_diff/trials << "\n";
    cout << "в) все разные: " << count_all_diff 
         << " раз, вероятность " << (double)count_all_diff/trials << "\n";
}

// Задача 61: Вероятность попадания 0.8, ни одного промаха
void task61(int trials) {
    cout << "\n=== Задача 61: Стрелок, p=0.8, ни одного промаха ===\n";
    double p = 0.8;
    double target_prob = 0.4;
    
    cout << "Ищем максимальное n, при котором P(все попадания) < " << target_prob << "\n";
    cout << "P(все попадания за n выстрелов) = " << p << "^n\n\n";
    
    int max_n = 0;
    for (int n = 1; n <= 10; ++n) {
        int success_count = 0;
        for (int t = 0; t < trials; ++t) {
            bool all_hits = true;
            for (int shot = 0; shot < n; ++shot) {
                if (random_double(0, 1) >= p) {
                    all_hits = false;
                    break;
                }
            }
            if (all_hits) success_count++;
        }
        double empirical_prob = (double)success_count / trials;
        double theoretical_prob = pow(p, n);
        
        cout << "n = " << n << ": попаданий без промаха " << success_count 
             << "/" << trials << " = " << fixed << setprecision(4) << empirical_prob
             << " (теор: " << theoretical_prob << ")";
        
        if (theoretical_prob < target_prob) {
            cout << " < " << target_prob << " ✓\n";
            if (max_n == 0) max_n = n;
        } else {
            cout << " >= " << target_prob << " ✗\n";
        }
    }
    
    cout << "\nОтвет: n = " << max_n << " (при n=" << max_n 
         << " вероятность " << pow(p, max_n) << " < " << target_prob << ")\n";
}

// Задача 62: Круг R, вписан треугольник, 4 точки
void task62(int trials) {
    cout << "\n=== Задача 62: Круг радиуса R, вписан правильный треугольник ===\n";
    double R = 1.0; // для удобства возьмем R=1
    // Площадь круга: πR² = π
    // Площадь правильного треугольника, вписанного в круг R: (3√3/4) * (√3R)²? 
    // Сторона треугольника a = √3 * R, площадь треугольника = (√3/4) * a² = (√3/4) * 3R² = (3√3/4)R²
    double area_circle = M_PI * R * R;
    double area_triangle = (3 * sqrt(3) / 4) * R * R;
    double area_segment = (area_circle - area_triangle) / 3; // три малых сегмента
    
    double p_triangle = area_triangle / area_circle;
    double p_segment = area_segment / area_circle;
    
    cout << "Площадь круга: " << area_circle << "\n";
    cout << "Площадь треугольника: " << area_triangle << "\n";
    cout << "Площадь одного сегмента: " << area_segment << "\n";
    cout << "p(треугольник) = " << p_triangle << "\n";
    cout << "p(сегмент) = " << p_segment << "\n\n";
    
    int count_all_in_triangle = 0;
    int count_one_in_triangle_one_per_segment = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<int> points(4, 0); // 0 - вне, 1 - треугольник, 2,3,4 - сегменты
        bool valid = true;
        
        cout << "Испытание " << t+1 << ": точки [";
        
        for (int i = 0; i < 4; ++i) {
            // Генерируем точку в круге методом rejection sampling
            double x, y;
            do {
                x = random_double(-R, R);
                y = random_double(-R, R);
            } while (x*x + y*y > R*R);
            
            // Проверяем, попадает ли точка в треугольник
            // Для правильного треугольника с вершинами: 
            // (0,R), (-√3/2*R, -R/2), (√3/2*R, -R/2)
            // Используем барицентрические координаты или проверку полуплоскостей
            double x1 = 0, y1 = R;
            double x2 = -sqrt(3)/2 * R, y2 = -R/2;
            double x3 = sqrt(3)/2 * R, y3 = -R/2;
            
            // Функция знака относительно прямой
            auto sign = [](double x1, double y1, double x2, double y2, double x3, double y3) {
                return (x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3);
            };
            
            double d1 = sign(x, y, x1, y1, x2, y2);
            double d2 = sign(x, y, x2, y2, x3, y3);
            double d3 = sign(x, y, x3, y3, x1, y1);
            
            bool has_neg = (d1 < 0) || (d2 < 0) || (d3 < 0);
            bool has_pos = (d1 > 0) || (d2 > 0) || (d3 > 0);
            
            bool in_triangle = !(has_neg && has_pos);
            
            if (in_triangle) {
                points[i] = 1; // треугольник
                cout << "T";
            } else {
                // Определяем сегмент по углу
                double angle = atan2(y, x);
                // Сегменты: примерно 120 градусов каждый
                int segment;
                if (angle > -M_PI/6 && angle <= M_PI/2) segment = 2; // правый нижний
                else if (angle > M_PI/2 && angle <= 7*M_PI/6) segment = 3; // левый нижний
                else segment = 4; // верхний
                
                points[i] = segment;
                cout << "S" << segment-1;
            }
            if (i < 3) cout << ",";
        }
        cout << "]";
        
        // а) все четыре точки внутри треугольника
        bool all_in_triangle = true;
        for (int p : points) {
            if (p != 1) all_in_triangle = false;
        }
        if (all_in_triangle) {
            cout << " — все в треугольнике ✓а";
            count_all_in_triangle++;
        }
        
        // б) одна точка в треугольнике и по одной в каждом сегменте
        map<int, int> count;
        for (int p : points) count[p]++;
        if (count[1] == 1 && count[2] == 1 && count[3] == 1 && count[4] == 1) {
            cout << " — по одной в каждой области ✓б";
            count_one_in_triangle_one_per_segment++;
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) все четыре точки внутри треугольника: " << count_all_in_triangle 
         << " раз, вероятность " << (double)count_all_in_triangle/trials << "\n";
    cout << "б) одна в треугольнике, по одной в сегментах: " << count_one_in_triangle_one_per_segment 
         << " раз, вероятность " << (double)count_one_in_triangle_one_per_segment/trials << "\n";
    cout << "Теоретически: а) p^4, где p = площадь треугольника/площадь круга\n";
    cout << "б) 4! * (p_треугольник) * (p_сегмент)^3\n";
}

// Задача 63: Отрезок на 3 части, 3 точки
void task63(int trials) {
    cout << "\n=== Задача 63: Отрезок разделен на 3 равные части, 3 точки ===\n";
    int count_one_per_part = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<double> points(3);
        vector<int> part(3, 0);
        
        cout << "Испытание " << t+1 << ": точки [";
        for (int i = 0; i < 3; ++i) {
            points[i] = random_double(0, 1);
            part[i] = floor(points[i] * 3); // 0, 1, 2
            cout << fixed << setprecision(3) << points[i] << "(" << part[i]+1 << ")" 
                 << (i < 2 ? ", " : "");
        }
        cout << "]";
        
        // Проверяем, что все части содержат по одной точке
        sort(part.begin(), part.end());
        if (part[0] == 0 && part[1] == 1 && part[2] == 2) {
            cout << " — по одной в каждой части ✓\n";
            count_one_per_part++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "по одной точке на каждую часть: " << count_one_per_part 
         << " раз, вероятность " << (double)count_one_per_part/trials << "\n";
    cout << "Теоретически: 3!/3^3 = 6/27 = 2/9 ≈ 0.2222\n";
}

// Задача 65: 100 билетов, 5 выигрышных, выбрать 2
void task65(int trials) {
    cout << "\n=== Задача 65: 100 лотерейных билетов, 5 выигрышных ===\n";
    int count_both_win = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<int> tickets(100, 0);
        for (int i = 0; i < 5; ++i) tickets[i] = 1; // первые 5 - выигрышные
        shuffle(tickets.begin(), tickets.end(), gen);
        
        int ticket1 = random_int(0, 99);
        int ticket2;
        do {
            ticket2 = random_int(0, 99);
        } while (ticket2 == ticket1);
        
        cout << "Испытание " << t+1 << ": билеты " << ticket1 << "(" << tickets[ticket1] 
             << ") и " << ticket2 << "(" << tickets[ticket2] << ")";
        
        if (tickets[ticket1] == 1 && tickets[ticket2] == 1) {
            cout << " — оба выигрышные ✓\n";
            count_both_win++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "оба выигрышные: " << count_both_win 
         << " раз, вероятность " << (double)count_both_win/trials << "\n";
    cout << "Теоретически: C(5,2)/C(100,2) = 10/4950 ≈ 0.00202\n";
}

// Задача 67: 10 деталей, 6 окрашенных, выбрать 4
void task67(int trials) {
    cout << "\n=== Задача 67: 10 деталей, 6 окрашенных, выбрать 4 ===\n";
    int count_all_colored = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<int> details(10, 0);
        for (int i = 0; i < 6; ++i) details[i] = 1; // окрашенные
        shuffle(details.begin(), details.end(), gen);
        
        int colored_count = 0;
        cout << "Испытание " << t+1 << ": выбраны [";
        for (int i = 0; i < 4; ++i) {
            int index = random_int(0, 9 - i);
            int detail = details[index];
            cout << detail << (i < 3 ? "," : "");
            if (detail == 1) colored_count++;
            details.erase(details.begin() + index);
        }
        cout << "] -> окрашенных " << colored_count;
        
        if (colored_count == 4) {
            cout << " — ВСЕ ОКРАШЕННЫЕ ✓\n";
            count_all_colored++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "все четыре окрашенные: " << count_all_colored 
         << " раз, вероятность " << (double)count_all_colored/trials << "\n";
    cout << "Теоретически: C(6,4)/C(10,4) = 15/210 = 1/14 ≈ 0.07143\n";
}

// Задача 68: 5 шаров с номерами 1-5, выбрать 3 без возвращения
void task68(int trials) {
    cout << "\n=== Задача 68: 5 шаров с номерами 1-5, выбрать 3 ===\n";
    int count_sequence_145 = 0;
    int count_set_145 = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<int> balls = {1, 2, 3, 4, 5};
        shuffle(balls.begin(), balls.end(), gen);
        
        vector<int> chosen;
        cout << "Испытание " << t+1 << ": выбраны [";
        for (int i = 0; i < 3; ++i) {
            chosen.push_back(balls[i]);
            cout << balls[i] << (i < 2 ? "," : "");
        }
        cout << "]";
        
        // а) последовательно 1,4,5
        if (chosen[0] == 1 && chosen[1] == 4 && chosen[2] == 5) {
            cout << " — последовательность 1,4,5 ✓а";
            count_sequence_145++;
        }
        
        // б) номера 1,4,5 в любом порядке
        sort(chosen.begin(), chosen.end());
        if (chosen[0] == 1 && chosen[1] == 4 && chosen[2] == 5) {
            cout << " — набор {1,4,5} ✓б";
            count_set_145++;
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) последовательно 1,4,5: " << count_sequence_145 
         << " раз, вероятность " << (double)count_sequence_145/trials << "\n";
    cout << "б) номера 1,4,5 в любом порядке: " << count_set_145 
         << " раз, вероятность " << (double)count_set_145/trials << "\n";
    cout << "Теоретически: а) 1/P(5,3) = 1/60 ≈ 0.01667, б) 1/C(5,3) = 1/10 = 0.1\n";
}

// Задача 69: Студент знает 20 из 25 вопросов
void task69(int trials) {
    cout << "\n=== Задача 69: Студент знает 20 из 25 вопросов ===\n";
    int count_knows_all = 0;
    
    for (int t = 0; t < trials; ++t) {
        vector<int> questions(25, 0);
        for (int i = 0; i < 20; ++i) questions[i] = 1; // знает
        shuffle(questions.begin(), questions.end(), gen);
        
        int known_count = 0;
        cout << "Испытание " << t+1 << ": вопросы [";
        for (int i = 0; i < 3; ++i) {
            int q = questions[i];
            cout << (q ? "З" : "Н") << (i < 2 ? "," : "");
            if (q == 1) known_count++;
        }
        cout << "] -> знает " << known_count;
        
        if (known_count == 3) {
            cout << " — ВСЕ ЗНАЕТ ✓\n";
            count_knows_all++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "знает все три вопроса: " << count_knows_all 
         << " раз, вероятность " << (double)count_knows_all/trials << "\n";
    cout << "Теоретически: C(20,3)/C(25,3) = 1140/2300 = 0.49565\n";
}

// Задача 70: Кубики с номерами 1-10, выбрать 3
void task70(int trials) {
    cout << "\n=== Задача 70: Кубики с номерами 1-10, выбрать 3 ===\n";
    int count_seq_without_return = 0;
    int count_seq_with_return = 0;
    
    // а) без возвращения
    for (int t = 0; t < trials; ++t) {
        vector<int> cubes = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        shuffle(cubes.begin(), cubes.end(), gen);
        
        cout << "Испытание " << t+1 << " (без возвр): [";
        bool is_123 = true;
        for (int i = 0; i < 3; ++i) {
            cout << cubes[i] << (i < 2 ? "," : "");
            if (cubes[i] != i+1) is_123 = false;
        }
        cout << "]";
        
        if (is_123) {
            cout << " — 1,2,3 ✓\n";
            count_seq_without_return++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\n";
    
    // б) с возвращением
    for (int t = 0; t < trials; ++t) {
        cout << "Испытание " << t+1 << " (с возвр): [";
        bool is_123 = true;
        for (int i = 0; i < 3; ++i) {
            int cube = random_int(1, 10);
            cout << cube << (i < 2 ? "," : "");
            if (cube != i+1) is_123 = false;
        }
        cout << "]";
        
        if (is_123) {
            cout << " — 1,2,3 ✓\n";
            count_seq_with_return++;
        } else {
            cout << " — ✗\n";
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "а) без возвращения (1,2,3): " << count_seq_without_return 
         << " раз, вероятность " << (double)count_seq_without_return/trials << "\n";
    cout << "б) с возвращением (1,2,3): " << count_seq_with_return 
         << " раз, вероятность " << (double)count_seq_with_return/trials << "\n";
    cout << "Теоретически: а) 1/(10*9*8) = 1/720 ≈ 0.001389, б) 1/10^3 = 1/1000 = 0.001\n";
}

int main() {
    int choice, trials;
    cout << "Сколько испытаний провести? ";
    cin >> trials;
    cout << "Выберите задачу (57,58,59,61,62,63,65,67,68,69,70): ";
    cin >> choice;
    
    switch (choice) {
        case 57: task57(trials); break;
        case 58: task58(trials); break;
        case 59: task59(trials); break;
        case 61: task61(trials); break;
        case 62: task62(trials); break;
        case 63: task63(trials); break;
        case 65: task65(trials); break;
        case 67: task67(trials); break;
        case 68: task68(trials); break;
        case 69: task69(trials); break;
        case 70: task70(trials); break;
        default: cout << "Неверный номер задачи\n";
    }
    return 0;
}