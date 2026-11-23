
#include "Team.h"

#include <iostream>
#include <stdexcept>

Team::Team(const std::string& teamName)
    : name(teamName),
      played(0),
      wins(0),
      draws(0),
      losses(0),
      goalsFor(0),
      goalsAgainst(0),
      points(0),
      homePlayed(0),
      awayPlayed(0),
      homeGoalsFor(0),
      homeGoalsAgainst(0),
      awayGoalsFor(0),
      awayGoalsAgainst(0) {

    if (teamName.empty()) {
        throw std::invalid_argument("Team name cannot be empty");
    }
}

void Team::recordHomeMatch(int scored, int conceded) {
    recordMatch(scored, conceded);
    ++homePlayed;
    homeGoalsFor += scored;
    homeGoalsAgainst += conceded;
}

void Team::recordAwayMatch(int scored, int conceded) {
    recordMatch(scored, conceded);
    ++awayPlayed;
    awayGoalsFor += scored;
    awayGoalsAgainst += conceded;
}

void Team::recordMatch(int scored, int conceded) {
    if (scored < 0 || conceded < 0) {
        throw std::invalid_argument("Goals cannot be negative");
    }

    played++;
    goalsFor += scored;
    goalsAgainst += conceded;

    if (scored > conceded) {
        wins++;
        points += 3;
    }
    else if (scored == conceded) {
        draws++;
        points += 1;
    }
    else {
        losses++;
    }
}

std::string Team::getName() const {
    return name;
}

int Team::getPlayed() const {
    return played;
}

int Team::getWins() const {
    return wins;
}

int Team::getDraws() const {
    return draws;
}

int Team::getLosses() const {
    return losses;
}

int Team::getGoalsFor() const {
    return goalsFor;
}

int Team::getGoalsAgainst() const {
    return goalsAgainst;
}

int Team::getGoalDifference() const {
    return goalsFor - goalsAgainst;
}

int Team::getPoints() const {
    return points;
}

int Team::getHomePlayed() const {
    return homePlayed;
}

int Team::getAwayPlayed() const {
    return awayPlayed;
}

int Team::getHomeGoalsFor() const {
    return homeGoalsFor;
}

int Team::getHomeGoalsAgainst() const {
    return homeGoalsAgainst;
}

int Team::getAwayGoalsFor() const {
    return awayGoalsFor;
}

int Team::getAwayGoalsAgainst() const {
    return awayGoalsAgainst;
}

void Team::display() const {
    std::cout << name
              << " | Played: " << played
              << " | W: " << wins
              << " | D: " << draws
              << " | L: " << losses
              << " | GD: " << getGoalDifference()
              << " | Points: " << points
              << '\n';
}
