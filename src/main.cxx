#include "IterateFilesystem.hpp"
#include "Srepl.hpp"
#include "Args.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    Args args{argc, argv};

    if (args.validate()) {
        Srepl srepl {
            args.mode(),
            args.path(),
            std::regex{args.pattern()},
            args.replacement()
        };

        IterateFilesystem(args.path(), srepl);
    }
}
