#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase07/clean.py
timeout 2s ./Vote_Counter TestCase07/Who_Won > TestCase07/vote_output.txt 2> TestCase07/error
./TestCase07/compare.py TestCase07/Who_Won/Who_Won.txt TestCase07/Who_Won/vote_solution.txt >> TestCase07/vote_output.txt 2>> TestCase07/error
ls TestCase07/Who_Won/Region_1/Region_1.txt >> TestCase07/vote_output.txt 2>> TestCase07/error
ls TestCase07/Who_Won/Region_2/Region_2.txt >> TestCase07/vote_output.txt 2>> TestCase07/error
ls TestCase07/Who_Won/Region_3/Region_3.txt >> TestCase07/vote_output.txt 2>> TestCase07/error
