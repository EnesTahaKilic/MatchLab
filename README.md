# MatchLab

A C++17 football analytics project for recording match results, managing
teams, and calculating league standings.

## Features

- Match results with optional kickoff and result availability timestamps
- Team statistics for wins, draws, losses, and goals
- League standings sorted by points, goal difference, goals scored, then name
- Automated tests for standings and invalid match handling
- CSV import for completed match results

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

The C++ core and a basic CSV reader are implemented. The reader expects the
header `home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time`,
with kickoff and result availability expressed as Unix timestamps in seconds.
Historical analysis, prediction models, and model evaluation remain planned.

## Goals

The goal of MatchLab is to build a modular football analysis application
that can later support historical data import, statistical analysis, and
carefully evaluated match predictions.
