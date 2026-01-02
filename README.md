# Kodiak

**Kodiak** is a high-performance, in-memory, columnar analytics engine.  
It is designed for **vectorized execution**, **columnar memory layout**, and **fast aggregation and filtering operations**, making it ideal for benchmarking against Pandas, Polars, and other data processing libraries.

Kodiak provides the **core execution engine of an analytical database** without the complexity of SQL parsing, joins, or disk storage — a lightweight, educational engine for high-performance analytics.

---

## Key Features

- Columnar in-memory table storage  
- Vectorized scan, filter, and aggregate operations  
- Python API for programmatic table construction and queries  
- Designed for performance benchmarking against Pandas / Polars  
- Lightweight, minimal dependencies  

---

## Motivation

Many data analytics workflows rely on Python libraries like Pandas, which are convenient but hit performance limits on large datasets.  

Kodiak explores how **a columnar, vectorized execution engine** works under the hood, similar to the core of DuckDB or Apache Arrow-based systems, but in a small, educational, and extendable package.

By focusing on **memory layout** and **operator pipelines**, Kodiak demonstrates how modern analytical engines achieve high throughput for in-memory computations.

---

