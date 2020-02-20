#include "IterateFilesystem.hpp"
#include <toolbox/text_file.hpp>
#include <toolbox/string/line_iterator.hpp>
#include <regex>
#include <string>
#include <iostream>

namespace
{
const auto TEXT_GREEN = "\x1b[32m";
const auto TEXT_RED = "\x1b[31m";
const auto TEXT_RESET = "\x1b[0m";
const auto TEXT_STRIKE = "\x1b[9m";
}

class Srepl
{
public:
    Srepl(toolbox::fs::path path, std::regex re, std::string with)
        : path_ {path}
        , re_ {re}
        , with_ {with}
    {}

    void operator()(toolbox::fs::path file)
    {
        auto contents = toolbox::text_file::load(file);
        auto line = toolbox::string::line_iterator{contents};
        auto end = toolbox::string::line_iterator{};

        while (line != end) {
            askUserForMatches(file, contents, line);
            ++ line;
        }
    }

private:
    template<class Iterator>
    void askUserForMatches(toolbox::fs::path file, std::string &contents, Iterator &iter)
    {
        auto line = *iter;
        auto m = std::cregex_iterator(line.begin(), line.end(), re_);
        auto e = std::cregex_iterator();

        while (m != e) {
            auto matchStr = std::string{line.substr(m->position(), m->length())};
            auto result = std::regex_replace(matchStr, re_, with_);

            std::cout << file << ":" << iter.lineNumber() << ": "
                << line.substr(0, m->position())
                << TEXT_RED << TEXT_STRIKE << matchStr
                << TEXT_GREEN << result
                << TEXT_RESET << line.substr(m->position() + m->length())
                << std::endl;
            m ++;
        }
    }

private:
    toolbox::fs::path   path_;
    std::regex          re_;
    std::string         with_;
};

int main(int argc, char **argv)
{
    Srepl srepl {argv[1], std::regex{argv[2]}, argv[3]};
    IterateFilesystem(argv[1], srepl);
}
