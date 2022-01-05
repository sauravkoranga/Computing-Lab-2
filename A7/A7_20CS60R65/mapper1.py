"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query1_mapper1.py
"""

import sys

for line in sys.stdin:

    if line.strip() == '':
        continue
    else:
        input_line = line.split(",")

        node1 = int(input_line[0])
        node2 = int(input_line[1])

        if node1 > node2:
        	print ('%d\t%d\t%d' % (node2, node1, 1))
        elif node2 > node1:
        	print ('%d\t%d\t%d' % (node1, node2, 1))
        else:
        	pass	# self loop
