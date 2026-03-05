#include <iostream>
#include <vector>
#include <map>
#include <random>
#include <string>
#include <iomanip>
#include <memory>
#include <cmath>
#include <cstring>

class QualityModule {
private:
    int count;
    
public:
    QualityModule(int c = 0) : count(c) {
        if (count < 0) count = 0;
        if (count > 4) count = 4;
    }
    
    int getCount() const { return count; }
    
    double getSpeedMultiplier() const {
        return 1.0 + (count * 0.1);
    }
    
    void printProbabilities(int currentLevel) const {
        std::cout << "      Шансы (текущий уровень " << currentLevel << "):\n";
        double totalProb = 0.0;
        
        for (int k = 1; k <= count; ++k) {
            if (currentLevel + k <= 5) {
                double prob = 52.0 * pow(10.0, -2.0 - k);
                std::cout << "        -> уровень " << (currentLevel + k) 
                          << ": " << std::fixed << std::setprecision(4) 
                          << (prob * 100) << "%\n";
                totalProb += prob;
            }
        }
        
        std::cout << "        остаться на уровне " << currentLevel 
                  << ": " << ((1.0 - totalProb) * 100) << "%\n";
    }
    
    int getNewQuality(int currentLevel, std::mt19937& rng) const {
        if (count == 0) return currentLevel;
        
        double totalProb = 0.0;
        std::vector<int> targetLevels;
        std::vector<double> probabilities;
        
        for (int k = 1; k <= count; ++k) {
            if (currentLevel + k <= 5) {
                double prob = 52.0 * pow(10.0, -2.0 - k);
                targetLevels.push_back(currentLevel + k);
                probabilities.push_back(prob);
                totalProb += prob;
            }
        }
        
        std::uniform_real_distribution<> dist(0.0, 1.0);
        double r = dist(rng);
        double cumulative = 0.0;
        
        for (size_t i = 0; i < targetLevels.size(); ++i) {
            cumulative += probabilities[i];
            if (r <= cumulative) {
                return targetLevels[i];
            }
        }
        
        return currentLevel;
    }
};

class Item {
private:
    int id;
    int qualityLevel;
    double creationTime;
    bool isComponent;
    
public:
    Item(int i, int q, double t, bool comp = false) 
        : id(i), qualityLevel(q), creationTime(t), isComponent(comp) {}
    
    int getId() const { return id; }
    int getQualityLevel() const { return qualityLevel; }
    double getCreationTime() const { return creationTime; }
    bool isComponent_() const { return isComponent; }
    
    std::string getQualityName() const {
        switch(qualityLevel) {
            case 1: return "ОБЫЧНЫЙ (1)";
            case 2: return "НЕОБЫЧНЫЙ (2)";
            case 3: return "РЕДКИЙ (3)";
            case 4: return "ЭПИЧЕСКИЙ (4)";
            case 5: return "ЛЕГЕНДАРНЫЙ (5)";
            default: return "НЕИЗВЕСТНЫЙ";
        }
    }
    
    std::string getShortQualityName() const {
        switch(qualityLevel) {
            case 1: return "Обычн(1)";
            case 2: return "Необыч(2)";
            case 3: return "Редк(3)";
            case 4: return "Эпич(4)";
            case 5: return "Легенд(5)";
            default: return "???";
        }
    }
};

class Recipe {
private:
    std::vector<int> requiredAmounts;
    
public:
    Recipe(const std::vector<int>& amounts) : requiredAmounts(amounts) {}
    
    int getComponentCount() const { return requiredAmounts.size(); }
    int getRequiredAmount(int index) const { return requiredAmounts[index]; }
    const std::vector<int>& getAmounts() const { return requiredAmounts; }
    
    void print() const {
        std::cout << "РЕЦЕПТ:";
        for (size_t i = 0; i < requiredAmounts.size(); ++i) {
            std::cout << " " << requiredAmounts[i] << " шт комп." << (i+1);
        }
        std::cout << std::endl;
    }
};

class AssemblyMachine {
private:
    int id;
    QualityModule qualityModule;
    Recipe recipe;
    double baseTime;
    bool busy;
    double timeRemaining;
    std::vector<std::shared_ptr<Item>> inputComponents;
    int currentProductId;
    double startTime;
    
public:
    AssemblyMachine(int i, const Recipe& r, double t, const QualityModule& qm)
        : id(i), recipe(r), baseTime(t), qualityModule(qm), busy(false), 
          timeRemaining(0), currentProductId(-1), startTime(0) {}
    
    int getId() const { return id; }
    bool isBusy() const { return busy; }
    
    bool canStart(const std::vector<int>& availableBaseComponents) const {
        if (busy) return false;
        for (int i = 0; i < recipe.getComponentCount(); ++i) {
            if (availableBaseComponents[i] < recipe.getRequiredAmount(i)) {
                return false;
            }
        }
        return true;
    }
    
    void startAssembly(int productId, double currentTime) {
        busy = true;
        timeRemaining = baseTime * qualityModule.getSpeedMultiplier();
        currentProductId = productId;
        startTime = currentTime;
        
        std::cout << "  ⚙️ [Время " << std::fixed << std::setprecision(2) << currentTime 
                  << "] СБОРЩИК #" << id << " начал продукт #" << productId 
                  << " (займет " << timeRemaining << ")" << std::endl;
    }
    
    void setInputComponents(const std::vector<std::shared_ptr<Item>>& components) {
        inputComponents = components;
    }
    
    std::shared_ptr<Item> update(double currentTime, std::mt19937& rng) {
        if (!busy) return nullptr;
        
        timeRemaining -= 1.0;
        if (timeRemaining <= 0.001) {
            busy = false;
            
            int maxQuality = 1;
            std::cout << "    Компоненты для продукта #" << currentProductId << ": ";
            if (inputComponents.empty()) {
                std::cout << "только базовые (уровень 1)";
            } else {
                for (const auto& comp : inputComponents) {
                    std::cout << comp->getShortQualityName() << " ";
                    if (comp->getQualityLevel() > maxQuality) {
                        maxQuality = comp->getQualityLevel();
                    }
                }
            }
            std::cout << " (макс. качество компонентов: " << maxQuality << ")" << std::endl;
            
            qualityModule.printProbabilities(maxQuality);
            
            int finalQuality = qualityModule.getNewQuality(maxQuality, rng);
            
            auto product = std::make_shared<Item>(currentProductId, finalQuality, startTime, false);
            
            std::cout << "    ✅ РЕЗУЛЬТАТ СБОРКИ #" << currentProductId 
                      << ": " << product->getQualityName() << std::endl;
            
            inputComponents.clear();
            return product;
        }
        return nullptr;
    }
};

class RecyclingMachine {
private:
    int id;
    QualityModule qualityModule;
    Recipe recipe;
    double baseTime;
    bool busy;
    double timeRemaining;
    std::shared_ptr<Item> currentProduct;
    double startTime;
    
public:
    RecyclingMachine(int i, const Recipe& r, double t, const QualityModule& qm)
        : id(i), recipe(r), baseTime(t), qualityModule(qm), busy(false), 
          timeRemaining(0), startTime(0) {}
    
    int getId() const { return id; }
    bool isBusy() const { return busy; }
    
    bool canStart() const { return !busy; }
    
    void startRecycling(std::shared_ptr<Item> product, double currentTime) {
        busy = true;
        timeRemaining = baseTime * qualityModule.getSpeedMultiplier();
        currentProduct = product;
        startTime = currentTime;
        
        std::cout << "  ♻️ [Время " << std::fixed << std::setprecision(2) << currentTime 
                  << "] ПЕРЕРАБОТЧИК #" << id << " начал продукт #" << product->getId() 
                  << " (" << product->getQualityName() << ")" << std::endl;
    }
    
    std::vector<std::shared_ptr<Item>> update(double currentTime, std::mt19937& rng) {
        if (!busy) return {};
        
        timeRemaining -= 1.0;
        if (timeRemaining <= 0.001) {
            busy = false;
            
            int productQuality = currentProduct->getQualityLevel();
            
            std::cout << "    Переработка продукта #" << currentProduct->getId() 
                      << " (качество " << productQuality << ")" << std::endl;
            
            qualityModule.printProbabilities(productQuality);
            
            int componentQuality = qualityModule.getNewQuality(productQuality, rng);
            
            std::vector<std::shared_ptr<Item>> components;
            int baseId = currentProduct->getId() * 100;
            
            std::cout << "    Получены компоненты с качеством " << componentQuality << " ("
                      << Item(0, componentQuality, 0).getQualityName() << "):" << std::endl;
            
            for (int i = 0; i < recipe.getComponentCount(); ++i) {
                int required = recipe.getRequiredAmount(i);
                int recovered = static_cast<int>(std::ceil(required * 0.25));
                
                std::cout << "      Тип " << (i+1) << ": " << recovered << " шт";
                
                for (int j = 0; j < recovered; ++j) {
                    int compId = baseId + i*100 + j;
                    auto comp = std::make_shared<Item>(compId, componentQuality, currentTime, true);
                    components.push_back(comp);
                }
            }
            std::cout << std::endl;
            
            currentProduct.reset();
            return components;
        }
        return {};
    }
};

class FactorySimulation {
private:
    std::vector<AssemblyMachine> assemblyMachines;
    std::vector<RecyclingMachine> recyclingMachines;
    Recipe recipe;
    std::vector<double> supplyRates;
    std::vector<int> baseComponentsStock;
    std::vector<std::shared_ptr<Item>> highQualityComponents;  
    std::vector<std::shared_ptr<Item>> products;
    
    std::vector<double> allLevel1Times;
    std::vector<double> allLevel2Times;
    std::vector<double> allLevel3Times;
    std::vector<double> allLevel4Times;
    std::vector<double> allLevel5Times;
    
    int nextItemId;
    int legendaryTarget;
    double currentTime;
    std::mt19937 rng;
    int totalAssembled;
    int totalRecycled;
    
    void addBaseComponents() {
        std::cout << "📦 Поступление базовых компонентов (уровень 1):" << std::endl;
        for (size_t i = 0; i < supplyRates.size(); ++i) {
            double rate = supplyRates[i];
            double intPart;
            double fracPart = std::modf(rate, &intPart);
            
            int newCount = static_cast<int>(intPart);
            baseComponentsStock[i] += newCount;
            
            std::uniform_real_distribution<> dist(0.0, 1.0);
            if (dist(rng) < fracPart) {
                baseComponentsStock[i]++;
                newCount++;
            }
            
            if (newCount > 0) {
                std::cout << "  Компонент " << (i+1) << ": +" << newCount 
                          << " шт (всего базовых: " << baseComponentsStock[i] << ")" << std::endl;
            }
        }
    }
    
    bool canTakeBaseComponents(const std::vector<int>& needed) {
        for (size_t i = 0; i < needed.size(); ++i) {
            if (baseComponentsStock[i] < needed[i]) return false;
        }
        return true;
    }
    
    void takeBaseComponents(const std::vector<int>& needed) {
        for (size_t i = 0; i < needed.size(); ++i) {
            baseComponentsStock[i] -= needed[i];
        }
    }
    
    std::vector<std::shared_ptr<Item>> findHighQualityComponents(const std::vector<int>& needed) {
        std::vector<std::shared_ptr<Item>> result;
        std::vector<int> remaining = needed;
        
        for (auto it = highQualityComponents.begin(); it != highQualityComponents.end(); ) {
            bool used = false;
            for (size_t i = 0; i < remaining.size(); ++i) {
                if (remaining[i] > 0) {
                    result.push_back(*it);
                    remaining[i]--;
                    used = true;
                    it = highQualityComponents.erase(it);
                    break;
                }
            }
            if (!used) ++it;
        }
        
        return result;
    }
    
    void recordProduct(std::shared_ptr<Item> product) {
        int level = product->getQualityLevel();
        double time = product->getCreationTime();
        
        switch(level) {
            case 1: allLevel1Times.push_back(time); break;
            case 2: allLevel2Times.push_back(time); break;
            case 3: allLevel3Times.push_back(time); break;
            case 4: allLevel4Times.push_back(time); break;
            case 5: allLevel5Times.push_back(time); break;
        }
    }
    
public:
    FactorySimulation(const Recipe& r, const std::vector<double>& rates,
                      int assemblyCount, int recyclingCount,
                      double asmTime, double recTime,
                      const QualityModule& asmQuality, const QualityModule& recQuality)
        : recipe(r), supplyRates(rates), nextItemId(1), 
          legendaryTarget(25), currentTime(0), totalAssembled(0), totalRecycled(0) {
        
        baseComponentsStock.resize(r.getComponentCount(), 0);
        
        for (int i = 0; i < assemblyCount; ++i) {
            assemblyMachines.emplace_back(i, r, asmTime, asmQuality);
        }
        
        for (int i = 0; i < recyclingCount; ++i) {
            recyclingMachines.emplace_back(i, r, recTime, recQuality);
        }
        
        std::random_device rd;
        rng = std::mt19937(rd());
        
        std::cout << "\n🏭 ЗАПУСК ЗАВОДА 🏭\n";
        std::cout << "========================================\n";
        std::cout << "СБОРЩИКОВ: " << assemblyCount << "\n";
        std::cout << "ПЕРЕРАБОТЧИКОВ: " << recyclingCount << "\n";
        std::cout << "ВРЕМЯ СБОРКИ: " << asmTime << "\n";
        std::cout << "ВРЕМЯ ПЕРЕРАБОТКИ: " << recTime << "\n";
        std::cout << "МОДУЛИ КАЧЕСТВА (СБОРКА): " << asmQuality.getCount() << "\n";
        std::cout << "МОДУЛИ КАЧЕСТВА (ПЕРЕРАБОТКА): " << recQuality.getCount() << "\n";
        recipe.print();
        std::cout << "СКОРОСТИ ПОДАЧИ:";
        for (double r : rates) std::cout << " " << r;
        std::cout << "\nЦЕЛЬ: 25 ЛЕГЕНДАРНЫХ продуктов\n";
        std::cout << "========================================\n\n";
    }
    
    void run() {
        while (allLevel5Times.size() < legendaryTarget) {
            std::cout << "\n--- ВРЕМЯ " << std::fixed << std::setprecision(2) << currentTime 
                      << " (Легендарных: " << allLevel5Times.size() << "/25) ---\n";
            
            addBaseComponents();
            
            std::cout << "ЗАПАСЫ:\n";
            std::cout << "  Базовые компоненты (ур.1):";
            for (size_t i = 0; i < baseComponentsStock.size(); ++i) {
                std::cout << " [" << i+1 << "]:" << baseComponentsStock[i];
            }
            std::cout << "\n";
            
            std::cout << "  Компоненты высокого качества: " << highQualityComponents.size() << " шт\n";
            if (!highQualityComponents.empty()) {
                std::map<int, int> qualityCount;
                for (const auto& c : highQualityComponents) {
                    qualityCount[c->getQualityLevel()]++;
                }
                for (const auto& [q, cnt] : qualityCount) {
                    std::cout << "    Уровень " << q << ": " << cnt << " шт\n";
                }
            }
            
            std::cout << "  Готовые продукты на складе: " << products.size() << " шт\n";
            if (!products.empty()) {
                std::map<int, int> productQualityCount;
                for (const auto& p : products) {
                    productQualityCount[p->getQualityLevel()]++;
                }
                for (const auto& [q, cnt] : productQualityCount) {
                    std::cout << "    Уровень " << q << ": " << cnt << " шт\n";
                }
            }
            
            for (auto& machine : assemblyMachines) {
                if (!machine.isBusy()) {
                    std::vector<int> needed = recipe.getAmounts();
                    
                    if (canTakeBaseComponents(needed)) {
                        std::cout << "СБОРЩИК #" << machine.getId() << " свободен\n";
                        
                        auto highQuality = findHighQualityComponents(needed);
                        
                        if (!highQuality.empty()) {
                            std::cout << "  Использовано " << highQuality.size() 
                                      << " компонентов высокого качества" << std::endl;
                            machine.setInputComponents(highQuality);
                        }
                        
                        takeBaseComponents(needed);
                        
                        machine.startAssembly(nextItemId++, currentTime);
                        totalAssembled++;
                    }
                }
            }
            
            for (auto& machine : recyclingMachines) {
                if (!machine.isBusy() && !products.empty()) {
                    auto it = products.begin();
                    std::cout << "ПЕРЕРАБОТЧИК #" << machine.getId() << " свободен\n";
                    machine.startRecycling(*it, currentTime);
                    products.erase(it);
                    totalRecycled++;
                }
            }
            
            for (auto& machine : assemblyMachines) {
                auto product = machine.update(currentTime, rng);
                if (product) {
                    recordProduct(product);
                    
                    if (product->getQualityLevel() >= 3) {
                        products.push_back(product);
                        std::cout << "      Продукт #" << product->getId() 
                                  << " (уровень " << product->getQualityLevel() 
                                  << ") добавлен на склад\n";
                    } else {
                        std::cout << "      Продукт #" << product->getId() 
                                  << " (уровень " << product->getQualityLevel() 
                                  << ") не добавлен на склад (качество < 3)\n";
                    }
                }
            }
            
            for (auto& machine : recyclingMachines) {
                auto components = machine.update(currentTime, rng);
                for (auto& comp : components) {
                    highQualityComponents.push_back(comp);
                    std::cout << "      Компонент #" << comp->getId() 
                              << " (уровень " << comp->getQualityLevel() 
                              << ") добавлен в хранилище компонентов\n";
                }
            }
            
            currentTime += 1.0;
        }
        
        std::cout << "\n🎉 ЦЕЛЬ ДОСТИГНУТА! Произведено 25 ЛЕГЕНДАРНЫХ продуктов 🎉\n";
    }
    
    void printResults() const {
        std::cout << std::fixed << std::setprecision(2);
        
        std::cout << "\n📊 ИТОГОВЫЕ РЕЗУЛЬТАТЫ (ВСЕ СОЗДАННЫЕ ПРОДУКТЫ) 📊\n";
        std::cout << "========================================\n";
        std::cout << "ВРЕМЯ ЗАВЕРШЕНИЯ: " << currentTime << "\n";
        std::cout << "ВСЕГО СОБРАНО: " << totalAssembled << "\n";
        std::cout << "ВСЕГО ПЕРЕРАБОТАНО: " << totalRecycled << "\n\n";
        
        int totalProducts = allLevel1Times.size() + allLevel2Times.size() + 
                           allLevel3Times.size() + allLevel4Times.size() + allLevel5Times.size();
        
        std::cout << "⚪ ОБЫЧНЫЕ (уровень 1) - " << allLevel1Times.size() << " шт:\n";
        for (double t : allLevel1Times) std::cout << "  " << t << "\n";
        if (allLevel1Times.empty()) std::cout << "  нет\n";
        
        std::cout << "\n🟢 НЕОБЫЧНЫЕ (уровень 2) - " << allLevel2Times.size() << " шт:\n";
        for (double t : allLevel2Times) std::cout << "  " << t << "\n";
        if (allLevel2Times.empty()) std::cout << "  нет\n";
        
        std::cout << "\n🔵 РЕДКИЕ (уровень 3) - " << allLevel3Times.size() << " шт:\n";
        for (double t : allLevel3Times) std::cout << "  " << t << "\n";
        if (allLevel3Times.empty()) std::cout << "  нет\n";
        
        std::cout << "\n🟣 ЭПИЧЕСКИЕ (уровень 4) - " << allLevel4Times.size() << " шт:\n";
        for (double t : allLevel4Times) std::cout << "  " << t << "\n";
        if (allLevel4Times.empty()) std::cout << "  нет\n";
        
        std::cout << "\n🌟 ЛЕГЕНДАРНЫЕ (уровень 5) - " << allLevel5Times.size() << " шт:\n";
        for (double t : allLevel5Times) std::cout << "  " << t << "\n";
        
        std::cout << "========================================\n";
        
        if (totalProducts > 0) {
            std::cout << "\n📈 СТАТИСТИКА КАЧЕСТВА (из " << totalProducts << " продуктов):\n";
            std::cout << "  Обычные (1): " << allLevel1Times.size() << " (" 
                      << (allLevel1Times.size() * 100.0 / totalProducts) << "%)\n";
            std::cout << "  Необычные (2): " << allLevel2Times.size() << " (" 
                      << (allLevel2Times.size() * 100.0 / totalProducts) << "%)\n";
            std::cout << "  Редкие (3): " << allLevel3Times.size() << " (" 
                      << (allLevel3Times.size() * 100.0 / totalProducts) << "%)\n";
            std::cout << "  Эпические (4): " << allLevel4Times.size() << " (" 
                      << (allLevel4Times.size() * 100.0 / totalProducts) << "%)\n";
            std::cout << "  Легендарные (5): " << allLevel5Times.size() << " (" 
                      << (allLevel5Times.size() * 100.0 / totalProducts) << "%)\n";
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc < 8) {
        std::cerr << "ИСПОЛЬЗОВАНИЕ: " << argv[0] 
                  << " AM RM t_сб t_пр МодСб МодПр КолКомп [кол1 кол2 ...] [s1 s2 ...]\n\n";
        std::cerr << "ПРИМЕР для рецепта с 2 компонентами:\n";
        std::cerr << "  Первого нужно 3 шт, второго 2 шт\n";
        std::cerr << "  Скорости подачи: 5.0 и 3.0\n";
        std::cerr << "  2 сборщика, 1 переработчик\n";
        std::cerr << "  Время сборки 4.0, переработки 3.0\n";
        std::cerr << "  2 модуля в сборке, 1 в переработке\n\n";
        std::cerr << argv[0] << " 2 1 4.0 3.0 2 1 2 3 2 5.0 3.0\n";
        return 1;
    }
    
    try {
        int assemblyCount = std::stoi(argv[1]);
        int recyclingCount = std::stoi(argv[2]);
        double assemblyTime = std::stod(argv[3]);
        double recyclingTime = std::stod(argv[4]);
        int asmModules = std::stoi(argv[5]);
        int recModules = std::stoi(argv[6]);
        int recipeComponents = std::stoi(argv[7]);
        
        if (argc < 8 + recipeComponents * 2) {
            std::cerr << "ОШИБКА: нужно " << recipeComponents 
                      << " количеств компонентов и " << recipeComponents << " скоростей\n";
            return 1;
        }
        
        std::vector<int> recipeAmounts;
        for (int i = 0; i < recipeComponents; ++i) {
            recipeAmounts.push_back(std::stoi(argv[8 + i]));
        }
        
        std::vector<double> supplyRates;
        for (int i = 0; i < recipeComponents; ++i) {
            supplyRates.push_back(std::stod(argv[8 + recipeComponents + i]));
        }
        
        Recipe recipe(recipeAmounts);
        QualityModule asmQuality(asmModules);
        QualityModule recQuality(recModules);
        
        FactorySimulation sim(recipe, supplyRates, assemblyCount, recyclingCount,
                              assemblyTime, recyclingTime, asmQuality, recQuality);
        
        sim.run();
        sim.printResults();
        
    } catch (const std::exception& e) {
        std::cerr << "ОШИБКА: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}