
#ifndef MATCH_H
#define MATCH_H

#include <string>

class Match {
private:
    std::string homeTeam;
    std::string awayTeam;
    int homeGoals;
    int awayGoals;

public:
    Match(const std::string& home,
          const std::string& away,
          int homeScore,
          int awayScore);

    std::string getHomeTeam() const;
    std::string getAwayTeam() const;
    int getHomeGoals() const;
    int getAwayGoals() const;

    void display() const;
};

#endif
