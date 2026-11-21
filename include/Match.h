
#ifndef MATCH_H
#define MATCH_H

#include <string>
#include <cstdint>

class Match {
private:
    std::string homeTeam;
    std::string awayTeam;
    int homeGoals;
    int awayGoals;
    std::int64_t kickoffTime;

public:
    Match(const std::string& home,
          const std::string& away,
          int homeScore,
          int awayScore,
          std::int64_t kickoffTime = 0);

    std::string getHomeTeam() const;
    std::string getAwayTeam() const;
    int getHomeGoals() const;
    int getAwayGoals() const;
    std::int64_t getKickoffTime() const;

    void display() const;
};

#endif
