# Kodiak

**Kodiak** is a fast, in-memory analytics engine built for working with columnar data in Python.

It focuses on vectorized execution and efficient aggregation and filtering, making it a great tool for learning about analytical databases and benchmarking performance against libraries like Pandas and Polars.

Kodiak provides the core of an analytical database but without SQL, joins, or disk storage.

---

## Key Features

- Columnar, in-memory data storage
- Fast vectorized scans, filters, and aggregates
- Simple Python API for building and querying tables
- Designed for performance experiments and education
- Minimal dependencies and easy to run

---

## Motivation

Many data analytics workflows rely on Python libraries like Pandas, which are convenient but hit performance limits on large datasets.  

Kodiak explores how **a columnar, vectorized execution engine** works under the hood, similar to the core of DuckDB or Apache Arrow-based systems, but in a small, educational, and extendable package.

By focusing on **memory layout** and **operator pipelines**, Kodiak demonstrates how modern analytical engines achieve high throughput for in-memory computations.

---

