# MatchLab

A C++17 football analytics project for recording match results, managing
teams, and calculating league standings.

## Features

- Match results with optional kickoff and result availability timestamps
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

If CMake is unavailable, the core tests can be compiled directly with a
C++17 compiler from the project root:

```sh
c++ -std=c++17 -Wall -Wextra -Wpedantic -I include \
    src/Match.cpp src/Team.cpp src/League.cpp tests/LeagueTests.cpp \
    -o matchlab_tests
./matchlab_tests
```

## Project Status

The first core milestone is implemented. It covers match and team validation,
result statistics, queryable league standings, and stored kickoff and result
availability times. CSV import, historical analysis, prediction models, and
model evaluation remain planned work.

## Goals

The goal of MatchLab is to build a modular football analysis application
that can later support historical data import, statistical analysis, and
carefully evaluated match predictions.
