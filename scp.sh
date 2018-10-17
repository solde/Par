#!/bin/bash

scp par2103@boada.ac.upc.edu:lab2/lab2_c/lab2/*.txt ./
mv speedup.txt lab2/speedup_row/speedup800.txt & mv elapsed.txt lab2/speedup_row/elapsed800.txt