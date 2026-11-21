
#ifndef LEAGUE_H
#define LEAGUE_H

#include "Match.h"
#include "Team.h"
#include <string>
#include <vector>

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

    void displayMatches() const;
    std::vector<Team> getStandings() const;
    void displayStandings() const;
};

#endif
