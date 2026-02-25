#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>
#include <map>
#include <cstdlib>

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

// Задача про ключи (из нового задания)
void task_keys(int n, int K) {
    cout << "\n========================================\n";
    cout << "ЗАДАЧА: Поиск нужного ключа в связке из " << n << " ключей\n";
    cout << "========================================\n\n";
    
    cout << "Теоретическая вероятность для каждого исхода (1.." << n << "): " 
         << "1/" << n << " = " << fixed << setprecision(4) << 1.0/n << "\n\n";
    
    // Вектор для подсчета количества экспериментов, закончившихся на каждом шаге
    vector<int> results(n, 0);
    
    cout << "Проводим " << K << " экспериментов:\n";
    cout << string(70, '-') << "\n";
    
    // Проведем серию экспериментов
    for (int exp = 0; exp < K; ++exp) {
        // Создаем вектор ключей: 0 - неправильные, 1 - правильный
        vector<int> keys(n, 0);
        int correct_pos = random_int(0, n-1);  // правильный ключ в случайной позиции
        keys[correct_pos] = 1;
        
        // Перемешиваем ключи (хотя они уже в случайном порядке, но для надежности)
        shuffle(keys.begin(), keys.end(), gen);
        
        // Ищем правильный ключ
        int attempt = 0;
        for (int i = 0; i < n; ++i) {
            attempt++;
            if (keys[i] == 1) {
                break;
            }
        }
        
        results[attempt - 1]++;  // attempt от 1 до n
        
        // Для первых 20 экспериментов показываем детали
        if (exp < 20) {
            cout << "Эксперимент " << setw(3) << exp+1 << ": ключи [";
            for (int i = 0; i < n; ++i) {
                cout << keys[i];
                if (i < n-1) cout << ",";
            }
            cout << "] -> правильный ключ найден на " << attempt << "-й попытке\n";
        }
        else if (exp == 20) {
            cout << "...\n";
        }
    }
    
    cout << string(70, '-') << "\n\n";
    cout << "РЕЗУЛЬТАТЫ:\n";
    cout << string(70, '-') << "\n";
    cout << "| Исход (попытка) | Частота | Эмпирическая вероятность | Теоретическая | Разница |\n";
    cout << string(70, '-') << "\n";
    
    double total_diff = 0;
    for (int i = 0; i < n; ++i) {
        double empirical = (double)results[i] / K;
        double theoretical = 1.0 / n;
        double diff = abs(empirical - theoretical);
        total_diff += diff;
        
        cout << "| " << setw(15) << i+1 << " | " 
             << setw(7) << results[i] << " | " 
             << setw(21) << fixed << setprecision(6) << empirical << " | "
             << setw(13) << theoretical << " | "
             << setw(8) << diff << " |\n";
    }
    cout << string(70, '-') << "\n";
    cout << "Среднее отклонение: " << fixed << setprecision(6) << total_diff / n << "\n\n";
}

// Задача 68 (новая): Информирование группы
void task68_new(int trials) {
    cout << "\n=== Задача 68: Информирование группы ===\n";
    cout << "Вероятность оповещения: староста - 0.3, студент - 0.5\n";
    cout << "Цель: вероятность > 0.9, что группа информирована\n\n";
    
    double p_head = 0.3;  // вероятность оповещения старостой
    double p_student = 0.5; // вероятность оповещения студентом
    
    cout << "Ищем минимальное количество студентов (кроме старосты), чтобы P > 0.9\n\n";
    
    int min_students = 0;
    cout << string(80, '-') << "\n";
    cout << "| Студентов | Успехов | Эмпирическая P | Теоретическая P | Статус |\n";
    cout << string(80, '-') << "\n";
    
    for (int n = 0; n <= 10; ++n) {
        int success_count = 0;
        
        for (int t = 0; t < trials; ++t) {
            bool informed = false;
            
            // Проверяем старосту
            if (random_double(0, 1) < p_head) {
                informed = true;
            } else {
                // Если староста не оповестил, проверяем студентов
                for (int i = 0; i < n; ++i) {
                    if (random_double(0, 1) < p_student) {
                        informed = true;
                        break;
                    }
                }
            }
            
            if (informed) success_count++;
        }
        
        double empirical_prob = (double)success_count / trials;
        // Теоретическая вероятность: 1 - (1-p_head)*(1-p_student)^n
        double theoretical_prob = 1.0 - (1.0 - p_head) * pow(1.0 - p_student, n);
        
        cout << "| " << setw(9) << n << " | " 
             << setw(7) << success_count << " | " 
             << setw(14) << fixed << setprecision(4) << empirical_prob << " | "
             << setw(15) << theoretical_prob << " | ";
        
        if (theoretical_prob > 0.9) {
            cout << "  >0.9 ✓";
            if (min_students == 0) min_students = n;
        } else {
            cout << " <=0.9 ✗";
        }
        cout << " |\n";
    }
    cout << string(80, '-') << "\n";
    
    cout << "\nРЕЗУЛЬТАТ: необходимо отправить письмо минимум " << min_students 
         << " студентам (кроме старосты)\n";
    
    // Дополнительный анализ
    cout << "\nДетальный анализ для n = " << min_students << ":\n";
    cout << "P(староста оповестит) = " << p_head << "\n";
    cout << "P(староста не оповестит) = " << (1-p_head) << "\n";
    cout << "P(хотя бы один студент оповестит при n=" << min_students << ") = " 
         << 1 - pow(1-p_student, min_students) << "\n";
    cout << "P(группа информирована) = 1 - " << (1-p_head) << " * " 
         << pow(1-p_student, min_students) << " = " 
         << 1 - (1-p_head)*pow(1-p_student, min_students) << "\n";
}

// Задача 69 (новая): Карты из колоды
void task69_new(int trials) {
    cout << "\n=== Задача 69: Карты из колоды ===\n";
    
    // Варианты колод
    vector<int> deck_sizes = {36, 52};
    
    for (int deck_size : deck_sizes) {
        cout << "\n" << string(60, '=') << "\n";
        cout << "КОЛОДА " << deck_size << " КАРТ\n";
        cout << string(60, '=') << "\n";
        
        int num_values = (deck_size == 36) ? 9 : 13;
        
        // Без возвращения
        cout << "\n--- БЕЗ ВОЗВРАЩЕНИЯ ---\n";
        int count_three_one = 0;      // три одного значения, одно другого
        int count_two_two = 0;         // два одного, два другого
        int count_all_diff = 0;         // все разного значения
        int count_all_same = 0;         // все одного значения
        
        for (int t = 0; t < trials; ++t) {
            // Создаем колоду
            vector<int> deck;
            for (int v = 0; v < num_values; ++v) {
                for (int s = 0; s < 4; ++s) {
                    deck.push_back(v);
                }
            }
            
            // Выбираем 4 карты без возвращения
            shuffle(deck.begin(), deck.end(), gen);
            vector<int> hand(deck.begin(), deck.begin() + 4);
            
            // Подсчитываем количество карт каждого значения
            map<int, int> value_count;
            for (int card : hand) {
                value_count[card]++;
            }
            
            // Анализируем комбинацию
            vector<int> counts;
            for (auto& p : value_count) {
                counts.push_back(p.second);
            }
            sort(counts.begin(), counts.end());
            
            if (counts.size() == 1 && counts[0] == 4) {
                count_all_same++;
            } else if (counts.size() == 2 && counts[0] == 1 && counts[1] == 3) {
                count_three_one++;
            } else if (counts.size() == 2 && counts[0] == 2 && counts[1] == 2) {
                count_two_two++;
            } else if (counts.size() == 4) {
                count_all_diff++;
            }
        }
        
        cout << "Результаты после " << trials << " испытаний:\n";
        cout << string(70, '-') << "\n";
        cout << "| Комбинация                    | Частота | Эмпирическая | Теоретическая |\n";
        cout << string(70, '-') << "\n";
        cout << "| Три одного значения, одно др. | " << setw(7) << count_three_one 
             << " | " << setw(12) << fixed << setprecision(6) << (double)count_three_one/trials 
             << " | " << setw(13) << (deck_size==36?0.01956:0.02357) << " |\n";
        cout << "| Два одного, два другого       | " << setw(7) << count_two_two 
             << " | " << setw(12) << (double)count_two_two/trials 
             << " | " << setw(13) << (deck_size==36?0.02200:0.1349) << " |\n";
        cout << "| Все разного значения           | " << setw(7) << count_all_diff 
             << " | " << setw(12) << (double)count_all_diff/trials 
             << " | " << setw(13) << (deck_size==36?0.5476:0.6761) << " |\n";
        cout << "| Все одного значения            | " << setw(7) << count_all_same 
             << " | " << setw(12) << (double)count_all_same/trials 
             << " | " << setw(13) << (deck_size==36?0.000153:0.000048) << " |\n";
        cout << string(70, '-') << "\n";
        
        // С возвращением
        cout << "\n--- С ВОЗВРАЩЕНИЕМ ---\n";
        count_three_one = 0;
        count_two_two = 0;
        count_all_diff = 0;
        count_all_same = 0;
        
        for (int t = 0; t < trials; ++t) {
            vector<int> hand(4);
            for (int i = 0; i < 4; ++i) {
                hand[i] = random_int(0, num_values - 1);
            }
            
            map<int, int> value_count;
            for (int card : hand) {
                value_count[card]++;
            }
            
            vector<int> counts;
            for (auto& p : value_count) {
                counts.push_back(p.second);
            }
            sort(counts.begin(), counts.end());
            
            if (counts.size() == 1 && counts[0] == 4) {
                count_all_same++;
            } else if (counts.size() == 2 && counts[0] == 1 && counts[1] == 3) {
                count_three_one++;
            } else if (counts.size() == 2 && counts[0] == 2 && counts[1] == 2) {
                count_two_two++;
            } else if (counts.size() == 4) {
                count_all_diff++;
            }
        }
        
        cout << "Результаты после " << trials << " испытаний:\n";
        cout << string(70, '-') << "\n";
        cout << "| Комбинация                    | Частота | Эмпирическая | Теоретическая |\n";
        cout << string(70, '-') << "\n";
        
        double total_combos = pow(num_values, 4);
        double theo_three_one = 4.0 * num_values * (num_values-1) / total_combos;
        double theo_two_two = 6.0 * num_values * (num_values-1) / 2.0 / total_combos;
        double theo_all_diff = (num_values * (num_values-1) * (num_values-2) * (num_values-3)) / total_combos;
        double theo_all_same = 1.0 * num_values / total_combos;
        
        cout << "| Три одного значения, одно др. | " << setw(7) << count_three_one 
             << " | " << setw(12) << fixed << setprecision(6) << (double)count_three_one/trials 
             << " | " << setw(13) << theo_three_one << " |\n";
        cout << "| Два одного, два другого       | " << setw(7) << count_two_two 
             << " | " << setw(12) << (double)count_two_two/trials 
             << " | " << setw(13) << theo_two_two << " |\n";
        cout << "| Все разного значения           | " << setw(7) << count_all_diff 
             << " | " << setw(12) << (double)count_all_diff/trials 
             << " | " << setw(13) << theo_all_diff << " |\n";
        cout << "| Все одного значения            | " << setw(7) << count_all_same 
             << " | " << setw(12) << (double)count_all_same/trials 
             << " | " << setw(13) << theo_all_same << " |\n";
        cout << string(70, '-') << "\n";
    }
}

void print_usage(const char* program_name) {
    cout << "Использование: " << program_name << " <количество_ключей> <количество_экспериментов>\n";
    cout << "Пример: " << program_name << " 10 100000\n";
    cout << "\nИли запустите без аргументов для выбора задачи из меню.\n";
}

int main(int argc, char* argv[]) {
    // Проверяем, запущена ли программа с аргументами для задачи про ключи
    if (argc == 3) {
        // Запуск задачи про ключи с параметрами из командной строки
        int n = atoi(argv[1]);
        int K = atoi(argv[2]);
        
        if (n <= 0 || K <= 0) {
            cout << "Ошибка: количество ключей и экспериментов должны быть положительными числами\n";
            return 1;
        }
        
        task_keys(n, K);
        return 0;
    }
    
    // Если аргументов нет, показываем меню
    int choice, trials;
    
    cout << "\n========================================\n";
    cout << "МОДЕЛИРОВАНИЕ ВЕРОЯТНОСТНЫХ ЗАДАЧ\n";
    cout << "========================================\n\n";
    
    cout << "Сколько испытаний провести? (рекомендуется 100000 для точности) ";
    cin >> trials;
    
    cout << "\nВыберите задачу:\n";
    cout << "68 - Информирование группы\n";
    cout << "69 - Карты из колоды\n";
    cout << "keys - Поиск нужного ключа (отдельный режим)\n";
    cout << "Ваш выбор (68/69/keys): ";
    
    string choice_str;
    cin >> choice_str;
    
    if (choice_str == "68") {
        task68_new(trials);
    }
    else if (choice_str == "69") {
        task69_new(trials);
    }
    else if (choice_str == "keys") {
        int n;
        cout << "Введите количество ключей: ";
        cin >> n;
        task_keys(n, trials);
    }
    else {
        cout << "Неверный выбор\n";
        print_usage(argv[0]);
    }
    
    return 0;
}