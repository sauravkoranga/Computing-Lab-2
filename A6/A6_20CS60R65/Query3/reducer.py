"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query3_reducer.py
"""

from operator import itemgetter
import sys

current_node = None
current_count = 0
node = None

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parse the input we got from mapper.py
    node, count = line.split('\t', 1)

    # convert count (currently a string) to int
    try:
        count = int(count)
    except ValueError:
        # if count not a number
        continue

    # key (here: node) before it is passed to the reducer
    if current_node == node:
        current_count += count
    else:
        if current_node:
            # write result to STDOUT
            print (current_node)
        current_count = count
        current_node = node

# for last node
if current_node == node:
    print(current_node)