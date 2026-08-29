/**
 * @file DummyApp.hpp
 * @brief Une application qui note ce qu'on lui fait.
 *
 * Elle n'affiche rien et ne calcule rien : elle ecrit l'ordre des appels
 * dans une chaine. C'est exactement ce qu'il faut pour verifier qu'une IApp
 * se comporte pareil qu'elle tourne seule ou qu'un hote la ticke.
 */

#ifndef DUMMYAPP_HPP_
#define DUMMYAPP_HPP_

#include "ICore.hpp"

#include <string>

class DummyApp : public IApp {

    public:
        /** @param frames nombre de tours avant de demander l'arret. 0 : jamais. */
        explicit DummyApp(int frames = 0, int code = 0)
            : _frames(frames), _code(code) {}

        /** @brief L'ordre des appels, un caractere chacun : "eud" par tour. */
        std::string trace;
        int turns = 0;
        bool destroyed = false;

        void event() override   { trace += 'e'; }
        void display() override { trace += 'd'; }

        void update() override
        {
            trace += 'u';
            turns++;
            if (_frames > 0 && turns >= _frames)
                stop(_code);
        }

    private:
        int _frames;
        int _code;
};

/** @brief Elle previent quand elle meurt : de quoi tester launch(). */
class TrackedApp : public IApp {

    public:
        explicit TrackedApp(bool &alive) : _alive(alive) { _alive = true; }
        ~TrackedApp() override { _alive = false; }

        void event() override {}
        void update() override { stop(7); }
        void display() override {}

    private:
        bool &_alive;
};

#endif /* !DUMMYAPP_HPP_ */
