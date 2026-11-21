#ifndef CSV_MATCH_READER_H
#define CSV_MATCH_READER_H

#include "Match.h"

#include <istream>
#include <vector>

class CsvMatchReader {
public:
    static std::vector<Match> read(std::istream& input);
};

#endif
