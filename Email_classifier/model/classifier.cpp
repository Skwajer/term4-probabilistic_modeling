#include "classifier.hpp"
#include <cmath>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <fstream>

void Classifier::load_csv(std::string const &path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    
    std::string line;
    std::getline(file, line);
    
    int count = 0;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string text;
        std::string label;
        
        std::getline(ss, text, ',');
        std::getline(ss, label, ',');
        
        std::vector<std::string> words = preprocess_text(text);
        
        if (!words.empty() && (label == "spam" || label == "ham")) {
            m_docs.emplace_back(words, label);
            count++;
        }
    }
    
    std::cout << "Loaded " << count << " documents from " << path << std::endl;
    file.close();
}

std::vector<std::string> Classifier::preprocess_text(std::string const &text) const
{
    std::vector<std::string> words;
    std::string word;
    std::string cleaned;
    std::cout << text << std::endl;

    
    for (char c : text) {
        if (std::isalpha(c)) {
            cleaned += std::tolower(c);
        } else if (c == ' ') {
            cleaned += ' ';
        }
    }
    
    std::stringstream ss(cleaned);
    while (ss >> word) {
        if (m_stop_words.find(word) == m_stop_words.end() && word.length() > 2) {
            words.push_back(word);
        }
    }
    
    std::cout << cleaned << std::endl;
    return words;
}

void Classifier::train_model()
{
    m_word_class_count.clear();
    m_total_words_in_class.clear();
    m_vocabulary.clear();
    m_total_spam_docs = 0;
    m_total_ham_docs = 0;

    for (const auto& doc : m_docs)
    {
        if (doc.type == "spam") {
            m_total_spam_docs++;
        } else if (doc.type == "ham") {
            m_total_ham_docs++;
        } else {
            throw std::invalid_argument("invalid document type: " + doc.type);
        }
        
        for (const auto& word : doc.words)
        {
            m_vocabulary.insert(word);
            m_total_words_in_class[doc.type]++;
            m_word_class_count[doc.type][word]++;
        }
    }
    
    std::cout << "\nTraining complete:" << std::endl;
    std::cout << "  Spam docs: " << m_total_spam_docs << std::endl;
    std::cout << "  Ham docs: " << m_total_ham_docs << std::endl;
    std::cout << "  Vocabulary size: " << m_vocabulary.size() << std::endl;
}

double Classifier::calc_log_word_prob(std::string word, std::string const &type) const
{
    size_t count = 0;
    auto class_it = m_word_class_count.find(type);
    if (class_it != m_word_class_count.end()) {
        auto word_it = class_it->second.find(word);
        if (word_it != class_it->second.end()) {
            count = word_it->second;
        }
    }
    
    size_t total_words = 0;
    auto total_it = m_total_words_in_class.find(type);
    if (total_it != m_total_words_in_class.end()) {
        total_words = total_it->second;
    }
    
    double numerator = static_cast<double>(count) + m_a;
    double denominator = static_cast<double>(total_words) + m_a * m_vocabulary.size();
    
    return std::log(numerator / denominator);
}

std::string Classifier::predict(std::vector<std::string> doc) const
{
    if (m_total_ham_docs + m_total_spam_docs == 0) {
        throw std::runtime_error("Model not trained yet!");
    }
    
    double ln_p_ham = std::log(static_cast<double>(m_total_ham_docs) / 
                               (m_total_ham_docs + m_total_spam_docs));
    double ln_p_spam = std::log(static_cast<double>(m_total_spam_docs) / 
                                (m_total_ham_docs + m_total_spam_docs));
    
    double score_ham = ln_p_ham;
    double score_spam = ln_p_spam;
    
    for (const auto& word : doc)
    {
        score_ham += calc_log_word_prob(word, "ham");
        score_spam += calc_log_word_prob(word, "spam");
    }

    return score_ham > score_spam ? "ham" : "spam";
}

Metrics Classifier::evaluate(std::string const &test_path) const
{
    std::ifstream file(test_path);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open test file: " + test_path);
    }
    
    std::vector<Document> test_docs;
    std::string line;
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string text;
        std::string label;
        
        std::getline(ss, text, ',');
        std::getline(ss, label, ',');
        
        std::vector<std::string> words = preprocess_text(text);
        
        if (!words.empty() && (label == "spam" || label == "ham")) {
            test_docs.emplace_back(words, label);
        }
    }
    file.close();
    
    std::cout << "\nLoaded " << test_docs.size() << " test documents" << std::endl;
    
    int TP = 0, TN = 0, FP = 0, FN = 0;
    
    std::cout << "\n--- Predictions on test set ---" << std::endl;
    
    for (size_t i = 0; i < test_docs.size(); ++i) {
        const auto& doc = test_docs[i];
        std::string prediction = predict(doc.words);
        
        if (doc.type == "spam" && prediction == "spam") {
            TP++;
            std::cout << "✓ [SPAM] correctly: ";
        } else if (doc.type == "ham" && prediction == "ham") {
            TN++;
            std::cout << "✓ [HAM]  correctly: ";
        } else if (doc.type == "ham" && prediction == "spam") {
            FP++;
            std::cout << "✗ [HAM]  FP error:  ";
        } else if (doc.type == "spam" && prediction == "ham") {
            FN++;
            std::cout << "✗ [SPAM] FN error:  ";
        }
        
        std::cout << "doc " << i+1 << ": ";
        for (size_t j = 0; j < std::min(size_t(3), doc.words.size()); ++j) {
            std::cout << doc.words[j] << " ";
        }
        std::cout << "... -> predicted: " << prediction;
        std::cout << " (actual: " << doc.type << ")" << std::endl;
    }
    
    std::cout << "\n=== Confusion Matrix ===" << std::endl;
    std::cout << "              Predicted" << std::endl;
    std::cout << "              Spam    Ham" << std::endl;
    std::cout << "Actual Spam   " << std::setw(6) << TP << "  " << std::setw(6) << FN << std::endl;
    std::cout << "       Ham    " << std::setw(6) << FP << "  " << std::setw(6) << TN << std::endl;
    
    Metrics m;
    int total = TP + TN + FP + FN;
    
    if (total > 0) {
        m.accuracy = static_cast<double>(TP + TN) / total;
    }
    
    if (TP + FP > 0) {
        m.precision = static_cast<double>(TP) / (TP + FP);
    }
    
    if (TP + FN > 0) {
        m.recall = static_cast<double>(TP) / (TP + FN);
    }
    
    if (m.precision + m.recall > 0) {
        m.f1_score = 2 * (m.precision * m.recall) / (m.precision + m.recall);
    }
    
    return m;
}