# Tensor Library with Optimized Matrix Multiplication

This repository contains an implementation of a `Tensor` class, which provides support for multi-dimensional array operations. The library includes essential tensor manipulations like reshaping, transposing, elementwise operations, and an optimized matrix multiplication function. The `matmul` function utilizes block matrix multiplication, designed to improve cache efficiency and computational speed, particularly on large matrices.

## Overview

The `Tensor` class is designed to perform mathematical operations on n-dimensional arrays (tensors) with a focus on efficiency. This lightweight library is suitable for numerical and deep learning operations that require frequent matrix and tensor manipulations. The primary highlight of this implementation is the **optimized matrix multiplication** (through block multiplication), which enhances performance for large datasets by reducing cache misses and memory access latency.

## Features

- **Basic Tensor Operations**: Includes elementwise addition, subtraction, multiplication, exponentiation, and activation functions like ReLU.
- **Tensor Manipulations**: Supports reshaping, transposing, and initializing tensors with ones.
- **Advanced Matrix Multiplication (`matmul`)**: Implements a cache-friendly matrix multiplication with block-based optimization for both 2D and batched 3D matrices.

## Code Structure

- **`Tensor` Class**: Implements the core tensor functionalities.
- **Member Functions**:
  - **Construction and Initialization**: Constructs tensors with specified dimensions and initializes with provided values if needed.
  - **Indexing and Access**: Access elements based on multi-dimensional indexing.
  - **Operations**: Perform tensor arithmetic, reshaping, transposing, and elementwise transformations.
  - **Optimized `matmul` Function**: Efficiently performs matrix multiplication using block-based processing.

## Optimized Matrix Multiplication: `matmul`

### Purpose of Optimization

Matrix multiplication can be computationally expensive, especially for large matrices. Naive implementations often suffer from cache inefficiencies, where data frequently needs to be fetched from slower main memory due to cache misses. The `matmul` function in this library uses **block matrix multiplication** to optimize for cache usage, reducing memory bandwidth requirements and improving processing speed.

### How Block Matrix Multiplication Works

1. **Dividing Matrices into Blocks**: The matrices are divided into smaller blocks of size `BLOCK_SIZE` (32 by default). This size is chosen to fit well within the CPU cache, allowing faster data access.
2. **Processing Blocks Individually**: Instead of performing operations on the entire matrix, we perform calculations on these smaller blocks, ensuring that each block stays in the cache while it’s being processed.
3. **Accumulation**: Each block’s result is accumulated into the output matrix after completing the multiplication for that block, minimizing write operations to main memory.

### Implementation Details

- **2D Matrix Multiplication**: For standard 2D matrices, the function multiplies blocks of rows and columns, iterating over the matrices in block steps (`i`, `j`, `k`) and performing operations on submatrices (`ib`, `jb`, `kb`).
- **3D Batched Matrix Multiplication**: For 3D tensors representing batches of 2D matrices, the function iterates over each batch and performs block-based matrix multiplication independently within each batch.

### Code Example: `matmul`

Here's a code snippet to illustrate the optimized `matmul` function:

```cpp
Tensor matmul(Tensor x) {
    // Ensure matrix dimensions are compatible
    if (dims.size() == 2 && x.dims.size() == 2) {  // Standard 2D matrix multiplication
        // ... (block-based 2D matrix multiplication)
    } else if (dims.size() == 3 && x.dims.size() == 2) {  // Batched 3D matrix multiplication
        // ... (block-based batched 3D matrix multiplication)
    } else {
        throw std::runtime_error("Incompatible dimensions for matmul.");
    }
}
