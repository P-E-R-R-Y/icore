/**
 * @file DummyCli.hpp
 * @brief Un ICore : une action, pas une boucle.
 *
 * Le contrat minimal existe pour ca - une CLI, un batch, un script font une
 * chose et s'arretent. Le lanceur ne fait aucune difference entre les deux.
 */

#ifndef DUMMYCLI_HPP_
#define DUMMYCLI_HPP_

#include "ICore.hpp"

#include <string>

class DummyCli : public ICore {

    public:
        DummyCli(int code, std::string &log) : _code(code), _log(log) { _log += "ctor "; }
        ~DummyCli() override { _log += "dtor "; }

        int run() override { _log += "run "; return _code; }

    private:
        int _code;
        std::string &_log;
};

#endif /* !DUMMYCLI_HPP_ */
