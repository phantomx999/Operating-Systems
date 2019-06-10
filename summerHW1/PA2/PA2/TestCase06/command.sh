#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase06/clean.py
timeout 2s ./Vote_Counter TestCase06/Who_Won > TestCase06/vote_output.txt 2> TestCase06/error
./TestCase06/compare.py TestCase06/Who_Won/Who_Won.txt TestCase06/Who_Won/vote_solution.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_1/Region_1.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_2/Region_2.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_3/Region_3.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_1/County_1/Sub_County_2/Sub_County_2.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
ls TestCase06/Who_Won/Region_1/County_3/County_3.txt >> TestCase06/vote_output.txt 2>> TestCase06/error
