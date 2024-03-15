import numpy as np

# Generate random matrices with variable sizes
def generate_random_matrices(size_range=(10,1000)):
  """
  Generates two random matrices with sizes within a specified range.

  Args:
      size_range: A tuple representing the minimum and maximum size for each matrix dimension.

  Returns:
      A tuple containing two randomly generated NumPy arrays (matrix_a, matrix_b).
  """
  min_size, max_size = size_range
  rows_a, cols_a = np.random.randint(min_size, max_size + 1, size=2)
  cols_b = np.random.randint(min_size, max_size + 1)
  matrix_a = np.random.rand(rows_a, cols_a)
  matrix_b = np.random.rand(cols_a, cols_b)
  return matrix_a, matrix_b

def multiply_load():
  # Example usage with variable input
  matrix_a, matrix_b = generate_random_matrices()
  result_matrix = np.matmul(matrix_a, matrix_b)
  return result_matrix

print(multiply_load())