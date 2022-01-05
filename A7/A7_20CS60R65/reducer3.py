"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query3_reducer3.py
"""

import sys

current_i = -1
current_k = None
element = 0
current_element = 0
node_i = None
k = None
current_j = 0
M_flag = -1

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parsing tab separated input
    node_i, k, node_j, M, count = line.split('\t', 4)

    # convert count to int
    try:
        count = int(count)
    except ValueError:
        # if count not a number
        continue

    if current_i == node_i and current_k == k and current_j == node_j and M_flag != M:
        element += count*current_element
        current_j = 300
        M_flag = -1

    elif current_i == node_i and current_k == k:
        current_j = node_j
        current_element = count
        M_flag = M

    # current_i != node_i and current_k != k
    else:
        if current_i != -1 and int(current_i) == int(current_k):
        	print('%d,%d' % (int(current_i), int(element)/2))

        current_element = count
        current_i = node_i
        current_k = k
        current_j = node_j
        M_flag = M
        element = 0

# for the last node
if current_i == node_i and current_k == k and current_j == node_j and M_flag != M:
    element += count*current_element
    current_j = 300

if current_i != -1 and int(current_i) == int(current_k):
    print('%d,%d' % (int(current_i), int(element)/2))

