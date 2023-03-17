import requests
import re

base = 4
# The URL of the website
url = "http://sudoku-puzzles.merschat.com/"
diff = ['Easy', 'Medium', 'Hard', 'MENSA', 'Genius', 'Lex Luthor']
payload = {'base': str(base), 'diff': diff[2], 'type': 'Numbers', "print_type": "None"}

# Do a POST request to the website with the payload
r = requests.post(url, data=payload)

# Get the raw HTML content
source = r.content
# Use regex to find the numbers on the Sudoku board
vals = re.findall(r'"(c\d*r\d*)" value="(\d*)"', str(source))
ref = re.findall(r'"(x\d*y\d*)" value="(\d*)"', str(source))
board = [int(i) if i != '' else 0 for _, i in vals]
ref_board = [int(i) if i != '' else 0 for _, i in ref]

# print(ref_board)
# print(board)

# Write the board to a file
with open('sudoku scraper/board.txt', 'w') as f:
    for i in range(len(board)):
        f.write(str(board[i]))
        f.write(' ')

with open('sudoku scraper/ref.txt', 'w') as f:
    for i in range(len(ref_board)):
        f.write(str(ref_board[i]))
        f.write(' ')