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
#include "IModuleRegistry.hpp"

/**
 * @class IAppModule
 * @brief The factory a loadable application exports.
 *
 * One module type covers every case, because IApp already IS an ICore and
 * IS an ITickable. The host decides how to read what it gets back :
 *
 * - as an ICore, it calls run() and hands over the whole loop,
 * - as an ITickable, it calls event()/update()/display() itself and keeps
 *   the loop.
 *
 * The second is the interesting one : several applications ticked by the
 * same host, sharing one window and one frame.
 *
 *     modules.Load("./game.so", "game");
 *     auto *app = modules.Get<IAppModule>("game")->createApp(everything);
 *     while (window->isOpen())
 *         app->...   // ticked by the host
 */
class IAppModule : public IModule {

public:
    /// Symbol the loader looks up : present = this library holds an application
    static constexpr const char *entry = "getAppModule";

    /// Ce que type() doit rendre.
    static constexpr const char *contract = "game";

    /** @brief Aucune sous-famille pour l'instant. */
    static constexpr const char *accepts[] = {"game", nullptr};

    virtual ~IAppModule() = default;

    /**
     * @brief Builds the application on top of everything already loaded.
     *
     * A LIVE registry, not a snapshot : the application re-reads it whenever
     * it needs to, so a vendor swapped at runtime is seen. Handing a
     * std::vector instead would freeze the state at creation, and the
     * pointers in it would dangle the moment a library is unloaded.
     *
     * The registry rather than the manager itself, because ModuleManager is
     * a template and a dll compiled apart cannot name it.
     *
     * Passed at construction, like an ITexture is to an ISprite : the link
     * can never be missing.
     *
     * @param modules
     * @return IApp*
     */
    virtual IApp *createApp(IModuleRegistry &modules) = 0;

    /**
     * @brief Destroys it. The dll that allocated it is the one that frees it.
     *
     * @param app
     */
    virtual void deleteApp(IApp *app) = 0;
};

/** @} */

#endif /* !IAPP_MODULE_HPP */
