#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>

constexpr int MAX_ERRORS = 2;

int calculate_overlap(const std::string & a, const std::string & b, const int mer) noexcept
{
    for (int i = 0, n = 1 + a.size()-mer; i < n; ++i) 
    {
        int errors = 0;
        for(int j = 0, s = a.size() - i; j < s && errors <= MAX_ERRORS; ++j)
            if(a[i+j] != b[j]) ++errors;

        if(errors <= MAX_ERRORS) return a.size() - i;
    }
    return 0;
}

char get_most_frq(const std::vector<char> chars)
{
    std::map<char, int> counts;
    
    for(auto c : chars) ++counts[c];
    
    std::pair<char, int> most_frq = *counts.begin();
    
    for(auto each : counts)
        if(each.second > most_frq.second)
            most_frq = each;
    
    return most_frq.first;
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

        std::vector<int> overlaps;
        std::vector<int> positions;

        for(int j = 0; j < reads.size(); ++j)
        {
            int overlap = calculate_overlap(cur_read, reads[j], mer);
            if(overlaps.empty() || overlap >= overlaps.back()) 
            {
                overlaps.push_back(overlap);
                positions.push_back(j);
                cur_index = j; 
            }
        }

        if(overlaps.size() > 3) 
        {
            char* suffix =  &genome[genome.size()-overlaps[overlaps.size()-4]];
            char* prefix1 = &reads[positions[positions.size()-4]][0];
            char* prefix2 = &reads[positions[positions.size()-3]][
                            (overlaps[overlaps.size()-3] - overlaps[overlaps.size()-4])];
            char* prefix3 = &reads[positions[positions.size()-2]][
                            (overlaps[overlaps.size()-2] - overlaps[overlaps.size()-4])];
            char* prefix4 = &reads[positions[positions.size()-1]][
                             (overlaps[overlaps.size()-1] - overlaps[overlaps.size()-4])];

            for(int i = 0, n = overlaps[overlaps.size()-4]; i < n; ++i, 
                        ++suffix, ++prefix1, ++prefix2, ++prefix3, ++prefix4)
            {
                if(*suffix == *prefix1 && *prefix1 == *prefix2 && 
                   *prefix2 == *prefix3 && *prefix3 == *prefix4)
                    continue;

                const char c = get_most_frq({*suffix, *prefix1, *prefix2, *prefix3, *prefix4});
                *suffix = *prefix1 = *prefix2 = *prefix3 = *prefix4 = c;
            }
        }
        
        genome += reads[cur_index].substr(overlaps.back());
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
            reads.emplace_back(std::move(s));

    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(reads.begin(), reads.end(), g);

    std::cout << assemble_genome(std::move(reads), 12) << std::endl;   

    return 0;
}