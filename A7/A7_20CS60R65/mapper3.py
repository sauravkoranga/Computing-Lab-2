"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query3_mapper3.py
"""

import sys

file1 = open("network.txt")
file2 = open("friends.txt")

# for matrix A

for line in file1:

    if line.strip() == '':
        continue
    else:
        node1, node2 = line.split('\t', 1)

    for i in range(300):
    	print('%d\t%d\t%d\t%d\t%d' % (int(node1), i, int(node2), 0, 1))
    	print('%d\t%d\t%d\t%d\t%d' % (int(node2), i, int(node1), 0, 1))

# for matrix A^2

for line in file2:

    if line.strip() == '':
        continue
    else:
        node1, node2, mutual_friends = line.split(',', 2)

    for i in range(300):
    	print('%d\t%d\t%d\t%d\t%d' % (i, int(node2), int(node1), 1, int(mutual_friends)))