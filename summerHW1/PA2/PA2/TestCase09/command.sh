#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase09/clean.py
timeout 2s ./Vote_Counter TestCase09/Who_Won > TestCase09/vote_output.txt 2> TestCase09/error
./TestCase09/compare.py TestCase09/Who_Won/Who_Won.txt TestCase09/Who_Won/vote_solution.txt >> TestCase09/vote_output.txt 2>> TestCase09/error
