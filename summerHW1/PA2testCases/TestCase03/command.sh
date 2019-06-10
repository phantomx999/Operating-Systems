#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase03/clean.py
timeout 2s ./Vote_Counter TestCase03/Who_Won > TestCase03/vote_output.txt 2> TestCase03/error
./TestCase03/compare.py TestCase03/Who_Won/Who_Won.txt TestCase03/Who_Won/vote_solution.txt >> TestCase03/vote_output.txt 2>> TestCase03/error
ls TestCase03/Who_Won/Region_1/County_1/Sub_County_1/Sub_County_1.txt >> TestCase03/vote_output.txt 2>> TestCase03/error
ls TestCase03/Who_Won/Region_1/County_1/Sub_County_1/Sub_Sub_County_1/Sub_Sub_County_1.txt >> TestCase03/vote_output.txt 2>> TestCase03/error
ls TestCase03/Who_Won/Region_1/Region_1.txt >> TestCase03/vote_output.txt 2>> TestCase03/error
