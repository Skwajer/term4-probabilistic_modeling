#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>

using namespace std;

random_device rd;
mt19937 gen(rd());

int random_int(int a, int b) 
{
    uniform_int_distribution<> dist(a, b);
    return dist(gen);
}

double random_double(double a, double b) 
{
    uniform_real_distribution<> dist(a, b);
    return dist(gen);
}

// ===================== TASK 1 =====================
void task1(int N, int C) 
{
    cout << "\n========== TASK 1: Выбор символов из алфавита ==========\n";
    cout << "Мощность алфавита N = " << N << ", число экспериментов C = " << C << "\n\n";

    int evenCount = N / 2;

    double p1 = (double)evenCount / N;
    double p2_no_change = (double)evenCount / N;
    double p2_with_change = (double)(evenCount - 1) / (N - 1);
    double p_both_no_change = (double)(evenCount * evenCount) / (N * N);
    double p_both_with_change = (double)(evenCount * (evenCount - 1)) / (N * (N - 1));

    cout << "РЕШЕНИЕ:\n";
    cout << "1. В первый раз выбрать чётный номер:\n";
    cout << "   P = (количество чётных) / N = " << evenCount << "/" << N << " = " << p1 << "\n\n";

    cout << "2. Во второй раз выбрать чётный номер:\n";
    cout << "   а) Индексы не меняются (выбор с возвращением):\n";
    cout << "      P = (количество чётных) / N = " << evenCount << "/" << N << " = " << p2_no_change << "\n";
    cout << "   б) Индексы меняются (выбор без возвращения):\n";
    cout << "      P = (количество чётных - 1) / (N - 1) = " << (evenCount-1) << "/" << (N-1) << " = " << p2_with_change << "\n\n";

    cout << "3. Оба раза чётные:\n";
    cout << "   а) Индексы не меняются: P = (" << evenCount << "/" << N << ")^2 = " << p_both_no_change << "\n";
    cout << "   б) Индексы меняются: P = (" << evenCount << "/" << N << ") * (" << (evenCount-1) << "/" << (N-1) << ") = " << p_both_with_change << "\n\n";

    cout << "========== EXPERIMENT STARTING... ==========\n";

    int first_even_no = 0, second_even_no = 0, both_even_no = 0;
    int first_even_with = 0, second_even_with = 0, both_even_with = 0;

    for (int exp = 0; exp < C; ++exp) {
        cout << "\n--- Эксперимент " << exp+1 << " ---\n";

        int idx1_no = random_int(1, N);
        int idx2_no = random_int(1, N);
        bool first_even_no_flag = (idx1_no % 2 == 0);
        bool second_even_no_flag = (idx2_no % 2 == 0);

        cout << "Без изменения индексов: выбраны (" << idx1_no << ", " << idx2_no << ") -> ";
        cout << (first_even_no_flag ? "первый чётный" : "первый нечётный") << ", ";
        cout << (second_even_no_flag ? "второй чётный" : "второй нечётный") << "\n";

        if (first_even_no_flag) first_even_no++;
        if (second_even_no_flag) second_even_no++;
        if (first_even_no_flag && second_even_no_flag) both_even_no++;

        vector<int> symbols(N);
        for (int i = 0; i < N; ++i) symbols[i] = i + 1;
        shuffle(symbols.begin(), symbols.end(), gen);
        int idx1_with = symbols[0];
        int idx2_with = symbols[1];
        bool first_even_with_flag = (idx1_with % 2 == 0);
        bool second_even_with_flag = (idx2_with % 2 == 0);

        cout << "С изменением индексов:   выбраны (" << idx1_with << ", " << idx2_with << ") -> ";
        cout << (first_even_with_flag ? "первый чётный" : "первый нечётный") << ", ";
        cout << (second_even_with_flag ? "второй чётный" : "второй нечётный") << "\n";

        if (first_even_with_flag) first_even_with++;
        if (second_even_with_flag) second_even_with++;
        if (first_even_with_flag && second_even_with_flag) both_even_with++;
    }

    cout << "\n========== TASK 1 RESULTS ==========\n";
    cout << "Без изменения индексов:\n";
    cout << "  P(1-й чётный) аналит = " << p1 << ", эмпирич = " << (double)first_even_no / C << "\n";
    cout << "  P(2-й чётный) аналит = " << p2_no_change << ", эмпирич = " << (double)second_even_no / C << "\n";
    cout << "  P(оба чётные) аналит = " << p_both_no_change << ", эмпирич = " << (double)both_even_no / C << "\n\n";

    cout << "С изменением индексов:\n";
    cout << "  P(1-й чётный) аналит = " << p1 << ", эмпирич = " << (double)first_even_with / C << "\n";
    cout << "  P(2-й чётный) аналит = " << p2_with_change << ", эмпирич = " << (double)second_even_with / C << "\n";
    cout << "  P(оба чётные) аналит = " << p_both_with_change << ", эмпирич = " << (double)both_even_with / C << "\n";
}

// ===================== TASK 2 =====================
void task2(int N) 
{
    cout << "\n========== TASK 2: Перестановки и теорема сложения ==========\n";
    cout << "Количество элементов N = " << N << "\n\n";

    double p_i = 1.0 / N;
    double p_j = 1.0 / N;
    double p_ij = 1.0 / (N * (N - 1));
    double p_i_or_j_analytical = p_i + p_j - p_ij;

    cout << "РЕШЕНИЕ:\n";
    cout << "1. P(A_i) = 1/N = 1/" << N << " = " << p_i << "\n";
    cout << "2. P(A_j) = 1/N = 1/" << N << " = " << p_j << "\n";
    cout << "3. P(A_i A_j) = 1/(N*(N-1)) = 1/(" << N << "*" << (N-1) << ") = " << p_ij << "\n";
    cout << "4. По теореме сложения: P(A_i + A_j) = P(A_i) + P(A_j) - P(A_i A_j) = ";
    cout << p_i << " + " << p_j << " - " << p_ij << " = " << p_i_or_j_analytical << "\n\n";

    cout << "========== EXPERIMENT STARTING... ==========\n";

    int i = 1, j = 2;

    if (N <= 8) 
    {
        vector<int> perm(N);
        for (int k = 0; k < N; ++k) perm[k] = k + 1;
        int total = 0;
        int count_i = 0, count_j = 0, count_ij = 0, count_i_or_j = 0;

        cout << "\nВсе перестановки:\n";
        do {
            total++;
            bool ai = (perm[i - 1] == i);
            bool aj = (perm[j - 1] == j);

            cout << "Перестановка " << total << ": ";
            for (int x : perm) cout << x << " ";
            cout << " | A" << i << ": " << (ai ? "да" : "нет")
                 << " | A" << j << ": " << (aj ? "да" : "нет") << "\n";

            if (ai) count_i++;
            if (aj) count_j++;
            if (ai && aj) count_ij++;
            if (ai || aj) count_i_or_j++;

        } while (next_permutation(perm.begin(), perm.end()));

        cout << "\n========== TASK 2 RESULTS ==========\n";
        cout << "Всего перестановок: " << total << "\n";
        cout << "Эмпирические вероятности:\n";
        cout << "  P(A_i) = " << (double)count_i / total << "\n";
        cout << "  P(A_j) = " << (double)count_j / total << "\n";
        cout << "  P(A_i A_j) = " << (double)count_ij / total << "\n";
        cout << "  P(A_i + A_j) = " << (double)count_i_or_j / total << "\n";
        cout << "  Проверка: P(A_i)+P(A_j)-P(A_i A_j) = "
             << (double)count_i / total + (double)count_j / total - (double)count_ij / total << "\n";
    } else 
    {
        int trials = 10000;
        int count_i = 0, count_j = 0, count_ij = 0, count_i_or_j = 0;

        for (int t = 0; t < trials; ++t) {
            vector<int> perm(N);
            for (int k = 0; k < N; ++k) perm[k] = k + 1;
            shuffle(perm.begin(), perm.end(), gen);

            bool ai = (perm[i - 1] == i);
            bool aj = (perm[j - 1] == j);

            if (t < 20) 
            {
                cout << "Эксперимент " << t+1 << ": перестановка: ";
                for (int k = 0; k < min(5, N); ++k) cout << perm[k] << " ";
                cout << "... | A" << i << ": " << (ai ? "да" : "нет")
                     << " | A" << j << ": " << (aj ? "да" : "нет") << "\n";
            }

            if (ai) count_i++;
            if (aj) count_j++;
            if (ai && aj) count_ij++;
            if (ai || aj) count_i_or_j++;
        }

        cout << "\n========== РЕЗУЛЬТАТЫ ЗАДАНИЯ 2 ==========\n";
        cout << "Эмпирические вероятности (по " << trials << " случайным перестановкам):\n";
        cout << "  P(A_i) аналит = " << p_i << ", эмпирич = " << (double)count_i / trials << "\n";
        cout << "  P(A_j) аналит = " << p_j << ", эмпирич = " << (double)count_j / trials << "\n";
        cout << "  P(A_i A_j) аналит = " << p_ij << ", эмпирич = " << (double)count_ij / trials << "\n";
        cout << "  P(A_i + A_j) аналит = " << p_i_or_j_analytical << ", эмпирич = " << (double)count_i_or_j / trials << "\n";
        cout << "  Проверка: P(A_i)+P(A_j)-P(A_i A_j) эмпирич = "
             << (double)count_i / trials + (double)count_j / trials - (double)count_ij / trials << "\n";
    }
}

// ===================== TASK 3 =====================
void task3(int k) {
    cout << "\n========== TASK3 3: Бросание монеты до двух одинаковых подряд ==========\n";
    cout << "Параметр k = " << k << " (опыт закончится до k-го бросания)\n\n";

    double p_before_k = (k > 2) ? (1.0 - pow(0.5, k - 2)) : 0.0;
    double p_even = 2.0 / 3.0;

    cout << "РЕШЕНИЕ:\n";
    cout << "1. Вероятность закончиться ровно на n-м бросании: P(n) = 1/2^(n-1)\n";
    cout << "2. а) Опыт закончится до " << k << "-го бросания:\n";
    cout << "   P = 1/2 + 1/4 + ... + 1/2^(k-2) = 1 - 1/2^(k-2) = 1 - 1/" << (1 << (k-2)) << " = " << p_before_k << "\n";
    cout << "3. б) Чётное число бросаний:\n";
    cout << "   P = 1/2 + 1/8 + 1/32 + ... = (1/2)/(1-1/4) = 2/3 ≈ " << p_even << "\n\n";

    cout << "========== EXPERIMENT STARTING... ==========\n";

    int trials = 30;
    int count_before_k = 0;
    int count_even = 0;

    for (int t = 0; t < trials; ++t) {
        cout << "\n--- Эксперимент " << t+1 << " ---\n";
        cout << "Бросания: ";

        int tosses = 0;
        int last = -1;
        int current;

        while (true) {
            current = random_int(0, 1);
            tosses++;
            cout << (current == 0 ? "О" : "Р") << " ";

            if (tosses >= 2 && current == last) break;
            last = current;
        }

        cout << " (всего " << tosses << " бросаний)\n";

        if (tosses < k) {
            cout << "   ✓ Закончился до " << k << "-го бросания\n";
            count_before_k++;
        }
        if (tosses % 2 == 0) {
            cout << "   ✓ Чётное число бросаний\n";
            count_even++;
        }
    }

    cout << "\n========== TASK 3 RESULTS ==========\n";
    cout << "Эмпирические вероятности (по " << trials << " экспериментам):\n";
    cout << "  P(опыт закончится до " << k << "-го бросания) аналит = " << p_before_k
         << ", эмпирич = " << (double)count_before_k / trials << "\n";
    cout << "  P(чётное число бросаний) аналит = " << p_even
         << ", эмпирич = " << (double)count_even / trials << "\n";
}

int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        cout << "using:\n";
        cout << "  for task 1: program 1 <N> <C>\n";
        cout << "  for task 2: program 2 <N>\n";
        cout << "  for task 3: program 3 <k>\n";
        return 1;
    }

    int task = atoi(argv[1]);

    if (task == 1) {
        if (argc < 4) {
            cout << "error: for task 1 essential N and C\n";
            return 1;
        }
        int N = atoi(argv[2]);
        int C = atoi(argv[3]);
        task1(N, C);
    } else if (task == 2) {
        if (argc < 3) {
            cout << "error: for task 2 essential N\n";
            return 1;
        }
        int N = atoi(argv[2]);
        task2(N);
    } else if (task == 3) {
        if (argc < 3) {
            cout << "error: for task 3 essential k\n";
            return 1;
        }
        int k = atoi(argv[2]);
        task3(k);
    } else {
        cout << "wrong task number. use 1, 2 or 3.\n";
    }

    return 0;
}