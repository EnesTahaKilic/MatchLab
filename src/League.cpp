
#include "League.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>

namespace {

void sortStandings(std::vector<Team>& teams) {
    std::sort(teams.begin(), teams.end(),
              [](const Team& a, const Team& b) {
        if (a.getPoints() != b.getPoints()) {
            return a.getPoints() > b.getPoints();
        }
        if (a.getGoalDifference() != b.getGoalDifference()) {
            return a.getGoalDifference() > b.getGoalDifference();
        }
        if (a.getGoalsFor() != b.getGoalsFor()) {
            return a.getGoalsFor() > b.getGoalsFor();
        }
        return a.getName() < b.getName();
    });
}

} // namespace

Team* League::findTeam(const std::string& name) {
    for (Team& team : teams) {
        if (team.getName() == name) {
            return &team;
        }
    }
    return nullptr;
}

void League::addTeam(const std::string& name) {
    if (name.empty() || findTeam(name) != nullptr) {
        throw std::invalid_argument("Invalid or duplicate team");
    }

    teams.emplace_back(name);
}

void League::addMatch(const std::string& home,
                      const std::string& away,
                      int homeGoals,
                      int awayGoals) {
    addMatch(home, away, homeGoals, awayGoals, 0, 0);
}

void League::addMatch(const std::string& home,
                      const std::string& away,
                      int homeGoals,
                      int awayGoals,
                      std::int64_t kickoffTime,
                      std::int64_t resultAvailableTime) {
    // Validate the match before changing league data.
    Match match(home, away, homeGoals, awayGoals, kickoffTime, resultAvailableTime);

    Team* homeTeam = findTeam(home);
    Team* awayTeam = findTeam(away);

    if (homeTeam == nullptr || awayTeam == nullptr) {
        throw std::invalid_argument("Team not found");
    }

    matches.push_back(match);

    homeTeam->recordHomeMatch(homeGoals, awayGoals);
    awayTeam->recordAwayMatch(awayGoals, homeGoals);
}

void League::addMatches(const std::vector<Match>& newMatches) {
    for (const Match& match : newMatches) {
        if (findTeam(match.getHomeTeam()) == nullptr || findTeam(match.getAwayTeam()) == nullptr) {
            throw std::invalid_argument("Team not found in imported match");
        }
    }

    std::vector<Team> updatedTeams = teams;
    std::vector<Match> updatedMatches = matches;
    updatedMatches.reserve(updatedMatches.size() + newMatches.size());
    updatedMatches.insert(updatedMatches.end(), newMatches.begin(), newMatches.end());

    for (const Match& match : newMatches) {
        Team* homeTeam = nullptr;
        Team* awayTeam = nullptr;
        for (Team& team : updatedTeams) {
            if (team.getName() == match.getHomeTeam()) {
                homeTeam = &team;
            }
            if (team.getName() == match.getAwayTeam()) {
                awayTeam = &team;
            }
        }
        homeTeam->recordHomeMatch(match.getHomeGoals(), match.getAwayGoals());
        awayTeam->recordAwayMatch(match.getAwayGoals(), match.getHomeGoals());
    }

    teams.swap(updatedTeams);
    matches.swap(updatedMatches);
}

void League::displayMatches() const {
    if (matches.empty()) {
        std::cout << "No matches recorded.\n";
        return;
    }

    for (const Match& match : matches) {
        match.display();
    }
}

std::vector<Match> League::getMatches() const {
    return matches;
}

std::vector<Team> League::getStandings() const {
    std::vector<Team> sortedTeams = teams;
    sortStandings(sortedTeams);
    return sortedTeams;
}

std::vector<Team> League::getStandings(std::int64_t asOfTime) const {
    if (asOfTime < 0) {
        throw std::invalid_argument("Standings timestamp cannot be negative");
    }

    std::vector<Team> standings;
    standings.reserve(teams.size());
    for (const Team& team : teams) {
        standings.emplace_back(team.getName());
    }

    for (const Match& match : matches) {
        const std::int64_t availableAt = match.getResultAvailableTime();
        if (availableAt == 0 || availableAt > asOfTime) {
            continue;
        }

        Team* homeTeam = nullptr;
        Team* awayTeam = nullptr;
        for (Team& team : standings) {
            if (team.getName() == match.getHomeTeam()) {
                homeTeam = &team;
            }
            if (team.getName() == match.getAwayTeam()) {
                awayTeam = &team;
            }
        }
        homeTeam->recordHomeMatch(match.getHomeGoals(), match.getAwayGoals());
        awayTeam->recordAwayMatch(match.getAwayGoals(), match.getHomeGoals());
    }

    sortStandings(standings);
    return standings;
}

void League::displayStandings() const {
    const std::vector<Team> sortedTeams = getStandings();

    std::cout << "\n===== LEAGUE STANDINGS =====\n";

    int position = 1;

    for (const Team& team : sortedTeams) {
        std::cout << position++ << ". ";
        team.display();
    }
}
