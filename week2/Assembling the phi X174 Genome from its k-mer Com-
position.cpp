#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>

std::vector<std::string> 
make_Euler_cycle(std::map<std::string, std::vector<std::string>> graph)
{
    std::stack<std::string> verteces;
    std::vector<std::string> path;
    path.reserve(graph.size());
    verteces.push(graph.begin()->first);
    std::string current = verteces.top();

    while(not verteces.empty())
    {
        current = verteces.top();
        if(not graph[current].empty())
        {
            verteces.push(std::move(graph[current].back()));
            graph[current].pop_back();
            continue;
        } 
        path.emplace_back(std::move(current));
        verteces.pop();
    }

    std::reverse(path.begin(), path.end());
    return path;
}

std::map<std::string, std::vector<std::string>> 
construct_DeBruijn_graph()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string s;
    std::map<std::string, std::vector<std::string>> graph;

    while(std::cin >> s) 
        graph[s.substr(0, s.size()-1)].emplace_back(s.substr(1));

    return graph;
}

void print_genome(const std::vector<std::string> & cycle)
{
    std::cout << cycle.front();
    for(size_t i = 1; i < cycle.size()-9; ++i)
        std::cout << cycle[i].back();
    std::cout << std::endl;
}

int main ()
{
    print_genome(make_Euler_cycle(construct_DeBruijn_graph()));

    return 0;
}