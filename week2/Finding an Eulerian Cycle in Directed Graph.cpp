#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <stack>

bool check_degrees(const std::vector<int> &in, const std::vector<int> &out)
{
    assert(in.size() == out.size());
    for(size_t i = 0; i < in.size(); ++i)
        if(in[i] != out[i]) return false;
    return true;
}

std::vector<int> make_euler_cycle(std::vector<std::vector<int>> graph)
{
    std::stack<int> verteces;
    std::vector<int> path;
    verteces.push(0);
    int current = 0;

    while(not verteces.empty())
    {
        current = verteces.top();
        if(not graph[current].empty())
        {
            verteces.push(graph[current].back());
            graph[current].pop_back();
            continue;
        } 
        path.push_back(current);
        verteces.pop();
    }

    std::reverse(path.begin(), path.end());
    path.pop_back();
    return path;
}   

int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    int num_verteces, num_edges;

    std::cin >> num_verteces >> num_edges;

    std::vector<std::vector<int>> graph(num_verteces);
    
    std::vector<int> in(num_verteces), out(num_verteces);

    for(int i = 0; i < num_edges; ++i) 
    {
        int from, to;
        std::cin >> from >> to;
        graph[--from].push_back(--to);
        ++in[to];
        ++out[from];
    }

    if (not check_degrees(in, out))
    {
        std::cout << 0 << std::endl;
        return 0;
    }

    auto cycle = make_euler_cycle(std::move(graph));

    std::cout << 1 << '\n';
    for(auto v : cycle)
        std::cout << v+1 << ' ';
    std::cout << std::endl;

    return 0;
}