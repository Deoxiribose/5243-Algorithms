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

# References

- Navarro, G. – *Wavelet Trees for All*  
- Ferragina & Manzini – *FM-Index*  
- SDSL – Succinct Data Structures Library  
