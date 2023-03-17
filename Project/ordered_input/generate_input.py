import random
import sys

input = []  
# read file into list
with open('solution.txt', 'r') as f:
    board = f.read().split()
    for i in range(256):
        input.append(int(board[i]))


for i in range(0, 256, 4):
    for j in range(4):
        index = random.randint(0, 255)
        input[index] = 0
    # Write file

    filename = 'input_' + str(i) + '.txt'

    with open(filename, 'w') as f:
        for x in range(256):
            f.write(str(input[x]) + " ")






