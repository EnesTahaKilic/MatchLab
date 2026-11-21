
#ifndef TEAM_H
#define TEAM_H

#include <string>

class Team {
private:
    std::string name;
    int played;
    int wins;
    int draws;
    int losses;
    int goalsFor;
    int goalsAgainst;
    int points;

public:
    Team(const std::string& teamName);

    void recordMatch(int scored, int conceded);

    std::string getName() const;
    int getPlayed() const;
    int getWins() const;
    int getDraws() const;
    int getLosses() const;
    int getGoalsFor() const;
    int getGoalsAgainst() const;
    int getGoalDifference() const;
    int getPoints() const;

    void display() const;
};

#endif
