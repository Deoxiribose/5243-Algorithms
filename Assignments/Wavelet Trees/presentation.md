# Wavelet Trees: A Succinct Data Structure for Strings

Wavelet trees are succinct data structures designed to answer rank, select, and access queries efficiently on sequences over large alphabets. They are widely used in compressed data structures and string processing.

---

# Why Use Wavelet Trees?

- Provide fast `access(i)`, `rank(c, i)`, and `select(c, j)` operations  
- Use `O(n log σ)` bits, where σ is alphabet size  
- Ideal for text indexing and compressed representation  
- Applied in bioinformatics, information retrieval, and data mining

---

# Core Concepts

- Recursive binary partitioning of the alphabet  
- Bitvectors at each node encode symbol positions  
- Tree depth is `log σ`, where σ is the alphabet size  
- Each character's path from root to leaf encodes its identity

---

# Construction Steps

1. Split the alphabet in half recursively  
2. Create bitvector per node:  
   - 0 → left half  
   - 1 → right half  
3. Recurse with filtered sequences until single-character nodes are reached

---

# Example: Building from "gattaca"

Input text: `"gattaca"`  
Alphabet: `{a, c, g, t}` → split into `{a, c}` and `{g, t}`

- Root bitvector: `1 0 1 1 0 0 0`  
- Left child ("ac"): `0 0 1 0`  
- Right child ("gt"): `1 0 1`

Each level filters characters and builds a new bitvector.

---

# Access Operation

`access(i)` retrieves the ith character by:

1. Following bitvector bits from root to leaf  
2. Using rank to calculate correct position at each level  
3. Repeating until a leaf node (character) is found

---

# Rank Operation

`rank(c, i)` finds how many times `c` appears up to position `i`:

1. Follow the character path down the tree  
2. At each node, apply rank to bitvector to update position  
3. Final value is the number of `c` occurrences in range

---

# Select Operation

`select(c, j)` returns position of the jth occurrence of `c`:

1. Start at the leaf for `c`  
2. Trace upward using select operations on bitvectors  
3. Reconstruct original position in the sequence

---

# Applications

- FM-index for compressed pattern matching  
- Range counting and quantile queries  
- Genome analysis in bioinformatics  
- Succinct data structures for search engines

---

# Advantages

- Supports multiple queries in logarithmic time  
- Uses significantly less space than uncompressed structures  
- Adaptable to various domains (text, numeric, spatial data)  
- Ideal for large-alphabet or repetitive data

---

# Limitations

- Complex to implement from scratch  
- Slower to construct than simpler structures  
- Overhead with small alphabets or short sequences  
- Bitvector efficiency directly impacts performance

---

# Summary

Wavelet trees balance space and functionality for string queries. They compress data while still supporting fast lookups, making them ideal for advanced text processing, bioinformatics, and succinct data applications.

---
# Wavelet Tree Q&A

## 1. Why use a wavelet tree?  
Isn't it just another binary tree? Not really.

**Answer:**  
Wavelet trees are used when you need to compress a sequence while still enabling fast queries like `rank`, `select`, and `access`. They store data in near-optimal space — about `n·H₀(S)` bits — while supporting operations in `O(log σ)` time, where σ is the alphabet size. This makes them ideal for high-dimensional, symbolic data (e.g., genomic sequences, columnar analytics). They’re different from suffix trees or BSTs — those deal with substrings or comparisons; wavelet trees focus on symbol positions and frequency.

---

## 2. Simple example with bitvector logic:  
Given:  
`S = [a, b, a, c, b, c, a]`  
Alphabet: `{a, b, c}`  
Let’s split as `{a}` vs `{b, c}`. What does the root node’s bitvector look like?

**Answer:**  
We label:
- `0` for `{a}`
- `1` for `{b, c}`

So scanning the sequence:
`[a, b, a, c, b, c, a]` → bitvector = `[0, 1, 0, 1, 1, 1, 0]`

This vector routes values either left or right in the tree structure.

---

## 3. How does `rank(i, c)` work?  
Let’s walk through: `rank(4, b)` in `S = [a, b, a, c, b, c, a]`

**Answer:**  
1. Start at the root. Since `b ∈ {b, c}`, we move right.
2. Count how many `1`s are in the root bitvector up to index 4 → `[0,1,0,1,1]` → 3.
3. We now look at the subtree that encodes `[b, c, c]`.
   In this subtree, suppose we split `{b}` vs `{c}` → `b = 0`, `c = 1`.
4. The new bitvector is `[0,1,1]`. Count how many `0`s up to index 3 → answer is 1.

So `rank(4, b) = 1`

---

## 4. Real-world use case: OLAP systems

**Answer:**  
In columnar databases (e.g., ClickHouse, DuckDB), wavelet trees can represent categorical columns (e.g., regions, event types) in a compressed format.  
They allow:
- Fast filtering: “How many events are ‘purchase’ between row 100,000 and 200,000?”
- Range counts: “How many users are age 25–30 in this block?”
- Minimal memory footprint: no need to decompress full columns.

These operations are enabled by `rank`, `select`, and range queries on wavelet trees — all while using space near entropy bounds.

---

## 5. Time and space complexity

**Answer:**
- `access(i)` = `O(log σ)`
- `rank(i, c)` = `O(log σ)`
- `select(k, c)` = `O(log σ)`
  
Space: roughly `n·H₀(S) + o(n log σ)` bits, which is optimal under entropy compression.  
The better the compression (i.e., more symbol repetition), the more space-efficient the structure becomes.

---

# References

- Navarro, G. – *Wavelet Trees for All*  
- Ferragina & Manzini – *FM-Index*  
- SDSL – Succinct Data Structures Library  
