# <img src="https://github.com/Dryd33n/Dryd33n/blob/main/logos/uvic.png" height="25"> CSC 226: Algorithms and Data Structures II
LaTeX files for assignments and exercises from UVic CSC 226, covering topics such as:
- **Hash Tables**: Experimental analysis of separate chaining vs. open addressing; hash function analysis.
- **Graph Algorithms**: Minimum spanning trees, shortest path algorithms (Dijkstra's, Floyd-Warshall-Roy, BFS), and graph transformations.
- **Algorithm Analysis**: Recurrence proofs, asymptotic complexity, and induction-based correctness proofs.
- **Data Structures**: Union-Find with adjacency matrix representation; min-heap implementations.
- **Dynamic Programming**: Optimal substructure design, recurrence formulation, and correctness arguments.
- **Network Flows**: Transshipment problems and flow networks.
- **Experimental Methodology**: Controlled benchmarking in C, statistical analysis, and visualization with Matplotlib.

---

## Assignments

### [Assignment 1](assignments/a1/a1.pdf)
**Separate Chaining vs. Linear Probing — Experimental Analysis**

A full experimental comparison of two hash table collision resolution strategies implemented in C. Both techniques were benchmarked across a range of load factors (0.5–0.95) and map sizes (2⁵–2²⁴), with each configuration run 10 times and the median selected to reduce noise. Operations were mixed at realistic proportions (50% insert, 40% search, 10% delete) using a fixed random seed for reproducibility. Timing was performed at nanosecond resolution using `clock_gettime(CLOCK_MONOTONIC)`. Results were visualized as 3D surface plots and 2D line plots using Python's Matplotlib.

**Key finding:** Linear probing consistently outperforms separate chaining across nearly all load factors and map sizes. The performance gap is attributed to memory locality — linear probing stores elements contiguously while separate chaining follows scattered heap-allocated linked list nodes. Separate chaining's performance degrades significantly at higher load factors due to its steeper time-vs-α slope.

#### Question Breakdown
- **Introduction & Methodology**: Experimental design rationale — language choice (C/gcc), hardware setup, timing method, domain of experimentation, operation mix, hashing function.
- **Results**: 3D surface plot comparing both techniques; 2D line comparisons across load factor and map size.
- **Conclusion**: Linear probing is faster in practice despite similar asymptotic bounds; cache locality identified as the primary driver.
- **Reflection**: Self-assessment of methodology, conclusions, scientific context, and presentation quality.

---

### [Assignment 2](assignments/a2/a2.pdf)
**k-Source Minimum Cost Path — Floyd-Warshall-Roy vs. k × Dijkstra's**

An experimental investigation into the conjecture that running the Floyd-Warshall-Roy (FWR) algorithm is faster than running Dijkstra's algorithm k times for the k-source minimum cost path problem. Implemented in C using `QueryPerformanceCounter` for high-resolution Windows timing. Tested across vertex counts (25–500 in steps of 25), graph densities (25%, 50%, 75%, 100%), and k values (1%–100% of n). Graphs were randomly generated with controlled edge densities. Dijkstra's was implemented with a min-heap and adjacency list; FWR used an adjacency matrix.

**Key finding:** FWR is faster in the vast majority of cases, even for small k. On average, FWR becomes faster once k exceeds ~23% of n, dropping to ~16% for larger graphs. The crossover point decreases as graph density increases. The advantage is attributed to simpler code (compiler-friendly triple loop), better cache behavior, and elimination of repeated heap initialization overhead in Dijkstra's.

#### Question Breakdown
- **Introduction & Methodology**: Problem framing, algorithm implementations, graph generation, timing approach, hardware setup, and fairness considerations (different graph representations for each algorithm).
- **Results**: 4 side-by-side 3D surface plots at different densities; crossover plot showing k/n threshold across vertex counts and densities.
- **Conclusion**: Conjecture substantiated; asymptotic complexity alone is insufficient — constant factors and memory access patterns dominate in practice.
- **Reflection**: Self-assessment on methodology, conclusion quality, scientific context, and presentation.

---

## Exercises

- **Exercise 2** — Recurrence proof for median-of-medians quicksort; shows T(n) ∈ Θ(n) by induction.
- **Exercise 3** — Hash function analysis; derives expected number of keys hashing to a given index as n/(m−1).
- **Exercise 4** — Union-Find via adjacency matrix; union in Θ(1), find (DFS) in Θ(n²) worst case.
- **Exercise 5** — Proof that the maximum weight edge in any cycle is excluded from the MSWT; by contradiction using edge swapping.
- **Exercise 6** — Dynamic programming recurrence for maximum value coin selection with no three consecutive coins chosen.
- **Exercise 7** — Minimum cost path on unit/weight-2 graphs; graph transformation + BFS in Θ(|V|+|E|).
- **Exercise 8** — Counterexample disproving that a minimum cost path tree rooted at some vertex is always an MWST.
- **Exercise 9** — Counterexample showing Floyd-Warshall-Roy does not discover all k-hop paths after k iterations.
- **Exercise 11** — Transshipment problem; supply/demand flow network construction and solution visualization.
