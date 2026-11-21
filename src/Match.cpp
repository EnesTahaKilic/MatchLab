
#include "Match.h"
#include <iostream>
#include <stdexcept>

Match::Match(const std::string& home,
             const std::string& away,
             int homeScore,
             int awayScore)
    : homeTeam(home),
      awayTeam(away),
      homeGoals(homeScore),
      awayGoals(awayScore) {

    if (home.empty() || away.empty() || home == away ||
        homeScore < 0 || awayScore < 0) {
        throw std::invalid_argument("Invalid match data");
    }
}

std::string Match::getHomeTeam() const {
    return homeTeam;
}

std::string Match::getAwayTeam() const {
    return awayTeam;
}

int Match::getHomeGoals() const {
    return homeGoals;
}

int Match::getAwayGoals() const {
    return awayGoals;
}

void Match::display() const {
    std::cout << homeTeam << " "
              << homeGoals << " - "
              << awayGoals << " "
              << awayTeam << '\n';
}
