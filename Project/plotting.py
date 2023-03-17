import matplotlib.pyplot as plt


# Data when changing the number of zeros in the input
sudoku_1 = [5.687930, 4.810051, 3.412784, 1.944575, 0.315888]
sudoku_serial_1 = [5.444937, 4.776954, 3.320723, 1.899772, 0.271611 ]
sudoku_openmp_1 = [5.461238, 4.750491, 3.280492, 1.938808, 0.268692]
num_zeroes = [176, 172, 168, 164, 160]

# plot data
fix, ax = plt.subplots()
ax.plot(num_zeroes, sudoku_1, label='Random parallel Sudoku')
ax.plot(num_zeroes, sudoku_serial_1, label='Serial Sudoku')
ax.plot(num_zeroes, sudoku_openmp_1, label='OpenMP Sudoku')

plt.title("Time to solve Sudokus with varying number of zeros")
ax.set_xlabel("Number of zeros")
ax.set_ylabel("Time (s)")
ax.legend()


plt.show()