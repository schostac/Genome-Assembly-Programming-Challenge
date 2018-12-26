# python3

"""
Coursera/Genome Assembly Programming Challenge/Week 3/Problem 4:
Tip Removal (in de Bruijn graph)
Grading: 'Good job! (Max time used: 0.24/100.00, max memory used: 21770240/536870912.)'
Author: Andrii Shostatskyi
Email: shostatskyi.andrii@gmail.com
Respect Coursera Honor Code
Copyright © 2018. All rights reserved
"""

import sys
import itertools

class TipRemoval:

    # Tips are error-prone ends of the reads that do not form a bubble but instead 
    # form a path starting in a vertex without incoming edges or ending in a vertex 
    # without outgoing edges in the de Bruijn graph.

    def __init__(self, k, reads):
        self.k = k
        self.threshold = self.k
        self.graph = {}
        self.paths = {}
        self.edges_removed = 0

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

    def remove_tips(self):
        for k, v in self.graph.items():
            find_and_remove = None

            if len(v[0]) == 1 and v[1] == 0:
                find_and_remove = self.find_and_remove_incoming 
            elif len(v[0]) > 1:
                find_and_remove = self.find_and_remove_outgoing 
            else : continue

            condition = True
            while condition:
                condition = False
                for edge in v[0]:
                    if find_and_remove(edge, 0):
                        v[0].remove(edge)
                        self.edges_removed += 1
                        condition = True
                        break

        return self.edges_removed

    def find_and_remove_outgoing(self, current, depth):
        if self.outgoing_num(current) > 1 or self.incoming_num(current) > 1:
            return False
        
        if depth == self.threshold:
            return False

        if self.outgoing_num(current) == 0:
            return True

        if self.find_and_remove_outgoing(next(iter(self.graph[current][0])), depth + 1):
            self.graph[current][0].pop()
            self.edges_removed += 1
            return True
        
        return False

    def find_and_remove_incoming(self, current, depth):
        if depth == self.threshold:
            return False

        if self.outgoing_num(current) == 0 or self.incoming_num(current) > 1:
            return True
        
        if self.find_and_remove_incoming(next(iter(self.graph[current][0])), depth + 1):
            self.graph[current][0].pop()
            self.edges_removed += 1
            return True
        
        return False

    def incoming_num(self, v):
        return self.graph[v][1]

    def outgoing_num(self, v):
        return len(self.graph[v][0])

if __name__ == "__main__":
    k, reads = 15, sys.stdin.read().split()
    print(TipRemoval(k, reads).remove_tips())
