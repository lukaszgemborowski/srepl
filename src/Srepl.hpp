#ifndef SREPL_SREPL_HPP
#define SREPL_SREPL_HPP

#include <regex>
#include <string>
#include <toolbox/cpp/filesystem.hpp>

class Srepl
{
public:
    Srepl(toolbox::fs::path path, std::regex re, std::string with);
    void operator()(toolbox::fs::path file);

private:
    template<class Iterator>
    bool askUserForMatches(toolbox::fs::path file, std::string &contents, Iterator &iter)
    {
        if (askUserForMatches(file, contents, iter.lineNumber(), *iter)) {
            iter.recalculate();
            return true;
        } else {
            return false;
        }
    }

    bool askUserForMatches(toolbox::fs::path file, std::string &contents, std::size_t lineNumber, std::string_view line);

private:
    toolbox::fs::path   path_;
    std::regex          re_;
    std::string         with_;
};

#endif // SREPL_SREPL_HPP