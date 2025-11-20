# Sudoku Generator & Solver (with PDF Export)

A complete C-based project that **generates Sudoku puzzles**, **solves them**, **stores multiple problems in text files**, and **exports generated puzzles to PDF** using **libharu**.

## 🚀 Project Overview

This project implements a **Sudoku puzzle generator**, a **backtracking-based solver**, and a **PDF creator** capable of exporting multiple puzzles. It is designed as a structured academic PBL (Project-Based Learning) submission.

The application can:

* Generate valid 9×9 Sudoku puzzles
* Solve any valid Sudoku input
* Store **multiple puzzles** in text files
* Create **PDF documents** containing puzzles using **libharu (C PDF library)**

---

## 🎯 Features

* ✔️ Random Sudoku puzzle generation using backtracking
* ✔️ Sudoku solver for any valid grid
* ✔️ Write multiple puzzles to files (e.g., `problems.txt`)
* ✔️ Create PDF files containing puzzles and formatted grids
* ✔️ Easy compilation & cross‑platform support

---

## 🛠️ Tech Stack

* **Environment:** MSYS2 (Windows)
* **Language:** C
* **Compiler:** GCC (MSYS2 MinGW)
* **PDF Library:** libharu (hpdf)
* **OS:** Windows (MSYS2 terminal)
* **Language:** C
* **Compiler:** GCC
* **PDF Library:** libharu (hpdf)
* **OS:** Works on Windows & Linux

---

## 📂 Directory Structure

```
├── src/
│   ├── main.c            # Full program (generation + solving + PDF export)
│   ├── solvedChecker.c   # Validity checker to verify if a user solved the puzzle correctly
│   
│
├── doc/
│   ├── problems.txt
│   ├── solution.txt
│   └── sudoku.pdf
│
├── README.md
└── .gitignore
```
---

## ⚙️ Getting Started

### **1. Install Prerequisites**

You need:

* GCC compiler(MSYS2)
* libharu development files

1. Open **MSYS2 UCRT64** terminal
2. Install GCC and libharu if needed:

```bash
pacman -S mingw-w64-ucrt-x86_64-gcc
pacman -S mingw-w64-ucrt-x86_64-toolchain
$ pacman -S mingw-w64-ucrt-x86_64-libharu
```

---

### **2. Clone the Repository**

```bash
git clone https://github.com/Janmejai-Pandey/SDF-PBL.git
cd SDF-PBL
```

---

### **3. Compile:

````bash
gcc src/main.c -lhpdf -lz -lpng -o build/sudoku
```
---

### **4. Run the Program**

```bash
./build/sudoku.exe
```

---

## 📝 Usage

### Generate a Sudoku Puzzle

The program automatically generates a valid Sudoku grid.

### Solve a Puzzle

The solver takes any valid puzzle and fills the missing values.


Outputs to:

```
problems.txt
solution.txt
output.pdf
```

PDF includes:

* Clean Sudoku grid layout
* Optional solutions section

---

## 🧠 Algorithms Used

### Sudoku Generator

* Backtracking
* Randomized number placement
* Constraint-checking for rows, columns, subgrids

### Sudoku Solver

* Depth-first search
* Backtracking
* Optimized validity checking

---

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch:

```bash
git checkout -b feature-new
```

3. Commit changes:

```bash
git commit -m "Added new feature"
```

4. Push and create a Pull Request

---

## 📬 Contact

Created by **Janmejai Pandey**
GitHub: [https://github.com/Janmejai-Pandey/SDF-PBL](https://github.com/Janmejai-Pandey/SDF-PBL)
