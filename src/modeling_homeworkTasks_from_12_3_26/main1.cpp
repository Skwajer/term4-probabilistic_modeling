#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <iomanip>

class FamilyProbability {
private:
    double lambda;
    std::mt19937 gen;
    std::poisson_distribution<> poisson_dist;
    std::bernoulli_distribution gender_dist;
    
public:
    FamilyProbability(double l = 1.94) : 
        lambda(l), 
        gen(std::random_device{}()),
        poisson_dist(l),
        gender_dist(0.5) {}
    
    double analytical_probability(int m, int max_k = 20) {
        double numerator = 0.0;
        double denominator = 0.0;
        
        for (int k = 0; k <= max_k; k++) {
            double p_k = std::exp(-lambda) * std::pow(lambda, k);
            for (int i = 1; i <= k; i++) {
                p_k /= i;
            }
            
            double no_girls_given_k = (k == 0) ? 1.0 : std::pow(0.5, k);
            
            denominator += p_k * no_girls_given_k;
            
            if (k >= m) {
                numerator += p_k * no_girls_given_k;
            }
        }
        
        return (denominator > 0) ? numerator / denominator : 0.0;
    }
    
    double empirical_probability(int m, int num_families = 1000000) {
        int total_without_girls = 0;
        int total_without_girls_and_ge_m = 0;
        
        for (int i = 0; i < num_families; i++) {
            int num_children = poisson_dist(gen);
            
            bool has_girls = false;
            for (int j = 0; j < num_children; j++) {
                if (gender_dist(gen)) {
                    has_girls = true;
                    break;
                }
            }
            
            if (!has_girls) {
                total_without_girls++;
                if (num_children >= m) {
                    total_without_girls_and_ge_m++;
                }
            }
        }
        
        return (total_without_girls > 0) ? 
            static_cast<double>(total_without_girls_and_ge_m) / total_without_girls : 0.0;
    }
    
    double unconditional_probability(int m, int max_k = 20) {
        double prob = 0.0;
        for (int k = m; k <= max_k; k++) {
            double p_k = std::exp(-lambda) * std::pow(lambda, k);
            for (int i = 1; i <= k; i++) {
                p_k /= i;
            }
            prob += p_k;
        }
        return prob;
    }
    
    void compare_probabilities(int m) {
        double cond = analytical_probability(m);
        double uncond = unconditional_probability(m);
        
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "\n=== Для m = " << m << " ===" << std::endl;
        std::cout << "Условная вероятность P(K >= " << m << " | нет девочек): " 
                  << cond * 100 << "%" << std::endl;
        std::cout << "Безусловная вероятность P(K >= " << m << "): " 
                  << uncond * 100 << "%" << std::endl;
        std::cout << "Отношение (усл/безусл): " << cond / uncond << std::endl;
        std::cout << "Разница: " << (cond - uncond) * 100 << " п.п." << std::endl;
    }
    
    void print_table(int max_m = 5) {
        std::cout << "\n================================================" << std::endl;
        std::cout << "m\tP(K>=m|D=0)\tP(K>=m)\tОтношение" << std::endl;
        std::cout << "================================================" << std::endl;
        
        for (int m = 1; m <= max_m; m++) {
            double cond = analytical_probability(m);
            double uncond = unconditional_probability(m);
            std::cout << m << "\t" 
                      << std::setw(8) << std::setprecision(4) << cond * 100 << "%\t"
                      << std::setw(8) << uncond * 100 << "%\t"
                      << std::setw(6) << cond / uncond << std::endl;
        }
    }
};

int main() {
    std::cout << "=== Анализ вероятностей количества детей ===" << std::endl;
    std::cout << "Распределение Пуассона с λ = 1.94" << std::endl;
    
    FamilyProbability fp(1.94);
    
    fp.print_table(5);
    
    std::cout << "\n=== Сравнение аналитического и эмпирического методов ===" << std::endl;
    
    for (int m = 1; m <= 3; m++) {
        double analytical = fp.analytical_probability(m);
        double empirical = fp.empirical_probability(m, 1000000);
        
        std::cout << "\nm = " << m << ":" << std::endl;
        std::cout << "  Аналитически: " << analytical * 100 << "%" << std::endl;
        std::cout << "  Эмпирически:  " << empirical * 100 << "%" << std::endl;
        std::cout << "  Разница:      " << std::abs(analytical - empirical) * 100 << " п.п." << std::endl;
    }
    
    int m_choice = 2;
    std::cout << "\n=== Детальный анализ для m = " << m_choice << " ===" << std::endl;
    fp.compare_probabilities(m_choice);
    
    return 0;
}