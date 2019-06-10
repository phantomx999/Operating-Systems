#!/usr/bin/python

import os

data = os.walk("TestCase01/Who_Won")
for _ in data:
	root = _[0]
	dirs = _[1]
	files = _[2]
	for file in files:
		if file == "votes.txt":
			continue
		elif "solution" in file:
			continue
		elif "leaf_output" in file or "aggregate_output" in file or "vote_output" in file:
			continue
		elif file.endswith(".txt"):
			os.remove(os.path.join(root, file))
