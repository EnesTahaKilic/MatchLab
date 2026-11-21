# MatchLab

A C++17 football analytics project for recording match results, managing
teams, and calculating league standings.

## Features

- Match results with optional kickoff timestamps
- Team statistics for wins, draws, losses, and goals
- League standings sorted by points, goal difference, goals scored, then name
- Automated tests for standings and invalid match handling

## Technologies

- C++17
- CMake
- Git
- GitHub

## Build and test

Requirements: CMake 3.20 or newer and a C++17 compiler.

```sh
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Project Status

The C++ core and league standings are implemented. Historical data import,
prediction models, and evaluation are planned.

## Goals

The goal of MatchLab is to build a modular football analysis application
that can later support historical data import, statistical analysis, and
carefully evaluated match predictions.
