#include <algorithm>
#include <iostream>
#include <string>
#include <bitset>
#include <vector>
#include <cmath>
#include <map>
#include <set>
#include <stack>


std::vector<std::string> 
make_Euler_cycle(std::map<std::string, std::set<std::string>> graph)
{
    std::stack<std::string> verteces;
    std::vector<std::string> path;
    verteces.push(graph.begin()->first);
    std::string current = verteces.top();

    while(not verteces.empty())
    {
        current = verteces.top();
        if(not graph[current].empty())
        {
            verteces.push(*graph[current].begin());
            graph[current].erase(graph[current].begin());
            continue;
        } 
        path.push_back(current);
        verteces.pop();
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::map<std::string, std::set<std::string>> 
construct_DeBruijn_graph(const int k, const int n)
{
    auto calculate_overlap = [](const auto & a, const auto & b, int mer) {
        for (size_t i = 1; i < a.size(); ++i)  if(a[i] != b[i-1]) return false;
        return true;
    };

    std::vector<std::string> mers;
    std::map<std::string, std::set<std::string>> graph;

    for(size_t i = 0; i < n; ++i)
        mers.emplace_back(std::bitset<16>{i}.to_string().substr(16-k));

    for(auto & m1 : mers)
        for(auto & m2 : mers)
            if(calculate_overlap(m1, m2, k-1))
                graph[m1.substr(0, k-1)].emplace(m2.substr(0, k-1));

    return graph;
}

void print_kUniversal_Circular_String(const std::vector<std::string> & cycle, const int k)
{
    for(size_t i = 0; i < cycle.size()-1; ++i) {
        const auto &s = cycle[i];
        std::cout << s.substr(0, s.size()-(k-2));
    }
    std::cout << std::endl;
}

int main ()
{
    int k;
    std::cin >> k;
    int n = pow(2, k);

    const auto cycle = make_Euler_cycle(construct_DeBruijn_graph(k, n));
    print_kUniversal_Circular_String(cycle, k);

    return 0;
}