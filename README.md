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

An example input file is available at `data/matches.csv`. The reader returns
`std::vector<Match>` and does not add records to a league automatically. For
example:

```cpp
#include "CsvMatchReader.h"

#include <fstream>

std::ifstream input("data/matches.csv");
const std::vector<Match> matches = CsvMatchReader::read(input);
```

## Learning roadmap

Each day below represents one focused learning session. A session is complete
when its code, tests, and short review are done. The order can change when a
session exposes a prerequisite.

### Completed sessions

1. **C++ core and league table** — Match, Team, League, validation, statistics,
   standings, and initial tests.
2. **Time-aware match data** — kickoff and result availability timestamps.
3. **Testable CMake project** — core library, CTest target, and README build steps.
4. **First CSV reader** — strict six-column input, validation, and sample data.

### Upcoming sessions

5. **Finish the CSV format** — support quoted team names and commas in fields,
   document malformed-row behavior, and test edge cases.
6. **Load matches into a League** — preserve timestamps while importing, validate
   all referenced teams, and make bulk import safe if one row is invalid.
7. **Standings at a point in time** — calculate results using only matches whose
   result was available by the requested timestamp. Test the exact cutoff.
8. **Home and away statistics** — expose separate scoring and conceding totals
   and cover teams with few or no matches.
9. **Recent form** — calculate a defined recent-match window in chronological
   order and test matches tied at the cutoff.
10. **Historical CSV dataset** — select a consistent source format, import a
    small real dataset, and check row counts and data quality.
11. **Baseline goal model** — calculate home and away expected goals from past
    data, with no information after the prediction time included.
12. **Outcome probabilities** — derive home win, draw, and away win estimates
    from a simple statistical model and verify probabilities sum to one.
13. **Chronological evaluation** — split historical matches by time and report
    log loss and Brier score against a simple baseline.
14. **Prediction interface** — let a user select a fixture and show probabilities,
    expected goals, and a brief explanation of available data.
15. **External data API** — investigate provider terms and data shape, then add
    an optional importer without coupling the core model to one service.

At the start of a session, review the day's goal and concepts. At the end, run
the tests, summarize the design decisions, and commit a few related completed
sessions together when they form a coherent change.

## Goals

The goal of MatchLab is to build a modular football analysis application
that can later support historical data import, statistical analysis, and
carefully evaluated match predictions.
