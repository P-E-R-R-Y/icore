/**
 * @file IAppModule.hpp
 * @author Perry Chouteau (perry.chouteau@outlook.com)
 * @brief Ce qui permet a une application de vivre dans une dll.
 * @date 2026-08-11
 *
 * @addtogroup icore
 * @{
 */

#ifndef IAPP_MODULE_HPP
#define IAPP_MODULE_HPP

#include "ICore.hpp"
#include "IModule.hpp"
#include "IModuleManager.hpp"

/**
 * @class IAppModule
 * @brief La fabrique qu'exporte une application chargeable.
 *
 * L'hote lit ce qu'il recupere comme il veut : en ICore il appelle run() et
 * cede sa boucle, en ITickable il appelle event()/update()/display() et
 * garde la sienne.
 *
 *     modules.Load("./game.so", "game");
 *     IApp *app = modules.Get<IAppModule>("game")->createApp(modules);
 */
class IAppModule : public IModule {

public:
    /// Le symbole que le chargeur cherche.
    static constexpr const char *entry = "getAppModule";

    /// Ce que type() doit rendre.
    static constexpr const char *contract = "game";

    /// Les contrats que ce type accepte.
    static constexpr const char *accepts[] = {"game", nullptr};

    virtual ~IAppModule() = default;

    /**
     * @brief Construit l'application.
     *
     * Le manager lui est donne vivant, pas en instantane : elle y relit ce
     * qui est charge quand elle en a besoin.
     *
     * @param modules
     * @return IApp*
     */
    virtual IApp *createApp(IModuleManager &modules) = 0;

    /**
     * @brief La detruit. Toujours par la bibliotheque qui l'a construite.
     *
     * @param app
     */
    virtual void deleteApp(IApp *app) = 0;
};

/** @} */

#endif /* !IAPP_MODULE_HPP */
