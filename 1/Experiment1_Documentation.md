# Experiment 1: Analysis of Mathematical Functions

## Objective
To analyze the growth of various mathematical functions commonly encountered in algorithm analysis.

## Code Overview
The code computes and outputs values for:
- $n$
- $(3/2)^n$
- $n^3$
- $(\log_2 n)^2$
- $\log_2(n!)$
- $2^{2n}$
- $\log(\log n)$
- $\log_2 n$
- $n \cdot 2^n$
- $\ln n$
- $e^n$
- $n!$

## Output
The output is a CSV file with computed values for $n = 1$ to $100$.

## Viva Questions & Answers

### 1. What is the purpose of this experiment?
**Answer:** To observe and compare the growth rates of different mathematical functions relevant to algorithm complexity.

### 2. Why do we analyze functions like $n$, $n^3$, $2^n$, $n!$?
**Answer:** These functions represent the time/space complexity of various algorithms. Understanding their growth helps in evaluating algorithm efficiency.

### 3. What is the significance of $\log_2(n!)$?
**Answer:** $\log_2(n!)$ approximates the number of comparisons in comparison-based sorting algorithms (e.g., Merge Sort, Heap Sort).

### 4. Why is $n!$ capped at 20 in the code?
**Answer:** Factorials grow very fast and exceed the range of standard integer types beyond $n=20$.

### 5. What is the difference between $\log n$ and $\log(\log n)$?
**Answer:** $\log n$ grows much faster than $\log(\log n)$; the latter is extremely slow-growing and appears in some advanced algorithms.

### 6. How does $2^{2n}$ compare to $2^n$?
**Answer:** $2^{2n} = (2^n)^2$, so it grows much faster than $2^n$.

### 7. Why do we use natural logarithm ($\ln n$) and base-2 logarithm ($\log_2 n$)?
**Answer:** $\log_2 n$ is used in computer science for binary operations; $\ln n$ is used in mathematical analysis.

### 8. What is the practical use of $(3/2)^n$?
**Answer:** It is an example of an exponential function with a base between 1 and 2, useful for analyzing certain randomized or approximation algorithms.

### 9. What is the output format?
**Answer:** A CSV file with each row representing the computed values for a specific $n$.

### 10. How can this experiment help in algorithm analysis?
**Answer:** By visualizing and comparing function growth, it helps in understanding which algorithms are feasible for large inputs.

---

*For more questions, ask about any specific function or code section!*
