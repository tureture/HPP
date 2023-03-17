import random
import sys
import numpy as np

input = np.array([[0 for x in range(16)] for y in range(16)]) 
# read file into list
with open('output.txt', 'r') as f:
    board = f.read().split()
    for i in range(16):
        for j in range(16):
            input[i][j] = int(board[i*16 + j])

print(input)
out = np.transpose(input)

filename = 'transpose_4_2.txt'

with open(filename, 'w') as f:
    for i in range(16):
        for j in range(16):
            f.write(str(out[i][j]) + " ")
        f.write('\n')
    

