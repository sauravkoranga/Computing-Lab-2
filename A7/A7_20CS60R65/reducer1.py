"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query1_reducer1.py
"""

import sys

current_node1 = None
current_node2 = None
current_count = 0
node1 = None
node2 = None
count = 0

users = 0
# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parsing tab separated input
    node1, node2, count = line.split('\t', 2)

    # convert count to int
    try:
        count = int(count)
    except ValueError:
        # if count not a number
        continue

    if current_node1 == node1 and current_node2 == node2:
        current_count += count
    else:
        if current_node1:
        	if current_count >= 30:
        		print ('%d\t%d' % (int(current_node1), int(current_node2)))

        current_count = count
        current_node1 = node1
        current_node2 = node2

# for the last node
if current_node1 == node1 and current_node2 == node2:
    
    if current_count >= 30:
        print ('%d\t%d' % (int(current_node1), int(current_node2)))
