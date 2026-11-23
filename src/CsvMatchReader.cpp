#include "CsvMatchReader.h"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

namespace {

std::string trim(std::string value) {
    const auto first = std::find_if_not(value.begin(), value.end(), [](unsigned char character) {
        return std::isspace(character) != 0;
    });
    const auto last = std::find_if_not(value.rbegin(), value.rend(), [](unsigned char character) {
        return std::isspace(character) != 0;
    }).base();

    if (first >= last) {
        return {};
    }

    return std::string(first, last);
}

std::int64_t parseInteger(const std::string& value, std::size_t lineNumber) {
    std::istringstream stream(trim(value));
    std::int64_t parsed = 0;
    stream >> parsed;

    if (!stream || stream.peek() != std::char_traits<char>::eof()) {
        throw std::invalid_argument("Invalid integer on CSV line " + std::to_string(lineNumber));
    }

    return parsed;
}

std::vector<std::string> parseCsvLine(const std::string& line, std::size_t lineNumber) {
    std::vector<std::string> fields;
    std::string field;
    bool insideQuotes = false;
    bool quotedFieldClosed = false;
    bool fieldWasQuoted = false;

    for (std::size_t i = 0; i < line.size(); ++i) {
        const char character = line[i];

        if (character == '"') {
            if (insideQuotes && i + 1 < line.size() && line[i + 1] == '"') {
                field.push_back('"');
                ++i;
            } else if (!insideQuotes && trim(field).empty() && !quotedFieldClosed) {
                field.clear();
                insideQuotes = true;
                fieldWasQuoted = true;
            } else if (insideQuotes) {
                insideQuotes = false;
                quotedFieldClosed = true;
            } else {
                throw std::invalid_argument("Unexpected quote on CSV line " + std::to_string(lineNumber));
            }
        } else if (character == ',' && !insideQuotes) {
            fields.push_back(fieldWasQuoted ? std::move(field) : trim(std::move(field)));
            field.clear();
            quotedFieldClosed = false;
            fieldWasQuoted = false;
        } else {
            if (quotedFieldClosed) {
                if (!std::isspace(static_cast<unsigned char>(character))) {
                    throw std::invalid_argument("Unexpected content after quoted field on CSV line " +
                                                std::to_string(lineNumber));
                }
            } else {
                field.push_back(character);
            }
        }
    }

    if (insideQuotes) {
        throw std::invalid_argument("Unclosed quoted field on CSV line " + std::to_string(lineNumber));
    }

    fields.push_back(fieldWasQuoted ? std::move(field) : trim(std::move(field)));
    return fields;
}

} // namespace

std::vector<Match> CsvMatchReader::read(std::istream& input) {
    constexpr const char* expectedHeader =
        "home_team,away_team,home_goals,away_goals,kickoff_time,result_available_time";

    std::string line;
    std::size_t lineNumber = 1;
    if (!std::getline(input, line) || trim(line) != expectedHeader) {
        throw std::invalid_argument("CSV header does not match the expected format");
    }

    std::vector<Match> matches;
    while (std::getline(input, line)) {
        ++lineNumber;
        if (line.empty()) {
            continue;
        }

        const std::vector<std::string> fields = parseCsvLine(line, lineNumber);
        if (fields.size() != 6) {
            throw std::invalid_argument("Expected six CSV columns on line " + std::to_string(lineNumber));
        }

        const std::int64_t homeGoals = parseInteger(fields[2], lineNumber);
        const std::int64_t awayGoals = parseInteger(fields[3], lineNumber);
        const std::int64_t kickoffTime = parseInteger(fields[4], lineNumber);
        const std::int64_t resultAvailableTime = parseInteger(fields[5], lineNumber);

        if (homeGoals > std::numeric_limits<int>::max() || awayGoals > std::numeric_limits<int>::max() ||
            homeGoals < std::numeric_limits<int>::min() || awayGoals < std::numeric_limits<int>::min()) {
            throw std::invalid_argument("Goal count is outside the supported range on CSV line " +
                                        std::to_string(lineNumber));
        }

        matches.emplace_back(fields[0], fields[1], static_cast<int>(homeGoals),
                             static_cast<int>(awayGoals), kickoffTime, resultAvailableTime);
    }

    if (input.bad()) {
        throw std::runtime_error("Failed while reading CSV input");
    }

    return matches;
}
