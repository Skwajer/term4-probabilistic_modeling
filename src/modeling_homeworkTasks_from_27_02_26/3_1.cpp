#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <random>
#include <algorithm>
#include <iomanip>

class ClusterAnalyzer {
private:
    std::vector<char> alphabet;   
    int n;                           
    int k;                     
    std::string pattern;       
    std::map<char, double> distribution;   
    std::mt19937 rng;      
    
    char generateRandomChar() {
        static std::uniform_real_distribution<> dist(0.0, 1.0);
        double r = dist(rng);
        double cumulative = 0.0;
        
        for (const auto& [symbol, prob] : distribution) {
            cumulative += prob;
            if (r <= cumulative) {
                return symbol;
            }
        }
        return distribution.begin()->first;
    }
    
public:
    bool areClustersConnected(const std::string& cluster1, const std::string& cluster2) {
        for (int left_len = 1; left_len <= std::min(k - 1, n); ++left_len) {
            int right_len = k - left_len;
            
            if (right_len < 1 || right_len > n) continue;
            
            std::string left_part = cluster1.substr(n - left_len, left_len);
            
            std::string right_part = cluster2.substr(0, right_len);
            
            if (left_part + right_part == pattern) {
                return true;
            }
        }
        return false;
    }
    
public:
    ClusterAnalyzer(const std::vector<char>& alph, int size_n, int size_k, 
                    const std::string& pat, const std::map<char, double>& dist)
        : alphabet(alph), n(size_n), k(size_k), pattern(pat), distribution(dist) {
        
        std::random_device rd;
        rng = std::mt19937(rd());
        
        double sum = 0.0;
        for (const auto& [sym, prob] : distribution) {
            sum += prob;
        }
        if (std::abs(sum - 1.0) > 1e-6) {
            std::cerr << "Warning: Distribution probabilities sum to " << sum 
                      << ", should be 1.0" << std::endl;
        }
    }
    
    std::string generateCluster() {
        std::string cluster;
        for (int i = 0; i < n; ++i) {
            cluster += generateRandomChar();
        }
        return cluster;
    }
    
    std::vector<std::string> generateClusters(int M) {
        std::vector<std::string> clusters;
        clusters.reserve(M);
        for (int i = 0; i < M; ++i) {
            clusters.push_back(generateCluster());
        }
        return clusters;
    }
    
    struct AnalysisResult {
        double prob_all_connected;
        double prob_D_connected;    
        double prob_all_not_connected;   
        std::vector<std::string> all_connected;     
        std::vector<std::string> D_connected;        
        std::vector<std::string> all_not_connected;  
        int total_series;               
        std::map<int, int> distribution_D; 
    };
    
    AnalysisResult analyze(int M, int num_series = 1000) {
        AnalysisResult result;
        result.total_series = num_series;
        
        int count_all_connected = 0;
        int count_all_not_connected = 0; 
        std::map<int, int> D_counts;
        
        std::vector<std::vector<std::string>> examples_all_connected;
        std::vector<std::vector<std::string>> examples_D_connected(101);
        std::vector<std::vector<std::string>> examples_all_not_connected;
        
        for (int series = 0; series < num_series; ++series) {
            std::vector<std::string> clusters = generateClusters(M);
            
            std::vector<bool> connected(M - 1, false);
            
            for (int i = 0; i < M - 1; ++i) {
                connected[i] = areClustersConnected(clusters[i], clusters[i + 1]);
            }
            
            int D = std::count(connected.begin(), connected.end(), true);
            D_counts[D]++;
            

            if (D == M - 1) { 
                count_all_connected++;
                if (examples_all_connected.size() < 5) { 
                    examples_all_connected.push_back(clusters);
                }
            }
            
            if (D == 0) {
                count_all_not_connected++;
                if (examples_all_not_connected.size() < 5) {
                    examples_all_not_connected.push_back(clusters);
                }
            }
            
            if (D <= 100 && examples_D_connected[D].empty()) {
                examples_D_connected[D] = clusters;
            }
        }
        
        result.prob_all_connected = static_cast<double>(count_all_connected) / num_series;
        result.prob_all_not_connected = static_cast<double>(count_all_not_connected) / num_series;
        result.distribution_D = D_counts;
        
        int most_frequent_D = 0;
        int max_count = 0;
        for (const auto& [d, count] : D_counts) {
            if (count > max_count) {
                max_count = count;
                most_frequent_D = d;
            }
        }
        result.prob_D_connected = static_cast<double>(max_count) / num_series;
        
        if (!examples_all_connected.empty()) {
            result.all_connected = examples_all_connected[0];
        }
        
        if (!examples_all_not_connected.empty()) {
            result.all_not_connected = examples_all_not_connected[0];
        }
        
        if (!examples_D_connected[most_frequent_D].empty()) {
            result.D_connected = examples_D_connected[most_frequent_D];
        }
        
        return result;
    }
    
    static void testExample() {
        std::cout << "\n=== Testing example from problem ===\n";
        std::cout << "Alphabet: {0, 1}, n = 5, pattern: 101\n";
        
        std::vector<std::pair<std::string, std::string>> test_pairs = {
            {"10010", "11010"},  
            {"10101", "01000"},  
            {"10011", "10111"} 
        };
        
        std::vector<char> alph = {'0', '1'};
        std::map<char, double> uniform = {{'0', 0.5}, {'1', 0.5}};
        ClusterAnalyzer temp(alph, 5, 3, "101", uniform);
        
        for (const auto& [c1, c2] : test_pairs) {
            bool connected = temp.areClustersConnected(c1, c2);
            std::cout << "Clusters [" << c1 << "] and [" << c2 << "]: "
                      << (connected ? "CONNECTED" : "NOT CONNECTED") << "\n";
        }
    }
};

int main() {
    std::vector<char> alphabet = {'0', '1'};
    int n = 5;
    int k = 3;
    std::string pattern = "101";
    int M = 100;
    int num_series = 10000;
    
    std::map<char, double> distribution;
    distribution['0'] = 0.5;
    distribution['1'] = 0.5;
    
    std::cout << "=== Cluster Connectivity Analysis ===\n";
    std::cout << "Parameters:\n";
    std::cout << "  Alphabet: {";
    for (size_t i = 0; i < alphabet.size(); ++i) {
        std::cout << (i > 0 ? ", " : "") << alphabet[i];
    }
    std::cout << "}\n";
    std::cout << "  Cluster size (n): " << n << "\n";
    std::cout << "  Pattern length (k): " << k << "\n";
    std::cout << "  Pattern: " << pattern << "\n";
    std::cout << "  Clusters per series (M): " << M << "\n";
    std::cout << "  Number of series: " << num_series << "\n";
    std::cout << "  Symbol distribution:\n";
    for (const auto& [sym, prob] : distribution) {
        std::cout << "    P('" << sym << "') = " << prob << "\n";
    }
    std::cout << std::endl;
    
    ClusterAnalyzer analyzer(alphabet, n, k, pattern, distribution);
    
    ClusterAnalyzer::testExample();
    
    std::cout << "\n=== Starting Analysis ===\n";
    auto result = analyzer.analyze(M, num_series);
    
    std::cout << "\n=== Results ===\n";
    std::cout << std::fixed << std::setprecision(4);
    
    std::cout << "\na) Probability that ALL pairs are connected: "
              << result.prob_all_connected * 100 << "%\n";
    if (!result.all_connected.empty()) {
        std::cout << "   Example series with all pairs connected:\n";
        for (size_t i = 0; i < result.all_connected.size(); ++i) {
            std::cout << "     Cluster " << i+1 << ": " << result.all_connected[i] << "\n";
        }
        std::cout << "   Connectivity check for this example:\n";
        for (size_t i = 0; i < result.all_connected.size() - 1; ++i) {
            bool conn = analyzer.areClustersConnected(
                result.all_connected[i], result.all_connected[i+1]);
            std::cout << "     Pair " << i+1 << "-" << i+2 << ": " 
                      << (conn ? "connected" : "not connected") << "\n";
        }
    }
    
    std::cout << "\nb) Most frequent number of connected pairs (D):\n";
    int max_D = 0, max_count = 0;
    for (const auto& [d, count] : result.distribution_D) {
        if (count > max_count) {
            max_count = count;
            max_D = d;
        }
    }
    std::cout << "   D = " << max_D << " occurred in " 
              << result.prob_D_connected * 100 << "% of series\n";
    
    if (!result.D_connected.empty()) {
        std::cout << "   Example series with D = " << max_D << ":\n";
        for (size_t i = 0; i < result.D_connected.size(); ++i) {
            std::cout << "     Cluster " << i+1 << ": " << result.D_connected[i] << "\n";
        }
        std::cout << "   Connectivity for this example (D = " << max_D << "):\n";
        int actual_D = 0;
        for (size_t i = 0; i < result.D_connected.size() - 1; ++i) {
            bool conn = analyzer.areClustersConnected(
                result.D_connected[i], result.D_connected[i+1]);
            std::cout << "     Pair " << i+1 << "-" << i+2 << ": " 
                      << (conn ? "connected" : "not connected") << "\n";
            if (conn) actual_D++;
        }
        std::cout << "     Actual D = " << actual_D << "\n";
    }
    
    std::cout << "\nc) Probability that NO pairs are connected: "
              << result.prob_all_not_connected * 100 << "%\n";
    if (!result.all_not_connected.empty()) {
        std::cout << "   Example series with no connected pairs:\n";
        for (size_t i = 0; i < result.all_not_connected.size(); ++i) {
            std::cout << "     Cluster " << i+1 << ": " << result.all_not_connected[i] << "\n";
        }
        std::cout << "   Connectivity check for this example:\n";
        for (size_t i = 0; i < result.all_not_connected.size() - 1; ++i) {
            bool conn = analyzer.areClustersConnected(
                result.all_not_connected[i], result.all_not_connected[i+1]);
            std::cout << "     Pair " << i+1 << "-" << i+2 << ": " 
                      << (conn ? "connected" : "not connected") << "\n";
        }
    }
    
    std::cout << "\n=== Distribution of D (number of connected pairs) ===\n";
    std::cout << "D\tCount\tPercentage\n";
    for (const auto& [d, count] : result.distribution_D) {
        double percentage = 100.0 * count / num_series;
        std::cout << d << "\t" << count << "\t" << std::fixed 
                  << std::setprecision(2) << percentage << "%\n";
    }
    
    return 0;
}