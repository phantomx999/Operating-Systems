#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase02/clean.py
timeout 2s ./Vote_Counter TestCase02/Who_Won > TestCase02/vote_output.txt 2> TestCase02/error
./TestCase02/compare.py TestCase02/Who_Won/Who_Won.txt TestCase02/Who_Won/vote_solution.txt >> TestCase02/vote_output.txt 2>> TestCase02/error
ls TestCase02/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt >> TestCase02/vote_output.txt 2>> TestCase02/error
ls TestCase02/Who_Won/Region_1/Region_1.txt >> TestCase02/vote_output.txt 2>> TestCase02/error
ls TestCase02/Who_Won/Region_2/Region_2.txt >> TestCase02/vote_output.txt 2>> TestCase02/error
