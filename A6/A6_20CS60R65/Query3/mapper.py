"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query3_mapper.py
"""
import sys

# Storing result of query2 in top_users
top_users = []
file = open("../Query2/result.txt")
for line in file:
	top_users.append(int(line))


# input comes from STDIN (standard input)
for line in sys.stdin:

    if line.strip() == '':
    	continue
    else:
        input_line = line.split(",")

        node1 = int(input_line[0])
        node2 = int(input_line[1])

        if node1 in top_users and node2 not in top_users:
        	print('%d\t%d' % (node2, 1))
        elif node1 not in top_users and node2 in top_users:
        	print('%d\t%d' % (node1, 1))
