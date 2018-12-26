# python3

"""
Coursera/Genome Assembly Programming Challenge/Week 3/Problem 3:
Bubble Detection (in de Bruijn graph)
Grading: 'Good job! (Max time used: 1.06/100.00, max memory used: 31244288/536870912.)'
Author: Andrii Shostatskyi
Email: shostatskyi.andrii@gmail.com
Respect Coursera Honor Code
Copyright © 2018. All rights reserved
"""

import sys
import itertools

class BubbleDetection:

    # Bubble (in a de Bruijn graph) is defined as a pair of short 
    # non-overlapping disjoint paths between some vertces V and W.

    def __init__(self, k, t, reads):
        self.k = k
        self.threshold = t
        self.graph = {}
        self.paths = {}
        self.bubbles = 0

        self.multiple_incoming = lambda vertex: self.graph[vertex][1] > 1
        self.multiple_outgoing = lambda vertex: len(self.graph[vertex][0]) > 1

        self.build_deBruijn_graph(self.break_reads_into_kmers(reads))

    def break_reads_into_kmers(self, reads):
        break_read = lambda read: [ read[j:j + self.k] for j in range(len(read) - self.k + 1) ]
        return [ kmer for read in reads for kmer in break_read(read) ]

    def build_deBruijn_graph(self, kmers):

        def add_edge(graph, left, right):
            graph.setdefault(left, [set(), 0])
            graph.setdefault(right, [set(), 0])
            
            if right not in graph[left][0]:
                graph[left][0].add(right)
                graph[right][1] += 1

        for kmer in kmers:
            left, right = kmer[:-1], kmer[1:]
            if left != right:
                add_edge(self.graph, left, right)

    def count_bubbles(self):
        for k, v in self.graph.items():
            if self.multiple_outgoing(k):
                self.dfs(path=[k], start=k, current=k, depth=0)

        for _, candidates_list in self.paths.items():
            for pair in itertools.combinations(candidates_list, r=2):
                if self.paths_disjoint(pair):
                    self.bubbles += 1

        return self.bubbles

    def paths_disjoint(self, pair):
        return len(set(pair[0]) & set(pair[1])) == 2 # only V and W are shared

    def dfs(self, path, start, current, depth):
        if current != start and self.multiple_incoming(current):
            self.paths.setdefault((start, current), list()).append(path[:])

        if depth == self.threshold:
            return

        for next_ in self.graph[current][0]:
            if next_ not in path:
                path.append(next_)
                self.dfs(path, start, next_, depth + 1)
                path.remove(next_)


if __name__ == "__main__":
    data = sys.stdin.read().split()
    k, t, reads = data[0], data[1], data[2:]
    print(BubbleDetection(int(k), int(t), reads).count_bubbles())
