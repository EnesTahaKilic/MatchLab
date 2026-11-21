
#include "Match.h"
#include <iostream>
#include <stdexcept>

Match::Match(const std::string& home,
             const std::string& away,
             int homeScore,
             int awayScore,
             std::int64_t matchKickoffTime,
             std::int64_t matchResultAvailableTime)
    : homeTeam(home),
      awayTeam(away),
      homeGoals(homeScore),
      awayGoals(awayScore),
      kickoffTime(matchKickoffTime),
      resultAvailableTime(matchResultAvailableTime) {

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

std::int64_t Match::getKickoffTime() const {
    return kickoffTime;
}

std::int64_t Match::getResultAvailableTime() const {
    return resultAvailableTime;
}

void Match::display() const {
    std::cout << homeTeam << " "
              << homeGoals << " - "
              << awayGoals << " "
              << awayTeam << '\n';
}
