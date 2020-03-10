#ifndef SREPL_ARGS_HPP
#define SREPL_ARGS_HPP

#include "SreplMode.hpp"
#include <string>

class Args
{
public:
    Args(int argc, char **argv);

    bool validate() const;
    SreplMode mode() const;
    std::string path() const;
    std::string pattern() const;
    std::string replacement() const;

private:
    SreplMode mode_ = SreplMode::Show;
    char* path_ = nullptr;
    char* pattern_ = nullptr;
    char* replacement_ = nullptr;
};

#endif // SREPL_ARGS_HPP
