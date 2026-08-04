/* ICore — une action, pas une boucle.
 * Le constructeur initialise, le destructeur nettoie, run() fait le travail.
 * launch<> transmet argc/argv au constructeur tels quels. */
#include <cstdio>

#include "ICore.hpp"

class MyCli final : public ICore {
    public:
        MyCli(int argc, char **argv) : _argc(argc), _argv(argv) {
            std::printf("[cli] pret\n");
        }

        ~MyCli() override { std::printf("[cli] nettoye\n"); }

        int run() override {
            for (int i = 0; i < _argc; ++i)
                std::printf("[cli] arg %d: %s\n", i, _argv[i]);
            return 0;
        }

    private:
        int _argc;
        char **_argv;
};

int main(int argc, char **argv) {
    return launch<MyCli>(argc, argv);
}
