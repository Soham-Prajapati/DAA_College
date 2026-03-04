# Experiment 3: Matrix Multiplication Algorithms

## Objective
To compare the performance of three matrix multiplication algorithms:
- Standard Triple Loop
- Divide and Conquer
- Strassen's Algorithm

## Code Overview
- **ThreeLoopMultiply:** Standard $O(n^3)$ matrix multiplication.
- **DivideAndConquer:** Recursively splits matrices, $O(n^3)$ but with different recursion structure.
- **Strassen:** Uses 7 multiplications per recursion, $O(n^{2.81})$.
- Output is a CSV with timings for each method and matrix size.

## Detailed Explanation of Methods

### Standard Triple Loop

**How it works:**  
Uses three nested loops to multiply matrices directly. For each cell $C[i][j]$, it computes the sum of products $A[i][k] \times B[k][j]$ for all $k$.

**What it does:**  
Computes matrix multiplication in the most straightforward way.

**Time Complexity:**  
- $O(n^3)$

**Space Complexity:**  
- $O(n^2)$

---

### Divide and Conquer

**How it works:**  
Splits matrices into four submatrices and recursively multiplies and adds them. For small matrices, falls back to the triple loop.

**What it does:**  
Performs matrix multiplication recursively, but still requires 8 multiplications per recursion.

**Time Complexity:**  
- $O(n^3)$

**Space Complexity:**  
- More than triple loop due to recursion and temporary matrices.

---

### Strassen's Algorithm

**How it works:**  
Optimizes divide-and-conquer by reducing the number of multiplications from 8 to 7 per recursion, using clever combinations of sums and differences.

**What it does:**  
Multiplies matrices faster than standard methods for large $n$.

**Time Complexity:**  
- $O(n^{2.81})$

**Space Complexity:**  
- Higher than other methods due to extra temporary matrices.

---

### Which is better and why?

- **Triple Loop** is simple and efficient for small matrices.
- **Divide and Conquer** is mainly educational; not faster than triple loop but shows recursion.
- **Strassen** is faster for large matrices but uses more memory and can be numerically unstable.

**Summary:**  
- Use **Triple Loop** for small or simple cases.
- Use **Strassen** for very large matrices where speed is critical and memory is available.

## Viva Questions & Answers

### 1. What is the standard matrix multiplication algorithm?
**Answer:** It uses three nested loops to compute $C = A \times B$ in $O(n^3)$ time.

### 2. How does Divide and Conquer work for matrix multiplication?
**Answer:** It splits matrices into submatrices and recursively multiplies and adds them.

### 3. What is Strassen's Algorithm?
**Answer:** An optimized divide-and-conquer algorithm that reduces the number of multiplications from 8 to 7 per recursion, improving time complexity.

### 4. What are the time complexities?
- **ThreeLoop:** $O(n^3)$
- **Divide and Conquer:** $O(n^3)$
- **Strassen:** $O(n^{2.81})$

### 5. When is Strassen's algorithm preferred?
**Answer:** For large matrices, as it is faster asymptotically, but for small matrices, overhead may outweigh benefits.

### 6. What are the limitations of Strassen's algorithm?
**Answer:** Numerical instability and extra memory usage.

### 7. How is the running time measured?
**Answer:** By recording the clock time before and after multiplication and calculating the difference.

### 8. What is the output format?
**Answer:** CSV file with timings for each method and matrix size.

### 9. Why do we compare these algorithms?
**Answer:** To understand trade-offs in speed, memory, and implementation complexity.

### 10. What is the base case for recursion in these algorithms?
**Answer:** For small matrices (e.g., $n \leq 32$), the standard triple loop is used to avoid recursion overhead.

---

*For more questions, ask about any specific function or code section!*
