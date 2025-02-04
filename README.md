# Kakuro - Genetic Algorithm for Board Generation

This project uses a Genetic Algorithm to generate Kakuro boards.  
The solver used for validation: [Kakuro Puzzle Solver using CUDA & OpenMP](https://github.com/egeaktemur/Kakuro-Puzzle-Solver-using-CUDA-OpenMP).

## Installation

### Requirements
- **CMake**
- **OpenMP**
- **GaLib (Genetic Algorithm Library)**

### Setup
Ensure that the paths to the **GaLib library** (header folder and library) are correctly set in the `CMakeLists.txt` file if needed.  
The current setup assumes a parallel installation where GaLib was built separately using CMake.

## Structure

- The board is initialized with numbers between **0 and 9**.
    - **0** represents a black field.
    - Other numbers correspond to the values that will be placed in the white fields when solving the board.
- **Mutation, crossover, and initialization** are designed to always satisfy all constraints:
    - No duplicate numbers in a row/column sum group.
    - No isolated white fields.
- The **fitness function** ensures that only boards with a **unique solution** are considered valid.
- If a unique solution is found, the algorithm terminates at the **end of the epoch**.

## Results & Conclusion

While this method generates valid Kakuro boards, they **rarely have a unique solution**.  
The training process is **computationally expensive** due to the need to verify board uniqueness.

To improve efficiency, we incorporated **constraint checking** directly into genome generation.  
Despite this optimization, finding valid boards with a unique solution remains difficult.

### Alternative Approach

A **potentially better approach** would be to evolve the **sums in the black fields** instead of the numbers in the white fields:
1. **First**, generate a board structure (white and black fields) while ensuring constraints (e.g., number of consecutive white fields).
2. **Next**, randomly assign sums in the black fields based on the number of white fields.

This method increases the likelihood of generating sums that lead to fewer valid combinations in the white fields.  
As a result, the probability of quickly obtaining a unique solution is **significantly higher**.

---
