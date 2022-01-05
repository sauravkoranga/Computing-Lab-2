"""
Name - Saurav koranga
Roll no. - 20CS60R65
Query1_mapper1.py
"""

import sys

file1 = open("final.txt")
file2 = open("triangles.txt")

l1 = []
l2 = []

for line in file1:

    i,j = line.split(",")
    l1.append(j)

for line in file2:

    i,j = line.split(",")
    l2.append(j)

count = 0
for i in range(300):
	if l1[i] != l2[i]:
		count += 1
		print(i)
print(count)