# python3

"""
Coursera/Genome Assembly Programming Challenge/Week 3/Problem 2:
Selecting the Optimal k-mer Size
Grading: 'Good job! (Max time used: 0.11/15.00, max memory used: 11403264/536870912.)'
Author: Andrii Shostatskyi
Email: shostatskyi.andrii@gmail.com
Respect Coursera Honor Code
Copyright © 2018. All rights reserved
"""

import sys
from enum import Enum


class Result(Enum):
    NoCycle = 1
    OneCycle = 2
    MultipleCycles = 3


class NoOptimalKFound(Exception):
    pass


def check_Euler_cycle(de_Bruijn_graph):
    for k, v in de_Bruijn_graph.items():
        if not v: 
            return Result.NoCycle
        if len(v) > 1: 
            return Result.MultipleCycles

    return Result.OneCycle


def break_reads_into_kmers(reads, k):
    break_read = lambda read: [ read[j:j + k] for j in range(len(read) - k + 1) ]
    return [ kmer for read in reads for kmer in break_read(read) ]


def build_de_Bruijn_graph(kmers):
    graph = {}
    for kmer in kmers:
        left, right = kmer[:-1], kmer[1:]
        if left != right:
            graph.setdefault(left, set()).add(right)
            graph.setdefault(right, set())

    return graph


def binary_search_optimal_k(reads, l, r):
    while r >= l:
        mid = int(l + (r - l) / 2)
        result = check_Euler_cycle(build_de_Bruijn_graph(break_reads_into_kmers(reads, mid)))

        if result == Result.NoCycle:
            r = mid - 1
            continue
        elif Result == Result.MultipleCycles:
            l = mid + 1
            continue
        else:
            return mid

    raise NoOptimalKFound()
 

if __name__ == "__main__":
    k, reads = 100, sys.stdin.read().split()
    print(binary_search_optimal_k(reads, 0, k))
