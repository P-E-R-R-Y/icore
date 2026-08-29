/**
 * @file DummyAppModule.hpp
 * @brief Un jeu vu comme module chargeable.
 *
 * LA DLL QUI ALLOUE EST CELLE QUI LIBERE : createApp et deleteApp vont par
 * paire pour cette seule raison. Ici on compte les deux pour verifier que
 * la paire tient.
 */

#ifndef DUMMYAPPMODULE_HPP_
#define DUMMYAPPMODULE_HPP_

#include "DummyApp.hpp"

#include "IAppModule.hpp"

class DummyAppModule : public IAppModule {

    public:
        int made = 0;
        int freed = 0;
        IModuleRegistry *received = nullptr;

        const char *type() const override { return IAppModule::contract; }
        const char *name() const override { return "faux"; }

        IApp *createApp(IModuleRegistry &modules) override
        {
            made++;
            received = &modules;
            return new DummyApp(1);
        }

        void deleteApp(IApp *app) override
        {
            freed++;
            delete app;
        }
};

#endif /* !DUMMYAPPMODULE_HPP_ */
