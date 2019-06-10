#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase04/clean.py
timeout 2s ./Vote_Counter TestCase04/Who_Won > TestCase04/vote_output.txt 2> TestCase04/error
./TestCase04/compare.py TestCase04/Who_Won/Who_Won.txt TestCase04/Who_Won/vote_solution.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
ls TestCase04/Who_Won/Region_1/Region_1.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
ls TestCase04/Who_Won/Region_2/Region_2.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
ls TestCase04/Who_Won/Region_3/Region_3.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
ls TestCase04/Who_Won/Region_4/Region_4.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
ls TestCase04/Who_Won/Region_5/Region_5.txt >> TestCase04/vote_output.txt 2>> TestCase04/error
