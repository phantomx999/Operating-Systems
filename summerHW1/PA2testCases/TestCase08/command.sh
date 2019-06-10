#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase08/clean.py
timeout 2s ./Vote_Counter TestCase08/Who_Won > TestCase08/vote_output.txt 2> TestCase08/error
./TestCase08/compare.py TestCase08/Who_Won/Who_Won.txt TestCase08/Who_Won/vote_solution.txt >> TestCase08/vote_output.txt 2>> TestCase08/error
ls TestCase08/Who_Won/Region_1/Region_1.txt >> TestCase08/vote_output.txt 2>> TestCase08/error
ls TestCase08/Who_Won/Region_2/Region_2.txt >> TestCase08/vote_output.txt 2>> TestCase08/error
ls TestCase08/Who_Won/Region_3/Region_3.txt >> TestCase08/vote_output.txt 2>> TestCase08/error
