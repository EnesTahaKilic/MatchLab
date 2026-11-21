
#ifndef MATCH_H
#define MATCH_H

#include <cstdint>
#include <string>

class Match {
private:
    std::string homeTeam;
    std::string awayTeam;
    int homeGoals;
    int awayGoals;
    std::int64_t kickoffTime;
    std::int64_t resultAvailableTime;

public:
    Match(const std::string& home,
          const std::string& away,
          int homeScore,
          int awayScore,
          std::int64_t kickoffTime = 0,
          std::int64_t resultAvailableTime = 0);

    std::string getHomeTeam() const;
    std::string getAwayTeam() const;
    int getHomeGoals() const;
    int getAwayGoals() const;
    std::int64_t getKickoffTime() const;
    std::int64_t getResultAvailableTime() const;

    void display() const;
};

#endif
