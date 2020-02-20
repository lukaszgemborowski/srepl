#include "IterateFilesystem.hpp"
#include "Srepl.hpp"

int main(int argc, char **argv)
{
    Srepl srepl {argv[1], std::regex{argv[2]}, argv[3]};
    IterateFilesystem(argv[1], srepl);
}
