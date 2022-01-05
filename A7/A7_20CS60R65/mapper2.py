"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query2_mapper2.py
"""

import sys

file = open("network.txt")

for line in file:

    if line.strip() == '':
        continue
    else:
        node1, node2 = line.split('\t', 1)

        for i in range(300):
                print ('%d\t%d\t%d\t%d\t%d' % (int(node1), i, int(node2), 0, 1))
                print ('%d\t%d\t%d\t%d\t%d' % (i, int(node1), int(node2), 1, 1))
                print ('%d\t%d\t%d\t%d\t%d' % (int(node2), i, int(node1), 0, 1))
                print ('%d\t%d\t%d\t%d\t%d' % (i, int(node2), int(node1), 1, 1))
