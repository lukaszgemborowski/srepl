#include "IterateFilesystem.hpp"
#include "Srepl.hpp"
#include <args/args.hpp>
#include <iostream>

int main(int argc, char **argv)
{
    auto mode = SreplMode::Show;
    auto parser = args::parser{
        args::opt{'i', false, args::save{mode}, [](auto) { return SreplMode::Interactive; }},
        args::opt{'y', false, args::save{mode}, [](auto) { return SreplMode::Replace; }},
    };

    parser.parse(argc, argv);
    auto args = parser.posargs();

    if (args.size() != 3) {
        std::cerr << argv[0] << " [path] [regex] [replace_with]\n";
        return 0;
    }

    Srepl srepl {
        mode,
        args[0],
        std::regex{args[1]},
        args[2]
    };

    IterateFilesystem(args[0], srepl);
}
