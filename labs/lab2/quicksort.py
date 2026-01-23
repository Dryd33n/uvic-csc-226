# quicksort.py
#
# Basic implementation of Quicksort, using the
# first element of the array as the pivot.
#
# B. Bird - 2026-01-16

import sys, time, random

sys.setrecursionlimit(100000)


def quicksort(A):
    if len(A) == 0 or len(A) == 1:
        return A  # Already sorted
    L, E, G = [], [], []

    # --- Pivot Selection --- #
    p = A[0]

    for x in A:
        if x < p:
            L.append(x)
        elif x > p:
            G.append(x)
        else:  # x == p
            E.append(x)

    sorted_L = quicksort(L)
    sorted_G = quicksort(G)
    return sorted_L + E + sorted_G


if __name__ == '__main__':
    input_values = [int(s) for s in sys.stdin.read().split()]
    if len(input_values) <= 50:
        print('Input values:', ' '.join(str(v) for v in input_values))

    # Call quicksort on a copy of the input values array, to
    # ensure that the original array is intact for later verification
    sorted_values = quicksort(input_values.copy())

    # Check that the array actually got sorted (and no values 
    # are missing or erroneous)
    if sorted_values != sorted(input_values):
        print("Error: Sorted sequence is incorrect")
    else:
        print("Sorting successful")

    if len(input_values) <= 50:
        print('Sorted values:', ' '.join(str(v) for v in sorted_values))
