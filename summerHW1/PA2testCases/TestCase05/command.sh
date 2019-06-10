#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase05/clean.py
timeout 2s ./Vote_Counter TestCase05/Who_Won > TestCase05/vote_output.txt 2> TestCase05/error
./TestCase05/compare.py TestCase05/Who_Won/Who_Won.txt TestCase05/Who_Won/vote_solution.txt >> TestCase05/vote_output.txt 2>> TestCase05/error
ls TestCase05/Who_Won/Region_1/Region_1.txt >> TestCase05/vote_output.txt 2>> TestCase05/error
ls TestCase05/Who_Won/Region_2/Region_2.txt >> TestCase05/vote_output.txt 2>> TestCase05/error
