"""
Latin Square Generator with Non-Rotational Column Constraint

A Latin square of order n is an n×n grid where each cell contains
a number from 0 to n-1, such that each number appears exactly once
in each row and each column.

Additional constraint: No column is a rotation of any other column.

Key insight: Two columns are rotations of each other if one can be
obtained by cyclically shifting the other. For a column of length n,
there are at most n distinct rotations.

Strategy: Build Latin squares column-by-column, ensuring each new
column is not a rotation of any previous column.
"""

from typing import List, Set, Tuple
from itertools import permutations
from copy import deepcopy

MAX_SOLUTIONS = 1
MIN_SIZE      = 4

def get_all_rotations(column: Tuple[int, ...]) -> Set[Tuple[int, ...]]:
    """Generate all possible rotations of a column."""
    n = len(column)
    rotations = set()
    for shift in range(n):
        rotation = tuple(column[(i + shift) % n] for i in range(n))
        rotations.add(rotation)
    return rotations


def columns_are_rotational(col1: Tuple[int, ...], col2: Tuple[int, ...]) -> bool:
    """Check if two columns are rotations of each other."""
    return col2 in get_all_rotations(col1)


class LatinSquareGenerator:
    def __init__(self, size: int):
        if size < MIN_SIZE:
            raise ValueError("Size must be at least " + str(MIN_SIZE))
        self.size = size
        self.solutions: List[List[List[int]]] = []
        self.max_solutions = MAX_SOLUTIONS

    def solve_column_by_column(
        self,
        columns: List[Tuple[int, ...]],
        used_row_masks: List[int]
    ) -> bool:
        """
        Build the Latin square column by column.

        columns: list of completed columns (as tuples)
        used_row_masks: for each row, a bitmask of values used so far
        """
        if len(self.solutions) >= self.max_solutions:
            return True

        col_idx = len(columns)

        # All columns placed - verify and save
        if col_idx >= self.size:
            # Convert columns to grid
            grid = [[columns[c][r] for c in range(self.size)] for r in range(self.size)]
            self.solutions.append(deepcopy(grid))
            return True

        # Generate candidate columns for this position
        all_perms = permutations(range(self.size))

        for perm in all_perms:
            perm_tuple = tuple(perm)

            # Check rotation constraint against all previous columns
            is_rotation = False
            for prev_col in columns:
                if columns_are_rotational(perm_tuple, prev_col):
                    is_rotation = True
                    break
            if is_rotation:
                continue

            # Check that this column works with row constraints
            valid = True
            new_row_masks = used_row_masks.copy()
            for row in range(self.size):
                val = perm_tuple[row]
                mask_bit = 1 << val
                if new_row_masks[row] & mask_bit:
                    valid = False
                    break
                new_row_masks[row] |= mask_bit

            if not valid:
                continue

            # Recurse with this column added
            columns.append(perm_tuple)
            self.solve_column_by_column(columns, new_row_masks)
            columns.pop()

            if len(self.solutions) >= self.max_solutions:
                return True

        return len(self.solutions) > 0

    def generate(self) -> List[List[List[int]]]:
        """Generate Latin squares with non-rotational column constraint."""
        self.solutions = []
        self.solve_column_by_column([], [0] * self.size)
        return self.solutions


def print_latin_square(grid: List[List[int]], index: int) -> None:
    """Print a Latin square in a formatted way."""
    n = len(grid)
    print(f"\n{'='*60}")
    print(f"Latin Square #{index}")
    print(f"{'='*60}")

    # Print column headers
    print("    ", end="")
    for j in range(n):
        print(f"{j:3}", end=" ")
    print()

    # Print top border
    print("    " + "+" + "-" * (3 * n + 1) + "+")

    for i, row in enumerate(grid):
        print(f"{i:2}  |", end=" ")
        for val in row:
            print(f"{val:2}", end=" ")
        print("|")

    # Print bottom border
    print("    " + "+" + "-" * (3 * n + 1) + "+")


def verify_latin_square(grid: List[List[int]]) -> bool:
    """Verify that a grid is a valid Latin square with non-rotational columns."""
    n = len(grid)

    # Check rows
    for row in grid:
        if sorted(row) != list(range(n)):
            print(f"Invalid row: {row}")
            return False

    # Check columns
    columns = []
    for col in range(n):
        column = [grid[row][col] for row in range(n)]
        columns.append(tuple(column))
        if sorted(column) != list(range(n)):
            print(f"Invalid column {col}: {column}")
            return False

    # Check non-rotational constraint
    for i in range(n):
        rotations = get_all_rotations(columns[i])
        for j in range(i + 1, n):
            if columns[j] in rotations:
                print(f"Column {j} is a rotation of column {i}")
                return False

    return True


def main():
    """Main function to generate and print Latin squares."""
    print("=" * 60)
    print("Latin Square Generator")
    print("Constraint: No column is a rotation of any other column")
    print("=" * 60)

    # Get size from user
    while True:
        try:
            size = int(input(f"\nEnter size (>= {MIN_SIZE}): "))
            if size < MIN_SIZE:
                print("Size must be at least {MIN_SIZE}. Try again.")
            else:
                break
        except ValueError:
            print("Please enter a valid integer.")

    print(f"\nGenerating Latin squares of size {size}...")
    print("(This may take a while for larger sizes)")

    generator = LatinSquareGenerator(size)
    solutions = generator.generate()

    if solutions:
        print(f"\nFound {len(solutions)} Latin square(s) satisfying the constraints.")
        for i, solution in enumerate(solutions, 1):
            print_latin_square(solution, i)

            # Verification
            if verify_latin_square(solution):
                print(f"[OK] Square #{i} is valid!")
            else:
                print(f"[FAIL] Square #{i} FAILED verification!")
    else:
        print("\nNo solutions found.")

    print(f"\n{'='*60}")
    print("Done!")


if __name__ == "__main__":
    main()

# Enter size (>= 4): 10

# Generating Latin squares of size 10...
# (This may take a while for larger sizes)

# Found 1 Latin square(s) satisfying the constraints.

# ============================================================
# Latin Square #1
# ============================================================
      # 0   1   2   3   4   5   6   7   8   9
    # +-------------------------------+
 # 0  |  0  1  2  3  4  5  6  7  8  9 |
 # 1  |  1  0  3  2  5  4  7  6  9  8 |
 # 2  |  2  3  0  1  6  7  8  9  4  5 |
 # 3  |  3  2  1  0  7  6  9  8  5  4 |
 # 4  |  4  5  6  7  8  9  2  3  0  1 |
 # 5  |  5  4  7  6  9  8  3  2  1  0 |
 # 6  |  6  7  8  9  0  1  4  5  2  3 |
 # 7  |  7  6  9  8  1  0  5  4  3  2 |
 # 8  |  8  9  4  5  3  2  0  1  6  7 |
 # 9  |  9  8  5  4  2  3  1  0  7  6 |
    # +-------------------------------+
# [OK] Square #1 is valid!