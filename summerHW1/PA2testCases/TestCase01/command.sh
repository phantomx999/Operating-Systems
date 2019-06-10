#!/bin/bash

./TestCase01/clean.py

# TESTING LEAF.
timeout 2s ./Leaf_Counter TestCase01/Who_Won/Region_1/County_1/Sub_County_1 > TestCase01/leaf_output1.txt 2>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt TestCase01/Who_Won/Region_1/County_1/Sub_County_1/solution.txt > TestCase01/leaf_output2.txt 2>>TestCase01/error
timeout 2s ./Leaf_Counter TestCase01/Who_Won/Region_1/County_1 > TestCase01/leaf_output3.txt 2>>TestCase01/error
./TestCase01/clean.py

# TESTING AGGREGATE.
timeout 2s ./Aggregate_Votes TestCase01/Who_Won/Region_1/County_1/Sub_County_1 > TestCase01/aggregate_output1.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt TestCase01/Who_Won/Region_1/County_1/Sub_County_1/solution.txt > TestCase01/aggregate_output2.txt 2>>TestCase01/error
./TestCase01/clean.py
timeout 2s ./Aggregate_Votes TestCase01/Who_Won/Region_1/County_1 >> TestCase01/aggregate_output1.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Region_1/County_1/County_1.txt TestCase01/Who_Won/Region_1/County_1/solution.txt >> TestCase01/aggregate_output2.txt 2>>TestCase01/error
./TestCase01/clean.py
timeout 2s ./Aggregate_Votes TestCase01/Who_Won >> TestCase01/aggregate_output1.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Who_Won.txt TestCase01/Who_Won/solution.txt >> TestCase01/aggregate_output2.txt 2>>TestCase01/error
ls TestCase01/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt > TestCase01/aggregate_output3.txt 2>>TestCase01/error
ls TestCase01/Who_Won/Region_2/Region_2.txt >> TestCase01/aggregate_output3.txt 2>>TestCase01/error
./TestCase01/clean.py

# TESTING VOTE_COUNTER.
timeout 2s ./Vote_Counter TestCase01/Who_Won/Region_1/County_1/Sub_County_1 > TestCase01/vote_output.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt TestCase01/Who_Won/Region_1/County_1/Sub_County_1/vote_solution.txt >> TestCase01/vote_output.txt 2>>TestCase01/error
./TestCase01/clean.py
timeout 2s ./Vote_Counter TestCase01/Who_Won/Region_1 >> TestCase01/vote_output.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Region_1/Region_1.txt TestCase01/Who_Won/Region_1/vote_solution.txt >> TestCase01/vote_output.txt 2>>TestCase01/error
./TestCase01/clean.py
timeout 2s ./Vote_Counter TestCase01/Who_Won >> TestCase01/vote_output.txt 2>>TestCase01/error
./TestCase01/compare.py TestCase01/Who_Won/Who_Won.txt TestCase01/Who_Won/vote_solution.txt >> TestCase01/vote_output.txt 2>>TestCase01/error
ls TestCase01/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt >> TestCase01/vote_output.txt 2>>TestCase01/error
ls TestCase01/Who_Won/Region_2/Region_2.txt >> TestCase01/vote_output.txt 2>>TestCase01/error
