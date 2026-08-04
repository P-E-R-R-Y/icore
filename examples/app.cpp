/* IApp — une boucle. run() est deja ecrit : on ne remplit que les 3 etapes.
 * Ici le constructeur recoit un argument du main via launch<>. */
#include <cstdio>
#include <cstdlib>

#include "ICore.hpp"

class MyApp final : public IApp {
    public:
        explicit MyApp(unsigned long frames) : _frames(frames) {
            std::printf("[app] pret pour %lu frames\n", _frames);
        }

        ~MyApp() override { std::printf("[app] nettoye\n"); }

    protected:
        void event() override {}

        void update() override {
            if (_frame >= _frames) stop();
        }

        void display() override { std::printf("[app] frame %lu\n", _frame++); }

    private:
        unsigned long _frames;
        unsigned long _frame = 0;
};

int main(int argc, char **argv) {
    unsigned long frames = (argc > 1) ? std::strtoul(argv[1], nullptr, 10) : 3;
    return launch<MyApp>(frames);
}
