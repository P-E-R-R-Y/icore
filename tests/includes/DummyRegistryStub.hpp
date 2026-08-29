/**
 * @file DummyRegistryStub.hpp
 * @brief Un registre vide : icore n'a besoin que d'une adresse a passer.
 *
 * Le contenu d'un registre se teste chez imodule ; ici on verifie seulement
 * que createApp recoit bien CELUI qu'on lui a donne.
 */

#ifndef DUMMYREGISTRYSTUB_HPP_
#define DUMMYREGISTRYSTUB_HPP_

#include "IModuleRegistry.hpp"

class DummyRegistryStub : public IModuleRegistry {

    public:
        IModule *Get(const std::string &, const std::string &) override { return nullptr; }
        std::vector<IModule *> GetAllByType(const std::string &) override { return {}; }
        std::vector<IModule *> GetAllByKey(const std::string &) override { return {}; }
        std::vector<IModule *> GetAll() override { return {}; }
        IModule *Current(const std::string &) override { return nullptr; }
        void Select(const std::string &, IModule *) override {}
};

#endif /* !DUMMYREGISTRYSTUB_HPP_ */
