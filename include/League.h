
#ifndef LEAGUE_H
#define LEAGUE_H

#include "Match.h"
#include "Team.h"
#include <string>
#include <vector>
#include <cstdint>

class League {
private:
    std::vector<Team> teams;
    std::vector<Match> matches;

    Team* findTeam(const std::string& name);

public:
    void addTeam(const std::string& name);
    void addMatch(const std::string& home,
                  const std::string& away,
                  int homeGoals,
                  int awayGoals);
    void addMatch(const std::string& home,
                  const std::string& away,
                  int homeGoals,
                  int awayGoals,
                  std::int64_t kickoffTime,
                  std::int64_t resultAvailableTime);
    void addMatches(const std::vector<Match>& matches);

    void displayMatches() const;
    std::vector<Match> getMatches() const;
    std::vector<Team> getStandings() const;
    void displayStandings() const;
};

#endif
