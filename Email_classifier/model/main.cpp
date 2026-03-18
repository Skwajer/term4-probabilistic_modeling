#include "classifier.hpp"
#include <iostream>
#include <exception>

int main()
{
    try {
        std::cout << "=== NAIVE BAYES SPAM CLASSIFIER ===" << std::endl;
        std::cout << "====================================" << std::endl;
        
        // Создаем классификатор с alpha = 1
        Classifier classifier(1);
        
        // 1. Загружаем обучающие данные
        std::cout << "\n[1] Loading training data..." << std::endl;
        classifier.load_csv("../data/train.csv");
        
        // 2. Обучаем модель
        std::cout << "\n[2] Training model..." << std::endl;
        classifier.train_model();
        
        // 3. Оцениваем на тестовых данных
        std::cout << "\n[3] Evaluating on test data..." << std::endl;
        Metrics metrics = classifier.evaluate("../data/test.csv");
        
        metrics.print();
        
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}