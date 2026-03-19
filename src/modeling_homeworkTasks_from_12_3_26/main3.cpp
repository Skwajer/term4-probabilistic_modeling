#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <random>
#include <iomanip>
#include <cmath>

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

// Базовый класс для шифров
class Cipher {
protected:
    map<string, double> text_probs;     // P(M)
    map<string, double> key_probs;       // P(Key)
    vector<string> texts;
    vector<string> keys;
    
public:
    Cipher(vector<string> t, vector<double> tp, vector<string> k, vector<double> kp) {
        texts = t;
        keys = k;
        for (size_t i = 0; i < texts.size(); i++) text_probs[texts[i]] = tp[i];
        for (size_t i = 0; i < keys.size(); i++) key_probs[keys[i]] = kp[i];
    }
    
    virtual string encrypt(string text, string key) = 0;
    virtual vector<string> get_all_ciphertexts() = 0;
    
    // P(C = c)
    double prob_ciphertext(string c) {
        double prob = 0;
        for (string m : texts) {
            for (string k : keys) {
                if (encrypt(m, k) == c) {
                    prob += text_probs[m] * key_probs[k];
                }
            }
        }
        return prob;
    }
    
    // P(C = c | M = m)
    double prob_ciphertext_given_text(string c, string m) {
        double prob = 0;
        for (string k : keys) {
            if (encrypt(m, k) == c) {
                prob += key_probs[k];
            }
        }
        return prob;
    }
    
    // P(M = m | C = c) - формула Байеса
    double prob_text_given_ciphertext(string m, string c) {
        double p_c = prob_ciphertext(c);
        if (p_c == 0) return 0;
        return text_probs[m] * prob_ciphertext_given_text(c, m) / p_c;
    }
    
    // Статистическая атака на перехваченный шифротекст
    void attack(string intercepted_ciphertext) {
        cout << "\n🔍 АТАКА на перехваченный шифротекст: \"" << intercepted_ciphertext << "\"\n";
        cout << "========================================\n\n";
        
        cout << "Априорные вероятности текстов (до перехвата):\n";
        for (string m : texts) {
            cout << "  P(" << m << ") = " << fixed << setprecision(4) << text_probs[m] << "\n";
        }
        
        cout << "\nАпостериорные вероятности (после перехвата " << intercepted_ciphertext << "):\n";
        
        vector<pair<string, double>> posteriors;
        double total = 0;
        
        for (string m : texts) {
            double prob = prob_text_given_ciphertext(m, intercepted_ciphertext);
            posteriors.push_back({m, prob});
            total += prob;
        }
        
        // Нормируем (на случай погрешностей)
        for (auto& p : posteriors) {
            p.second /= total;
        }
        
        // Сортируем по убыванию вероятности
        sort(posteriors.begin(), posteriors.end(), 
             [](auto a, auto b) { return a.second > b.second; });
        
        for (auto [m, prob] : posteriors) {
            cout << "  P(" << m << " | " << intercepted_ciphertext << ") = " 
                 << fixed << setprecision(4) << prob;
            
            // Показываем изменение вероятности
            double change = prob - text_probs[m];
            if (abs(change) > 0.001) {
                cout << " (изменение: " << (change > 0 ? "+" : "") << change << ")";
            }
            cout << "\n";
        }
        
        // Наиболее вероятный открытый текст
        cout << "\n🏆 Наиболее вероятный открытый текст: \"" << posteriors[0].first 
             << "\" с вероятностью " << fixed << setprecision(4) << posteriors[0].second << "\n";
    }
};

// Шифр Виженера (только буквы A-Z)
class VigenereCipher : public Cipher {
private:
    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    char shift_char(char c, int shift) {
        if (c < 'A' || c > 'Z') return c;
        int idx = (c - 'A' + shift) % 26;
        if (idx < 0) idx += 26;
        return 'A' + idx;
    }
    
public:
    VigenereCipher(vector<string> t, vector<double> tp, vector<string> k, vector<double> kp) 
        : Cipher(t, tp, k, kp) {}
    
    string encrypt(string text, string key) override {
        string result = text;
        for (size_t i = 0; i < text.length(); i++) {
            int shift = key[i % key.length()] - 'A';
            result[i] = shift_char(text[i], shift);
        }
        return result;
    }
    
    vector<string> get_all_ciphertexts() override {
        vector<string> all;
        for (string m : texts) {
            for (string k : keys) {
                all.push_back(encrypt(m, k));
            }
        }
        // Уникальные
        sort(all.begin(), all.end());
        all.erase(unique(all.begin(), all.end()), all.end());
        return all;
    }
};

// Шифр Вернама (XOR) - для простоты работаем с числами
class VernamCipher : public Cipher {
public:
    VernamCipher(vector<string> t, vector<double> tp, vector<string> k, vector<double> kp) 
        : Cipher(t, tp, k, kp) {}
    
    string encrypt(string text, string key) override {
        // XOR для строк (превращаем в числа)
        string result = text;
        for (size_t i = 0; i < text.length(); i++) {
            result[i] = text[i] ^ key[i % key.length()];
        }
        return result;
    }
    
    vector<string> get_all_ciphertexts() override {
        vector<string> all;
        for (string m : texts) {
            for (string k : keys) {
                all.push_back(encrypt(m, k));
            }
        }
        sort(all.begin(), all.end());
        all.erase(unique(all.begin(), all.end()), all.end());
        return all;
    }
    
    // Для демонстрации преобразуем в hex представление
    string to_hex(string s) {
        const char* hex = "0123456789ABCDEF";
        string result;
        for (unsigned char c : s) {
            result += hex[c >> 4];
            result += hex[c & 0x0F];
        }
        return result;
    }
};

// Генерация случайного ключа Виженера
string random_vigenere_key(int len) {
    string key;
    for (int i = 0; i < len; i++) {
        key += 'A' + random_int(0, 25);
    }
    return key;
}

// Демонстрация для шифра Виженера
void demonstrate_vigenere() {
    cout << "\n══════════════════════════════════════════════\n";
    cout << "     🔐 АТАКА НА ШИФР ВИЖЕНЕРА\n";
    cout << "══════════════════════════════════════════════\n";
    
    // Открытые тексты (английские слова)
    vector<string> texts = {"HELLO", "WORLD", "CRYPTO", "SECURE", "ATTACK"};
    vector<double> text_probs = {0.3, 0.25, 0.2, 0.15, 0.1};
    
    // Ключи (разные длины)
    vector<string> keys = {"A", "B", "C", "AB", "BC", "ABC", "KEY", "SECRET"};
    vector<double> key_probs = {0.2, 0.15, 0.15, 0.1, 0.1, 0.1, 0.1, 0.1};
    
    VigenereCipher cipher(texts, text_probs, keys, key_probs);
    
    cout << "\n📋 Открытые тексты и их вероятности:\n";
    for (size_t i = 0; i < texts.size(); i++) {
        cout << "  " << texts[i] << " : " << text_probs[i] << "\n";
    }
    
    cout << "\n🔑 Ключи и их вероятности:\n";
    for (size_t i = 0; i < keys.size(); i++) {
        cout << "  " << keys[i] << " : " << key_probs[i] << "\n";
    }
    
    // Выбираем случайный текст и ключ
    discrete_distribution<> text_dist(text_probs.begin(), text_probs.end());
    discrete_distribution<> key_dist(key_probs.begin(), key_probs.end());
    
    string selected_text = texts[text_dist(gen)];
    string selected_key = keys[key_dist(gen)];
    string ciphertext = cipher.encrypt(selected_text, selected_key);
    
    cout << "\n📨 Отправляем сообщение:\n";
    cout << "  Открытый текст: " << selected_text << "\n";
    cout << "  Ключ: " << selected_key << "\n";
    cout << "  Шифротекст: " << ciphertext << "\n";
    
    // Атака
    cipher.attack(ciphertext);
    
    // Проверка всех возможных шифротекстов
    cout << "\n📊 Статистика по всем возможным шифротекстам:\n";
    auto all_ciphers = cipher.get_all_ciphertexts();
    for (string c : all_ciphers) {
        double p = cipher.prob_ciphertext(c);
        if (p > 0.05) { // Показываем только значимые
            cout << "  P(" << c << ") = " << fixed << setprecision(4) << p << "\n";
        }
    }
}

// Демонстрация для шифра Вернама
void demonstrate_vernam() {
    cout << "\n══════════════════════════════════════════════\n";
    cout << "     🔐 АТАКА НА ШИФР ВЕРНАМА (XOR)\n";
    cout << "══════════════════════════════════════════════\n";
    
    // Открытые тексты (числа в виде строк)
    vector<string> texts = {"0A1B", "2C3D", "4E5F", "6A7B", "8C9D"};
    vector<double> text_probs = {0.25, 0.25, 0.2, 0.2, 0.1};
    
    // Ключи (случайные строки)
    vector<string> keys = {"12", "34", "56", "78", "9A", "BC", "DE", "F0"};
    vector<double> key_probs = {0.2, 0.15, 0.15, 0.1, 0.1, 0.1, 0.1, 0.1};
    
    VernamCipher cipher(texts, text_probs, keys, key_probs);
    
    cout << "\n📋 Открытые тексты (hex) и их вероятности:\n";
    for (size_t i = 0; i < texts.size(); i++) {
        cout << "  " << texts[i] << " : " << text_probs[i] << "\n";
    }
    
    cout << "\n🔑 Ключи (hex) и их вероятности:\n";
    for (size_t i = 0; i < keys.size(); i++) {
        cout << "  " << keys[i] << " : " << key_probs[i] << "\n";
    }
    
    // Выбираем случайный текст и ключ
    discrete_distribution<> text_dist(text_probs.begin(), text_probs.end());
    discrete_distribution<> key_dist(key_probs.begin(), key_probs.end());
    
    string selected_text = texts[text_dist(gen)];
    string selected_key = keys[key_dist(gen)];
    string ciphertext = cipher.encrypt(selected_text, selected_key);
    
    cout << "\n📨 Отправляем сообщение:\n";
    cout << "  Открытый текст (hex): " << selected_text << "\n";
    cout << "  Ключ (hex): " << selected_key << "\n";
    cout << "  Шифротекст (raw): " << ciphertext << "\n";
    cout << "  Шифротекст (hex): " << cipher.to_hex(ciphertext) << "\n";
    
    // Атака
    cipher.attack(ciphertext);
}

// Демонстрация сравнения шифров
void compare_ciphers() {
    cout << "\n══════════════════════════════════════════════\n";
    cout << "     ⚔️  СРАВНЕНИЕ СТОЙКОСТИ ШИФРОВ\n";
    cout << "══════════════════════════════════════════════\n";
    
    // Одинаковые условия для обоих шифров
    vector<string> texts = {"HELLO", "WORLD", "CRYPTO"};
    vector<double> text_probs = {0.5, 0.3, 0.2};
    
    vector<string> keys_vigenere = {"A", "B", "C"};
    vector<string> keys_vernam = {"12", "34", "56"};
    vector<double> key_probs = {0.4, 0.3, 0.3};
    
    VigenereCipher vig(texts, text_probs, keys_vigenere, key_probs);
    VernamCipher vern(texts, text_probs, keys_vernam, key_probs);
    
    cout << "\n📊 Энтропия (неопределенность) открытых текстов: H(M) = ";
    double h_text = 0;
    for (double p : text_probs) {
        if (p > 0) h_text -= p * log2(p);
    }
    cout << fixed << setprecision(4) << h_text << " бит\n";
    
    cout << "\n🔍 Анализ для шифра Виженера:\n";
    auto vig_ciphers = vig.get_all_ciphertexts();
    cout << "  Количество возможных шифротекстов: " << vig_ciphers.size() << "\n";
    
    double h_cipher_vig = 0;
    for (string c : vig_ciphers) {
        double p = vig.prob_ciphertext(c);
        if (p > 0) h_cipher_vig -= p * log2(p);
    }
    cout << "  Энтропия шифротекстов: " << h_cipher_vig << " бит\n";
    cout << "  H(M) / H(C) = " << h_text / h_cipher_vig << "\n";
    
    cout << "\n🔍 Анализ для шифра Вернама:\n";
    auto vern_ciphers = vern.get_all_ciphertexts();
    cout << "  Количество возможных шифротекстов: " << vern_ciphers.size() << "\n";
    
    double h_cipher_vern = 0;
    for (string c : vern_ciphers) {
        double p = vern.prob_ciphertext(c);
        if (p > 0) h_cipher_vern -= p * log2(p);
    }
    cout << "  Энтропия шифротекстов: " << h_cipher_vern << " бит\n";
    cout << "  H(M) / H(C) = " << h_text / h_cipher_vern << "\n";
    
    cout << "\n📌 Вывод: Чем ближе H(M)/H(C) к 1, тем лучше шифр скрывает статистику.\n";
    cout << "   Для идеального шифра (одноразовый блокнот) H(M) = H(C).\n";
}

int main() {
    cout << "══════════════════════════════════════════════════════════\n";
    cout << "     🕵️ СТАТИСТИЧЕСКИЙ АНАЛИЗ И АТАКИ НА ШИФРЫ\n";
    cout << "══════════════════════════════════════════════════════════\n";
    
    int choice;
    do {
        cout << "\nМеню:\n";
        cout << "1. Атака на шифр Виженера\n";
        cout << "2. Атака на шифр Вернама (XOR)\n";
        cout << "3. Сравнить стойкость шифров\n";
        cout << "0. Выход\n";
        cout << "Выбор: ";
        cin >> choice;
        
        switch (choice) {
            case 1: demonstrate_vigenere(); break;
            case 2: demonstrate_vernam(); break;
            case 3: compare_ciphers(); break;
            case 0: cout << "До свидания!\n"; break;
            default: cout << "Неверный выбор!\n";
        }
    } while (choice != 0);
    
    return 0;
}