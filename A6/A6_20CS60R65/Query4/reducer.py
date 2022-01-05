"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query4_mapper.py
"""

import operator
import sys

current_node = None
current_count = 0
us_count = 0
node1 = None

# at the end this dict will hold the nodes of top users(most friends) in facebook subgraph
top_users = {}

for i in range(0, 10):
    top_users[i+1000] = [0, 0];

# input comes from STDIN
for line in sys.stdin:
    # remove leading and trailing whitespace
    line = line.strip()

    # parse the input we got from mapper.py
    node1, node2 = line.split('\t', 1)
    node1 = int(node1)
    node2 = int(node2)

    top_users_sorted = {}

    # by key (here: node) before it is passed to the reducer

    if current_node == node1:
        current_count += 1
        if node2 <= 999:
            us_count += 1
    else:
        if current_node:
            for user in sorted(top_users.items(), key = lambda kv:(kv[1], kv[0])):
                top_users_sorted[user[0]] = top_users[user[0]]

            #print(top_users_sorted)
            #print((current_node , current_count))
            # Getting first key in dictionary
            f_key = next(iter(top_users_sorted))

            if us_count > int(top_users[f_key][0]):
                # changing keys of dictionary
                #print("del - "+str(f_key))
                del top_users[f_key]
                top_users[int(current_node)] = [us_count, current_count]
                #print("add - "+str(current_node))
                #print(top_users)

        current_count = 1
        current_node = node1
        if node2 <= 999:
            us_count = 1
        else:
            us_count = 0

top_users_sorted = {}

# do not forget to output the last node if needed!
if current_node == node1:
    #print ('%s\t%s' % (current_node, current_count))
    for user in sorted(top_users.items(), key = lambda kv:(kv[1], kv[0])):
        top_users_sorted[user[0]] = top_users[user[0]]

    # Getting first key in dictionary
    f_key = next(iter(top_users_sorted))

    if us_count > int(top_users[f_key][0]):
        # changing keys of dictionary
        del top_users[f_key]
        top_users[int(current_node)] = [us_count, current_count]

top_users_desc = dict(sorted(top_users.items(), key=operator.itemgetter(1), reverse=True))

cost = 0
for user in top_users_desc.keys():
    print("Node: " + str(user) + "\tFriends in US: " + str(top_users_desc[user][0]) + "\tOverall Friends: " + str(top_users_desc[user][1]))
    cost += top_users_desc[user][1]*10

print("cost: " + str(cost))