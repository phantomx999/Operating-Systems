#!/bin/bash

# TESTING VOTE_COUNTER.
./ExtraCredit/clean.py
timeout 2s ./Vote_Counter ExtraCredit/Who_Won > ExtraCredit/vote_output.txt 2> ExtraCredit/error
./ExtraCredit/compare.py ExtraCredit/Who_Won/Who_Won.txt ExtraCredit/Who_Won/vote_solution.txt >> ExtraCredit/vote_output.txt 2>> ExtraCredit/error
