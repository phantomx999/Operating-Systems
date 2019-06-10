#!/usr/bin/python

import sys
import os

def check(list1, list2):
	for vote in list1:
		if not vote in list2:
			return False
	return True

if len(sys.argv) != 3:
	print("Usage: ./compare.py <result_1> <result_2>")
	sys.exit(0)

if not os.path.exists(sys.argv[1]) and not os.path.exists(sys.argv[2]):
	print(False)
	sys.exit(0)

f1 = open(sys.argv[1])
f2 = open(sys.argv[2])

f1_lines = f1.readlines()
f2_lines = f2.readlines()

compare = True

if not len(f1_lines) == len(f2_lines):
	compare = False
else:
	for i in range(0,len(f1_lines)):
		list1 = f1_lines[i].rstrip("\n\r").lower().split(",")
		list2 = f2_lines[i].rstrip("\n\r").lower().split(",")
		if not check(list1, list2):
			compare = False

print(compare)
