#ifndef SREPL_SREPL_HPP
#define SREPL_SREPL_HPP

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

        toolbox::text_file::save(file, contents);
    }

private:
    template<class Iterator>
    void askUserForMatches(toolbox::fs::path file, std::string &contents, Iterator &iter)
    {
        auto line = *iter;
        auto m = std::cregex_iterator(line.begin(), line.end(), re_);
        auto e = std::cregex_iterator();

        for (; m != e; ++m) {
            auto matchStr = std::string{line.substr(m->position(), m->length())};
            auto result = std::regex_replace(matchStr, re_, with_);

            std::cout << file << ":" << iter.lineNumber() << ": "
                << line.substr(0, m->position())
                << TEXT_RED << TEXT_STRIKE << matchStr
                << TEXT_GREEN << result
                << TEXT_RESET << line.substr(m->position() + m->length());

            if (askYesNo() == false)
                continue;

            auto absolutePosition = line.data() - contents.data() + m->position();

            contents.replace(absolutePosition, m->length(), result);
            iter.recalculate();
        }
    }

    bool askYesNo()
    {
        while (true) {
            std::cout << "\nReplace? [y/n]: ";
            std::flush(std::cout);
            std::string r;
            std::cin >> r;

            if (r == "y" || r == "Y")
                return true;
            else if (r == "n" || r == "N")
                return false;
        }
    }

private:
    toolbox::fs::path   path_;
    std::regex          re_;
    std::string         with_;
};

#endif // SREPL_SREPL_HPP