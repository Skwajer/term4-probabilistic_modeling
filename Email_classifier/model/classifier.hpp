#ifndef CLASSIFIER_HPP
#define CLASSIFIER_HPP

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>

struct Document
{
    std::vector<std::string> words;
    std::string type;
    Document(std::vector<std::string> init_words, std::string init_type) :
        words(init_words), type(init_type) 
    {}
};

struct Metrics
{
    double accuracy = 0.0;
    double precision = 0.0;
    double recall = 0.0;
    double f1_score = 0.0;
    
    void print() const
    {
        std::cout << "\n=== Quality Metrics ===" << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Accuracy:  " << accuracy * 100 << "%" << std::endl;
        std::cout << "Precision: " << precision * 100 << "%" << std::endl;
        std::cout << "Recall:    " << recall * 100 << "%" << std::endl;
        std::cout << "F1-score:  " << f1_score * 100 << "%" << std::endl;
    }
};

class Classifier
{
public:
    Classifier(int init_a) : m_a(init_a) {}
    
    void load_csv(std::string const &path);
    void train_model();
    std::string predict(std::vector<std::string> doc) const;
    Metrics evaluate(std::string const &test_path) const;

private:
    double calc_log_word_prob(std::string word, std::string const &type) const;
    std::vector<std::string> preprocess_text(std::string const &text) const;
    void preprocess_docs();

private:
    std::vector<Document> m_docs;
    std::unordered_set<std::string> m_vocabulary;
    size_t m_total_spam_docs = 0;
    size_t m_total_ham_docs = 0;
    std::unordered_map<std::string, std::unordered_map<std::string, size_t>> m_word_class_count;
    std::unordered_map<std::string, size_t> m_total_words_in_class;
    int m_a = 1;
    
    const std::unordered_set<std::string> m_stop_words = {
        "и", "в", "на", "с", "по", "для", "а", "но", "да", "к", "у", "за",
        "от", "из", "о", "про", "без", "через", "после", "до", "при", "об",
        "это", "эта", "этот", "что", "как", "так", "когда", "где", "кто",
        "the", "a", "an", "in", "on", "at", "for", "to", "with", "by",
        "is", "are", "was", "were", "be", "been", "being"
    };
};

#endif // CLASSIFIER_HPP