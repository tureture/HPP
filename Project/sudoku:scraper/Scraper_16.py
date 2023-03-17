import requests
import re
from bs4 import BeautifulSoup

# The URL of the website
url = "https://www.sudoku-puzzles-online.com/cgi-bin/hexadoku/print-1-grid-hexadoku.cgi"
payload = {'niv': 3}

valid_chars = '0123456789ABCDEF'
# Do a POST request to the website with the payload
r = requests.post(url, data=payload)

# Use BeautifulSoup to parse the HTML and find the table with the id "grid"
soup = BeautifulSoup(r.text, 'html.parser')
table = soup.find('table', id='grid')

table_rows = table.find_all('tr') # Find all the rows in the table
table_data = [] # Create an empty list to store the data

# Loop through the rows and find all the data in each row
for tr in table_rows:
    td = tr.find_all('td')
    row = [i.text for i in td]
    table_data.append(row)

# Create an empty list to store the board
board = []


for i in range(len(table_data)):
    for j in range(len(table_data[i])):
        if table_data[i][j] in valid_chars:
            # Convert the hex to decimal
            board.append(int(table_data[i][j], 16)+1)
        else:
            board.append(0)

# Print the board
for i in range(len(board)):
    print(board[i], end=' ')

# Write the board to a file
with open('board.txt', 'w') as f:
    for i in range(len(board)):
        f.write(str(board[i]))
        f.write(' ')

