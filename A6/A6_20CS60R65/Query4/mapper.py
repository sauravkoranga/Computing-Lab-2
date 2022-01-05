"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query4_mapper.py
"""

import sys

# input comes from STDIN / network.txt
for line in sys.stdin:

    if line.strip() == '':
    	continue
    else:
        input_line = line.split(",")

        node1 = int(input_line[0])
        node2 = int(input_line[1])

        if node1 in range(0, 1000):
        	print('%s\t%s' % (node1, node2))
        if node2 in range(0, 1000):
        	print('%s\t%s' % (node2, node1))
