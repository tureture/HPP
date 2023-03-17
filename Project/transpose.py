import random
import sys
import numpy as np

input = np.array([[0 for x in range(16)] for y in range(16)])
num = "180" 
# read file into list
with open('ordered_input/input_' + num + '.txt', 'r') as f:
    board = f.read().split()
    for i in range(16):
        for j in range(16):
            input[i][j] = int(board[i*16 + j])

out = np.transpose(input)

filename = 'ordered_input/transpose_' + num + '.txt'

with open(filename, 'w') as f:
    for i in range(16):
        for j in range(16):
            f.write(str(out[i][j]) + " ")
        f.write('\n')
    

