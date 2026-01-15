# ♟️ Chess Engine (C++)

A fully playable chess game built in C++ that follows the official rules of chess.  
The project emphasizes clean object-oriented design, correct rule enforcement, and an AI opponent that evaluates future board states to make decisions.

---

## 🚀 Features

- Complete implementation of official chess rules
  - Legal move validation
  - Check, checkmate, and stalemate detection
- Object-oriented architecture with clear class hierarchies
- AI opponent that evaluates board positions and selects optimal moves
- Modular design that allows for future extensions

---

## 🧠 AI Overview

The AI evaluates potential future board states to determine the best move based on:
- Material advantage
- Board positioning
- Risk and reward trade-offs

The decision-making logic was designed to be readable, efficient, and easy to extend with stronger algorithms.

---

## 🛠 Tech Stack

- **Language:** C++
- **Paradigm:** Object-Oriented Programming
- **Key Concepts Used:**
  - Inheritance and polymorphism
  - Encapsulation and abstraction
  - State management
  - Algorithmic decision-making

---

## 🏗 Architecture

- `Board` manages the game state and piece positions
- `Piece` base class with derived classes for each chess piece
- `Move` logic enforces legal gameplay
- `AI` module evaluates board states and selects moves

---

## ▶️ How to Run

```bash
# Compile
g++ -std=c++17 -o chess main.cpp

# Run
./chess
