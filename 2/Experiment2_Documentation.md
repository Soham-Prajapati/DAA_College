# Experiment 2: Merge Sort vs Quick Sort (Best, Worst, Average Cases)

## Objective
To compare the performance of Merge Sort and Quick Sort under best, worst, and average case scenarios.

## Code Overview
- **Merge Sort** and **Quick Sort** are implemented.
- The code should be modified to measure and output timings for:
  - **Best Case**: Already sorted input (ascending)
  - **Worst Case**: Reverse sorted input (descending)
  - **Average Case**: Random input
- Output should be a CSV with columns: `n,merge_best,merge_worst,merge_avg,quick_best,quick_worst,quick_avg`

## Detailed Explanation of Methods

### Merge Sort

**How it works:**  
Merge Sort is a divide-and-conquer algorithm. It recursively splits the array into halves until each subarray has one element, then merges the subarrays in a sorted manner.

**Steps:**  
1. Divide the array into two halves.
2. Recursively sort each half.
3. Merge the two sorted halves into a single sorted array.

**What it does:**  
It sorts an array by repeatedly dividing and merging, ensuring the result is always sorted.

**Time Complexity:**  
- Best, Worst, Average: $O(n \log n)$

**Space Complexity:**  
- $O(n)$ (needs extra space for merging)

**Stability:**  
- Stable (preserves order of equal elements)

---

### Quick Sort

**How it works:**  
Quick Sort is also a divide-and-conquer algorithm. It selects a ‘pivot’ element, partitions the array so elements less than the pivot come before it and greater after, then recursively sorts the partitions.

**Steps:**  
1. Pick a pivot element.
2. Partition the array into two subarrays: less than and greater than the pivot.
3. Recursively sort the subarrays.

**What it does:**  
It sorts an array by partitioning and recursively sorting the partitions.

**Time Complexity:**  
- Best/Average: $O(n \log n)$  
- Worst: $O(n^2)$ (when the smallest/largest element is always chosen as pivot)

**Space Complexity:**  
- $O(\log n)$ (in-place, uses stack space for recursion)

**Stability:**  
- Not stable (order of equal elements may change)

---

### Which is better and why?

- **Merge Sort** is better for linked lists and when stability is required. Its time complexity is always $O(n \log n)$, making it predictable.
- **Quick Sort** is usually faster for arrays in practice due to better cache performance and low constant factors, but its worst-case is $O(n^2)$.
- For large datasets and when memory is a concern, Quick Sort is preferred for arrays. For guaranteed performance and stability, Merge Sort is better.

---

**Summary:**  
- Use **Merge Sort** for stability and linked lists.
- Use **Quick Sort** for arrays and when average-case speed is more important than worst-case guarantees.

## Viva Questions & Answers

### 1. What is the difference between Merge Sort and Quick Sort?
**Answer:** Merge Sort is a stable, divide-and-conquer algorithm with $O(n \log n)$ worst-case time. Quick Sort is usually faster on average but has $O(n^2)$ worst-case time.

### 2. What are the best, worst, and average cases for Merge Sort?
**Answer:** All cases are $O(n \log n)$ for Merge Sort, as its divide and merge steps are always the same.

### 3. What are the best, worst, and average cases for Quick Sort?
**Answer:**
- **Best/Average:** $O(n \log n)$ (when pivots split the array evenly)
- **Worst:** $O(n^2)$ (when pivots are always the smallest/largest element)

### 4. How do you generate best, worst, and average case inputs?
**Answer:**
- **Best:** Sorted array
- **Worst:** Reverse sorted array (for Quick Sort)
- **Average:** Random array

### 5. Why is Merge Sort preferred for linked lists?
**Answer:** It does not require random access and is stable.

### 6. Is Quick Sort stable?
**Answer:** No, standard Quick Sort is not stable.

### 7. Which algorithm is in-place?
**Answer:** Quick Sort is in-place; Merge Sort requires extra space.

### 8. How do you measure the running time?
**Answer:** By recording the clock time before and after sorting and calculating the difference.

### 9. What is the output format?
**Answer:** CSV file with timings for each case and method.

### 10. Which algorithm is better in practice?
**Answer:** Quick Sort is usually faster for arrays due to better cache performance, but Merge Sort is more predictable.

---

*For more questions, ask about any specific function or code section!*
