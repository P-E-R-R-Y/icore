/**
 * @file ICore.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief Le socle d'un programme : ICore (une action) et IApp (une boucle).
 * @date 2026-08-04
 */

#ifndef ICORE_HPP
#define ICORE_HPP

#include <utility>

/**
 * @interface ICore
 * @brief Ca se lance, ca rend un code de sortie. Pas forcement une boucle.
 *
 * L'init est le constructeur, le nettoyage le destructeur.
 *
 *     class MyCli : public ICore {
 *         MyCli(int argc, char **argv) { ... }
 *         int run() override { ...; return 0; }
 *     };
 */
class ICore {
    public:
        virtual ~ICore() = default;

        /**
         * @brief Deroule le programme.
         * @return le code de sortie.
         */
        virtual int run() = 0;
};

/**
 * @interface ITickable
 * @brief Ce qui se fait ticker par quelqu'un d'autre, frame par frame.
 *
 * L'ordre est event(), puis update(), puis display().
 */
class ITickable {
    public:
        virtual ~ITickable() = default;

        virtual void event() = 0;
        virtual void update() = 0;
        virtual void display() = 0;
};

/**
 * @class IApp
 * @brief Une application a boucle. Elle tourne seule, ou se fait ticker.
 *
 * On en herite pour remplir event / update / display.
 *
 *     class MyApp : public IApp {
 *         void event() override   { ... }
 *         void update() override  { if (done) stop(); }
 *         void display() override { ... }
 *     };
 *
 * ITickable est publiquement herite : c'est ce qui permet a un hote de la
 * piloter frame par frame au lieu de lui ceder la boucle.
 */
class IApp : public ICore, public ITickable {
    public:
        virtual ~IApp() = default;

        /** @brief La boucle, jusqu'a stop(). Non redefinissable. */
        int run() final {
            _running = true;
            while (_running) {
                event();
                update();
                display();
            }
            return _exitCode;
        }

        /**
         * @brief Demande l'arret. La frame en cours se termine normalement.
         * @param code le code de sortie rendu par run().
         */
        void stop(int code = 0) {
            _running = false;
            _exitCode = code;
        }

        /** @brief Vrai des la construction, faux apres stop(). */
        bool running() const { return _running; }

    private:
        /* Vrai des la construction : une IApp hebergee ne passe jamais par
         * run(), elle naitrait sinon deja arretee. */
        bool _running = true;
        int _exitCode = 0;
};

/**
 * @brief Construit, lance, detruit. Les arguments vont au constructeur.
 *
 *     int main(int argc, char **argv) { return launch<MyCli>(argc, argv); }
 *     int main()                      { return launch<MyApp>(); }
 *
 * L'objet est detruit avant que le code de sortie ne remonte.
 */
template <typename T, typename... Args>
int launch(Args &&...args) {
    T core(std::forward<Args>(args)...);
    return core.run();
}

#endif /* !ICORE_HPP */
