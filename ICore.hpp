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
 * @brief Le contrat minimal : ca se lance, ca rend un code de sortie.
 *
 * Volontairement PAS une boucle : une CLI, un batch, un script font une
 * action et s'arretent. Le lanceur ne fait aucune difference entre les deux.
 *
 *     class MyCli : public ICore {
 *     public:
 *         MyCli(int argc, char **argv) { ... }   // l'init EST le constructeur
 *         ~MyCli() override { ... }              // le nettoyage, le destructeur
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

class ITickable {                // ça se fait ticker par quelqu'un
    public:
        virtual ~ITickable() = default;

        /**
         * @brief Une frame de l'application.
         *
         * L'ordre est event() puis update() puis display(). Le tickeur ne
         * fait pas de rendu : il ne sait pas ce qu'est un "display".
         */
        virtual void event() = 0;
        virtual void update() = 0;
        virtual void display() = 0;
};

/**
 * @class IApp
 * @brief Une application a boucle.
 *
 * `run()` est ECRIT ici : c'est du code, pas une promesse de plus. On en
 * herite pour remplir event / update / display, et rien d'autre.
 *
 * Il n'y a pas d'`init()` ni de `destroy()` : ce sont le constructeur et le
 * destructeur. Un objet qui existe est un objet pret.
 *
 *     class MyApp : public IApp {
 *     public:
 *         MyApp() { ... }              // init
 *         ~MyApp() override { ... }    // destroy
 *     protected:
 *         void event() override   { ... }
 *         void update() override  { if (done) stop(); }
 *         void display() override { ... }
 *     };
 */
class IApp : public ICore, protected ITickable {
    public:
        virtual ~IApp() = default;

        /**
         * @brief La boucle. Non redefinissable : c'est ce qui rend le cycle
         *        previsible d'une application a l'autre.
         */
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

        /** @brief La boucle tourne-t-elle ? */
        bool running() const { return _running; }

//    protected:
//        /** @brief Les entrees : clavier, requetes, reseau... */
//        virtual void event() = 0;
//
//        /** @brief La logique. Elle tourne meme sans affichage. */
//        virtual void update() = 0;
//
//        /** @brief Le rendu. */
//        virtual void display() = 0;

    private:
        bool _running = false;
        int _exitCode = 0;
};

/**
 * @brief Construit puis lance. Les arguments sont transmis TELS QUELS au
 *        constructeur : argc/argv, une config, ou rien.
 *
 * Le main devient identique pour une action ou une boucle :
 *
 *     int main(int argc, char **argv) { return launch<MyCli>(argc, argv); }
 *     int main()                      { return launch<MyApp>(); }
 *
 * L'objet meurt AVANT le retour : le destructeur a fini de tourner quand le
 * code de sortie remonte au systeme.
 */
template <typename T, typename... Args>
int launch(Args &&...args) {
    T core(std::forward<Args>(args)...);
    return core.run();
}

#endif /* !ICORE_HPP */
