
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
    int homePlayed;
    int awayPlayed;
    int homeGoalsFor;
    int homeGoalsAgainst;
    int awayGoalsFor;
    int awayGoalsAgainst;

public:
    Team(const std::string& teamName);

    void recordMatch(int scored, int conceded);
    void recordHomeMatch(int scored, int conceded);
    void recordAwayMatch(int scored, int conceded);

    std::string getName() const;
    int getPlayed() const;
    int getWins() const;
    int getDraws() const;
    int getLosses() const;
    int getGoalsFor() const;
    int getGoalsAgainst() const;
    int getGoalDifference() const;
    int getPoints() const;
    int getHomePlayed() const;
    int getAwayPlayed() const;
    int getHomeGoalsFor() const;
    int getHomeGoalsAgainst() const;
    int getAwayGoalsFor() const;
    int getAwayGoalsAgainst() const;

    void display() const;
};

#endif
