#!/bin/bash

# TESTING VOTE_COUNTER.
./TestCase10/clean.py
timeout 2s ./Vote_Counter TestCase10/Who_Won > TestCase10/vote_output.txt 2> TestCase10/error
