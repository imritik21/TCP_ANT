# Ant Colony Optimization for TSP

This project implements the Ant Colony Optimization (ACO) algorithm in C++ to solve the Traveling Salesman Problem (TSP). It simulates a colony of ants that build tours based on pheromone trails and heuristic information (inverse of distances) and then iteratively improves the solution.

---

## Table of Contents

- [Overview](#overview)
- [Algorithm Flow](#algorithm-flow)
- [Detailed Explanation](#detailed-explanation)
  - [1. Initialization](#1-initialization)
  - [2. Main Iteration Loop](#2-main-iteration-loop)
  - [3. Tour Construction](#3-tour-construction)
  - [4. Pheromone Update](#4-pheromone-update)
  - [5. Output](#5-output)
- [Compilation and Execution](#compilation-and-execution)
- [License](#license)

---

## Overview

Ant Colony Optimization is inspired by the foraging behavior of ants. This implementation uses:
- **Ants** as agents to build candidate tours.
- **Pheromone trails** to reinforce good paths.
- **Heuristic information** (distance inverses) to guide search.
- **Evaporation** to gradually reduce pheromone levels on less promising paths.

The algorithm iteratively builds solutions and updates pheromones until it converges to a near-optimal tour.

---

## Algorithm Flow

Below is a graphical flowchart:

```mermaid
flowchart TD
    A[Initialization] --> B[For Each Iteration]
    B --> C[For Each Ant]
    C --> D[Construct Tour]
    D --> E["Select Next City\n(Roulette Wheel Selection)"]
    E --> F[Add City to Tour & Mark as Visited]
    F --> G{Tour Complete?}
    G -- No --> E
    G -- Yes --> H[Compute Tour Length]
    H --> I[Update Best Tour if Shorter]
    I --> J[End Ant Loop]
    J --> K[Update Pheromones Based on All Tours]
    K --> B
    B --> L[End Iteration Loop]
    L --> M[Output Best Tour]

