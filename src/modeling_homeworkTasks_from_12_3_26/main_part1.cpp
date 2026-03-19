#include <iostream>
#include <random>

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

// Задача 92: Пирамида из 5 винтовок (3 с оптикой, 2 без)
void task92(int trials) {
    cout << "\n=== Задача 92: Пирамида из 5 винтовок ===\n";
    cout << "Условие: 3 винтовки с оптикой (p=0.95), 2 без оптики (p=0.7)\n";
    
    int hits = 0;
    
    for (int t = 0; t < trials; ++t) {
        int rifle = random_int(0, 4);
        bool has_optics = (rifle < 3);
        
        double prob = has_optics ? 0.95 : 0.7;
        bool hit = random_double(0, 1) < prob;
        
        cout << "Испытание " << t+1 << ": выбрана винтовка ";
        if (has_optics) {
            cout << "с оптикой (№" << rifle+1 << ")";
        } else {
            cout << "без оптики (№" << rifle+1 << ")";
        }
        
        if (hit) {
            cout << " → ПОПАДАНИЕ";
            hits++;
        } else {
            cout << " → промах";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Всего попаданий: " << hits << ", вероятность " << (double)hits/trials << "\n";
    cout << "Теоретически: 0.85\n";
}

// Задача 93: Детали с трех заводов
void task93(int trials) {
    cout << "\n=== Задача 93: Детали с трех заводов ===\n";
    cout << "Условие: Завод1:12 деталей (p=0.9), Завод2:20 деталей (p=0.6), Завод3:18 деталей (p=0.9)\n";
    
    int excellent = 0;
    
    for (int t = 0; t < trials; ++t) {
        int part = random_int(0, 49);
        int factory;
        if (part < 12) factory = 1;
        else if (part < 32) factory = 2;
        else factory = 3;
        
        double prob = (factory == 2) ? 0.6 : 0.9;
        bool is_excellent = random_double(0, 1) < prob;
        
        cout << "Испытание " << t+1 << ": деталь с завода №" << factory;
        
        if (is_excellent) {
            cout << " → ОТЛИЧНОГО качества";
            excellent++;
        } else {
            cout << " → обычного качества";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Отличных деталей: " << excellent << ", вероятность " << (double)excellent/trials << "\n";
    cout << "Теоретически: 0.78\n";
}

// Задача 95: Три урны с перекладыванием
void task95(int trials) {
    cout << "\n=== Задача 95: Три урны с перекладыванием ===\n";
    cout << "Условие: в каждой урне 6 черных и 4 белых шара. Перекладываем по одному из 1->2, 2->3, затем из 3\n";
    
    int white_from_third = 0;
    
    for (int t = 0; t < trials; ++t) {
        int urn1_white = 4, urn1_black = 6;
        int urn2_white = 4, urn2_black = 6;
        int urn3_white = 4, urn3_black = 6;
        
        cout << "Испытание " << t+1 << ":\n";
        
        int total1 = urn1_white + urn1_black;
        int rnd = random_int(0, total1 - 1);
        bool from1_white = (rnd < urn1_white);
        
        if (from1_white) {
            urn1_white--;
            urn2_white++;
            cout << "  Из 1-й вынули БЕЛЫЙ → во 2-й добавлен белый\n";
        } else {
            urn1_black--;
            urn2_black++;
            cout << "  Из 1-й вынули ЧЕРНЫЙ → во 2-й добавлен черный\n";
        }
        
        int total2 = urn2_white + urn2_black;
        rnd = random_int(0, total2 - 1);
        bool from2_white = (rnd < urn2_white);
        
        if (from2_white) {
            urn2_white--;
            urn3_white++;
            cout << "  Из 2-й вынули БЕЛЫЙ → в 3-ю добавлен белый\n";
        } else {
            urn2_black--;
            urn3_black++;
            cout << "  Из 2-й вынули ЧЕРНЫЙ → в 3-ю добавлен черный\n";
        }
        
        int total3 = urn3_white + urn3_black;
        rnd = random_int(0, total3 - 1);
        bool from3_white = (rnd < urn3_white);
        
        cout << "  Из 3-й вынули " << (from3_white ? "БЕЛЫЙ" : "ЧЕРНЫЙ");
        if (from3_white) {
            cout << " ✓";
            white_from_third++;
        } else {
            cout << " ✗";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Белых из 3-й: " << white_from_third << ", вероятность " << (double)white_from_third/trials << "\n";
    cout << "Теоретически: 0.4\n";
}

// Задача 98: 10 винтовок, стрелок попал
void task98(int trials) {
    cout << "\n=== Задача 98: 10 винтовок (4 с оптикой p=0.95, 6 без p=0.8) ===\n";
    cout << "Стрелок попал. Что вероятнее: с оптикой или без?\n";
    
    int optics_hits = 0, no_optics_hits = 0;
    int optics_selected = 0, no_optics_selected = 0;
    
    for (int t = 0; t < trials; ++t) {
        int rifle = random_int(0, 9);
        bool has_optics = (rifle < 4);
        
        double prob = has_optics ? 0.95 : 0.8;
        bool hit = random_double(0, 1) < prob;
        
        if (has_optics) {
            optics_selected++;
            if (hit) optics_hits++;
        } else {
            no_optics_selected++;
            if (hit) no_optics_hits++;
        }
        
        if (hit) {
            cout << "Испытание " << t+1 << ": стрелок ПОПАЛ из винтовки " 
                 << (has_optics ? "с оптикой" : "без оптики") << "\n";
        }
    }
    
    int total_hits = optics_hits + no_optics_hits;
    cout << "\nРезультаты после " << trials << " испытаний (только ПОПАДАНИЯ):\n";
    cout << "Всего попаданий: " << total_hits << "\n";
    if (total_hits > 0) {
        cout << "Попаданий с оптикой:  " << optics_hits << ", вероятность " 
             << (double)optics_hits/total_hits << "\n";
        cout << "Попаданий без оптики: " << no_optics_hits << ", вероятность " 
             << (double)no_optics_hits/total_hits << "\n";
        cout << "\nТеоретически: с оптикой ≈0.442, без оптики ≈0.558\n";
    }
}

// Задача 100: Две перфораторщицы
void task100(int trials) {
    cout << "\n=== Задача 100: Две перфораторщицы ===\n";
    cout << "Условие: первая ошибается с p=0.05, вторая с p=0.1\n";
    
    int errors_detected = 0;
    int first_error_when_detected = 0;
    
    for (int t = 0; t < trials; ++t) {
        bool first_error = random_double(0, 1) < 0.05;
        bool second_error = random_double(0, 1) < 0.1;
        
        bool error_detected = first_error || second_error;
        
        cout << "Испытание " << t+1 << ": ";
        cout << "первая " << (first_error ? "ОШИБЛАСЬ" : "права") << ", ";
        cout << "вторая " << (second_error ? "ОШИБЛАСЬ" : "права");
        
        if (error_detected) {
            cout << " → ОШИБКА ОБНАРУЖЕНА";
            errors_detected++;
            if (first_error) first_error_when_detected++;
        } else {
            cout << " → ошибок нет";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Ошибок обнаружено: " << errors_detected << "\n";
    if (errors_detected > 0) {
        double prob = (double)first_error_when_detected / errors_detected;
        cout << "Вероятность, что ошиблась ПЕРВАЯ при обнаружении ошибки: " << prob << "\n";
        cout << "Теоретически: 0.345\n";
    }
}

// Задача 102: Два товароведа
void task102(int trials) {
    cout << "\n=== Задача 102: Два товароведа ===\n";
    cout << "Условие: первый проверяет с p=0.55 (p_std=0.9), второй с p=0.45 (p_std=0.98)\n";
    
    int recognized_std = 0;
    int second_recognized = 0;
    
    for (int t = 0; t < trials; ++t) {
        int expert = random_double(0, 1) < 0.55 ? 1 : 2;
        
        double prob = (expert == 1) ? 0.9 : 0.98;
        bool recognized = random_double(0, 1) < prob;
        
        cout << "Испытание " << t+1 << ": товаровед №" << expert;
        
        if (recognized) {
            cout << " → признал СТАНДАРТНЫМ";
            recognized_std++;
            if (expert == 2) second_recognized++;
        } else {
            cout << " → признал НЕСТАНДАРТНЫМ";
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Признано стандартными: " << recognized_std << "\n";
    if (recognized_std > 0) {
        double prob = (double)second_recognized / recognized_std;
        cout << "Вероятность, что это был ВТОРОЙ товаровед: " << prob << "\n";
        cout << "Теоретически: 0.471\n";
    }
}

// Задача 104: Три гипотезы
void task104(int trials) {
    cout << "\n=== Задача 104: Три гипотезы ===\n";
    cout << "Дано: P_A(B1)=0.6, P_A(B2)=0.3. Найти P_A(B3)\n";
    
    int count = 0;
    int b3_count = 0;
    
    for (int t = 0; t < trials; ++t) {
        double rnd = random_double(0, 1);
        int hypothesis;
        
        if (rnd < 0.5) {
            hypothesis = 1;
        } else if (rnd < 0.8) {
            hypothesis = 2;
        } else {
            hypothesis = 3;
        }
        
        double prob_a;
        if (hypothesis == 1) prob_a = 0.72;
        else if (hypothesis == 2) prob_a = 0.6;
        else prob_a = 0.3;
        
        bool a_happened = random_double(0, 1) < prob_a;
        
        if (a_happened) {
            count++;
            if (hypothesis == 3) b3_count++;
        }
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    if (count > 0) {
        cout << "P_A(B3) = " << (double)b3_count/count << "\n";
    }
    cout << "Теоретически: 0.1\n";
}

// Задача 107: Три стрелка, два попадания
void task107(int trials) {
    cout << "\n=== Задача 107: Три стрелка ===\n";
    cout << "Условие: p1=0.6, p2=0.5, p3=0.4. Два попадания. Найти P(третий попал)\n";
    
    int two_hits = 0;
    int third_hit_when_two = 0;
    
    for (int t = 0; t < trials; ++t) {
        bool hit1 = random_double(0, 1) < 0.6;
        bool hit2 = random_double(0, 1) < 0.5;
        bool hit3 = random_double(0, 1) < 0.4;
        
        int hits = hit1 + hit2 + hit3;
        
        cout << "Испытание " << t+1 << ": ";
        cout << "стрелок1 " << (hit1 ? "попал" : "промах") << ", ";
        cout << "стрелок2 " << (hit2 ? "попал" : "промах") << ", ";
        cout << "стрелок3 " << (hit3 ? "попал" : "промах");
        cout << " → всего " << hits << " попаданий";
        
        if (hits == 2) {
            cout << " (ровно два!)";
            two_hits++;
            if (hit3) third_hit_when_two++;
        }
        cout << "\n";
    }
    
    cout << "\nРезультаты после " << trials << " испытаний:\n";
    cout << "Случаев с ровно двумя попаданиями: " << two_hits << "\n";
    if (two_hits > 0) {
        double prob = (double)third_hit_when_two / two_hits;
        cout << "Вероятность, что третий попал при двух попаданиях: " << prob << "\n";
        cout << "Теоретически: 10/19 ≈ 0.5263\n";
    }
}

int main() {
    int choice, trials;
    cout << "Сколько испытаний провести? ";
    cin >> trials;
    cout << "\nВыберите задачу (92, 93, 95, 98, 100, 102, 104, 107): ";
    cin >> choice;
    
    switch (choice) {
        case 92: task92(trials); break;
        case 93: task93(trials); break;
        case 95: task95(trials); break;
        case 98: task98(trials); break;
        case 100: task100(trials); break;
        case 102: task102(trials); break;
        case 104: task104(trials); break;
        case 107: task107(trials); break;
        default: cout << "wrong task number\n";
    }
    
    return 0;
}