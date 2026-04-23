#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <queue>
#include <chrono>
#include <functional>

class RandomGraph 
{
private:
    std::mt19937 rng;
    std::uniform_int_distribution<int> weight_dist;
    std::uniform_real_distribution<double> prob_dist;
    double edge_probability;
    
public:
    RandomGraph() : rng(std::random_device{}()),
                    weight_dist(1, 10),
                    prob_dist(0.0, 1.0),
                    edge_probability(0.5) {}
    
    std::pair<std::vector<std::vector<bool>>, std::vector<std::vector<int>>> 
    generateGraph(int n, bool connected = false) 
    {
        std::vector<std::vector<bool>> adj(n, std::vector<bool>(n, false));
        std::vector<std::vector<int>> weights(n, std::vector<int>(n, 0));
        
        do 
        {
            for (int i = 0; i < n; i++) 
            {
                for (int j = i + 1; j < n; j++) 
                {
                    if (prob_dist(rng) < edge_probability) 
                    {
                        int w = weight_dist(rng);
                        adj[i][j] = adj[j][i] = true;
                        weights[i][j] = weights[j][i] = w;
                    } 
                    else 
                    {
                        adj[i][j] = adj[j][i] = false;
                        weights[i][j] = weights[j][i] = 0;
                    }
                }
            }
        } while (connected && !isConnected(adj, n));
        
        return {adj, weights};
    }
    
    bool isConnected(const std::vector<std::vector<bool>>& adj, int n) 
    {
        if (n == 0) return true;
        std::vector<bool> visited(n, false);
        std::queue<int> q;
        q.push(0);
        visited[0] = true;
        int count = 1;
        
        while (!q.empty()) 
        {
            int v = q.front();
            q.pop();
            for (int u = 0; u < n; u++) 
            {
                if (adj[v][u] && !visited[u]) 
                {
                    visited[u] = true;
                    q.push(u);
                    count++;
                }
            }
        }
        return count == n;
    }
    
    // a) 
    int mstWeightSum(const std::vector<std::vector<bool>>& adj, 
                     const std::vector<std::vector<int>>& weights, int n) 
                     {
        std::vector<std::tuple<int, int, int>> edges;
        for (int i = 0; i < n; i++) 
        {
            for (int j = i + 1; j < n; j++) 
            {
                if (adj[i][j]) 
                {
                    edges.push_back({weights[i][j], i, j});
                }
            }
        }
        
        std::sort(edges.begin(), edges.end());
        
        std::vector<int> parent(n);
        std::iota(parent.begin(), parent.end(), 0);
        
        std::function<int(int)> find = [&](int x) -> int 
        {
            if (parent[x] != x) parent[x] = find(parent[x]);
            return parent[x];
        };
        
        auto unite = [&](int x, int y) 
        {
            int rx = find(x), ry = find(y);
            if (rx != ry) 
            {
                parent[ry] = rx;
                return true;
            }
            return false;
        };
        
        int totalWeight = 0;
        int edgesUsed = 0;
        
        for (const auto& [w, u, v] : edges) 
        {
            if (unite(u, v)) 
            {
                totalWeight += w;
                edgesUsed++;
                if (edgesUsed == n - 1) break;
            }
        }
        
        return totalWeight;
    }
    
    // b)
    int longestCycleByEdges(const std::vector<std::vector<bool>>& adj, int n) 
    {
        int maxCycle = 0;
        
        for (int start = 0; start < n; start++) 
        {
            std::vector<int> path;
            std::vector<bool> visited(n, false);
            
            std::function<void(int, int)> dfs = [&](int v, int parent) 
            {
                visited[v] = true;
                path.push_back(v);
                
                for (int u = 0; u < n; u++) 
                {
                    if (adj[v][u]) 
                    {
                        if (!visited[u]) 
                        {
                            dfs(u, v);
                        } 
                        else if (u != parent) 
                        {
                            auto it = std::find(path.begin(), path.end(), u);
                            if (it != path.end()) 
                            {
                                int cycleLen = path.end() - it;
                                maxCycle = std::max(maxCycle, cycleLen);
                            }
                        }
                    }
                }
                
                path.pop_back();
                visited[v] = false;
            };
            
            dfs(start, -1);
        }
        
        return maxCycle;
    }
    
    // c)
    int edgesInMaxWeightCycle(const std::vector<std::vector<bool>>& adj, 
                               const std::vector<std::vector<int>>& weights, int n) 
    {
        int maxWeight = 0;
        int bestCycleEdges = 0;
        
        for (int start = 0; start < n; start++) 
        {
            std::vector<int> path;
            std::vector<bool> visited(n, false);
            std::vector<int> pathWeights;
            
            std::function<void(int, int, int)> dfs = [&](int v, int parent, int currentWeight) 
            {
                visited[v] = true;
                path.push_back(v);
                
                for (int u = 0; u < n; u++) 
                {
                    if (adj[v][u]) 
                    {
                        if (!visited[u]) 
                        {
                            dfs(u, v, currentWeight + weights[v][u]);
                        } 
                        else if (u != parent) 
                        {
                            auto it = std::find(path.begin(), path.end(), u);
                            if (it != path.end()) 
                            {
                                int cycleEdges = path.end() - it;
                                if (currentWeight > maxWeight) 
                                {
                                    maxWeight = currentWeight;
                                    bestCycleEdges = cycleEdges;
                                }
                            }
                        }
                    }
                }
                
                path.pop_back();
                visited[v] = false;
            };
            
            dfs(start, -1, 0);
        }
        
        return bestCycleEdges;
    }
    
    // d)
    int isolatedVertices(const std::vector<std::vector<bool>>& adj, int n) 
    {
        int count = 0;
        for (int i = 0; i < n; i++) 
        {
            bool isolated = true;
            for (int j = 0; j < n; j++) 
            {
                if (adj[i][j]) 
                {
                    isolated = false;
                    break;
                }
            }
            if (isolated) count++;
        }
        return count;
    }
    
    // f)
    int componentsCount(const std::vector<std::vector<bool>>& adj, int n) 
    {
        std::vector<bool> visited(n, false);
        int components = 0;
        
        for (int i = 0; i < n; i++) 
        {
            if (!visited[i]) 
            {
                components++;
                std::queue<int> q;
                q.push(i);
                visited[i] = true;
                
                while (!q.empty()) 
                {
                    int v = q.front();
                    q.pop();
                    for (int u = 0; u < n; u++) 
                    {
                        if (adj[v][u] && !visited[u]) 
                        {
                            visited[u] = true;
                            q.push(u);
                        }
                    }
                }
            }
        }
        
        return components;
    }
    
    // g)
    int completeComponentsCount(const std::vector<std::vector<bool>>& adj, int n) 
    {
        std::vector<bool> visited(n, false);
        int completeComponents = 0;
        
        for (int i = 0; i < n; i++) 
        {
            if (!visited[i]) 
            {
                std::vector<int> component;
                std::queue<int> q;
                q.push(i);
                visited[i] = true;
                
                while (!q.empty()) 
                {
                    int v = q.front();
                    q.pop();
                    component.push_back(v);
                    for (int u = 0; u < n; u++) 
                    {
                        if (adj[v][u] && !visited[u]) 
                        {
                            visited[u] = true;
                            q.push(u);
                        }
                    }
                }
                bool isComplete = true;
                int size = component.size();
                for (int a = 0; a < size && isComplete; a++) 
                {
                    for (int b = a + 1; b < size; b++) 
                    {
                        if (!adj[component[a]][component[b]]) 
                        {
                            isComplete = false;
                            break;
                        }
                    }
                }
                
                if (isComplete && size > 0) 
                {
                    completeComponents++;
                }
            }
        }
        
        return completeComponents;
    }
    
    // e)
    static int numberOfNonIsomorphicTrees(int n) 
    {
        const std::vector<int> trees = {0, 1, 1, 1, 2, 3, 6, 11, 23, 47, 106};
        if (n <= 10 && n >= 1) return trees[n];
        return -1;
    }
};

class Statistics 
{
private:
    std::vector<double> samples;
    
public:
    void addSample(double value) 
    {
        samples.push_back(value);
    }
    
    double mean() const 
    {
        if (samples.empty()) return 0.0;
        double sum = std::accumulate(samples.begin(), samples.end(), 0.0);
        return sum / samples.size();
    }
    
    double variance() const 
    {
        if (samples.size() < 2) return 0.0;
        double m = mean();
        double sum = 0.0;
        for (double x : samples) 
        {
            sum += (x - m) * (x - m);
        }
        return sum / (samples.size() - 1);
    }
    
    void clear() 
    {
        samples.clear();
    }
};

int main() {
    int n;
    int numIterations;
    
    std::cout << "Введите количество вершин n (1-10): ";
    std::cin >> n;
    
    if (n < 1 || n > 10) 
    {
        std::cout << "n должно быть от 1 до 10" << std::endl;
        return 1;
    }
    
    std::cout << "Введите количество итераций для моделирования: ";
    std::cin >> numIterations;
    
    RandomGraph rg;
    Statistics stats_a, stats_b, stats_c, stats_d, stats_f, stats_g;
    
    std::cout << "\nВыполняется моделирование...\n" << std::endl;
    
    for (int iter = 0; iter < numIterations; iter++) 
    {
        auto [adj, weights] = rg.generateGraph(n, false);
        
        // a) Связный граф для MST
        auto [adj_connected, weights_connected] = rg.generateGraph(n, true);
        int mstSum = rg.mstWeightSum(adj_connected, weights_connected, n);
        stats_a.addSample(mstSum);
        
        // b) Длина самого большого цикла
        int longestCycle = rg.longestCycleByEdges(adj, n);
        stats_b.addSample(longestCycle);
        
        // c) Количество рёбер в самом тяжёлом цикле
        int edgesInHeavyCycle = rg.edgesInMaxWeightCycle(adj, weights, n);
        stats_c.addSample(edgesInHeavyCycle);
        
        // d) Количество изолированных вершин
        int isolated = rg.isolatedVertices(adj, n);
        stats_d.addSample(isolated);
        
        // f) Количество компонент связности
        int components = rg.componentsCount(adj, n);
        stats_f.addSample(components);
        
        // g) Количество полных компонент
        int completeComponents = rg.completeComponentsCount(adj, n);
        stats_g.addSample(completeComponents);
        
        if ((iter + 1) % (numIterations / 10) == 0) 
        {
            std::cout << "Прогресс: " << (iter + 1) * 100 / numIterations << "%" << std::endl;
        }
    }
    
    std::cout << "\n========== РЕЗУЛЬТЫ ДЛЯ n = " << n << " ==========\n" << std::endl;
    
    std::cout << "a) Сумма весов рёбер MST (связный граф):" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_a.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_a.variance() << "\n" << std::endl;
    
    std::cout << "b) Длина самого большого цикла (по числу рёбер):" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_b.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_b.variance() << "\n" << std::endl;
    
    std::cout << "c) Количество рёбер самого тяжёлого цикла:" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_c.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_c.variance() << "\n" << std::endl;
    
    std::cout << "d) Количество изолированных вершин:" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_d.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_d.variance() << "\n" << std::endl;
    
    std::cout << "e) Количество неизоморфных деревьев:" << std::endl;

    int trees = RandomGraph::numberOfNonIsomorphicTrees(n);
    
    std::cout << "   Значение = " << trees << " (детерминированная величина)" << std::endl;
    std::cout << "   Математическое ожидание = " << trees << std::endl;
    std::cout << "   Дисперсия = 0\n" << std::endl;
    
    std::cout << "f) Количество компонент связности:" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_f.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_f.variance() << "\n" << std::endl;
    
    std::cout << "g) Количество полных компонент связности:" << std::endl;
    std::cout << "   Математическое ожидание ≈ " << stats_g.mean() << std::endl;
    std::cout << "   Дисперсия ≈ " << stats_g.variance() << std::endl;
    
    return 0;
}