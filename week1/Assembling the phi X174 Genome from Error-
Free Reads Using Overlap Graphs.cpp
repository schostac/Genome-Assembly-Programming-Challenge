#include <iostream>
#include <cstring>
#include <vector>

int calculate_overlap(const std::string & a, const std::string & b, const int mer) noexcept
{
    for (int i = 0, n = 1 + a.size()-mer; i < n; ++i)
        if(strncmp(b.c_str(), a.c_str() + i, a.size() - i) == 0) 
            return a.size() - i;
    return 0;
}

std::string assemble_genome(std::vector<std::string> reads, const int mer) noexcept
{    
    std::string genome;
    genome.reserve(1000);
    genome += reads.front();

    std::string first_read = reads.front(), cur_read = "";
    int cur_index = 0;

    while(reads.size() > 1)
    {
        cur_read = std::move(reads[cur_index]);
        reads.erase(reads.begin() + cur_index);
        
        int max_overlap = -1;

        for(int j = 0; j < reads.size(); ++j)
        {
            int overlap = calculate_overlap(cur_read, reads[j], mer);
            if(overlap > max_overlap) max_overlap = overlap, cur_index = j;
        }

        genome += reads[cur_index].substr(max_overlap);
    }

    genome.erase(0, calculate_overlap(reads[0], first_read, mer));
    return genome;
}

int main ()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::vector<std::string> reads;
    reads.reserve(1618);
    std::string s;

    while(std::cin >> s)
        if(reads.back() != s) reads.emplace_back(std::move(s));

    std::cout << assemble_genome(std::move(reads), 12) << std::endl;   

    return 0;
}