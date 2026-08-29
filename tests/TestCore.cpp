/**
 * @file TestCore.cpp
 * @brief Le socle : une action, une boucle, et une boucle qu'on peut ticker.
 *
 * Ces trois choses tiennent en cent cinquante lignes d'en-tete, et deux de
 * leurs proprietes ne se voient qu'a l'usage : que ITickable soit PUBLIC,
 * et que _running soit vrai DES LA CONSTRUCTION. Les deux sont ce qui rend
 * l'hebergement possible, et les deux se cassent sans bruit.
 */

#include <gtest/gtest.h>

#include "DummyApp.hpp"
#include "DummyAppModule.hpp"
#include "DummyCli.hpp"
#include "DummyRegistryStub.hpp"

/* ---- ICore : une action ------------------------------------------- */

TEST(Core, RunReturnsItsExitCode)
{
    std::string log;
    DummyCli cli(42, log);

    EXPECT_EQ(cli.run(), 42);
}

/**
 * @brief launch() construit, lance, et DETRUIT avant de rendre le code.
 *
 * Le destructeur a fini de tourner quand le code de sortie remonte au
 * systeme : c'est ce qui permet de mettre le nettoyage dans le destructeur
 * plutot que dans un destroy() qu'on oublierait d'appeler.
 */
TEST(Core, LaunchDestroysBeforeReturning)
{
    std::string log;

    EXPECT_EQ(launch<DummyCli>(3, log), 3);
    EXPECT_EQ(log, "ctor run dtor ");
}

TEST(Core, LaunchWorksForALoopToo)
{
    bool alive = false;

    EXPECT_EQ(launch<TrackedApp>(alive), 7);
    EXPECT_FALSE(alive);   //detruite avant le retour
}

/* ---- IApp : la boucle --------------------------------------------- */

/**
 * @brief L'ordre est event, update, display. Toujours, et sans exception.
 *
 * C'est ce qui rend le cycle previsible d'une application a l'autre, et
 * c'est pour ca que run() est final : un jeu ne peut pas le reordonner.
 */
TEST(App, RunsEventUpdateDisplayInOrder)
{
    DummyApp app(3);

    EXPECT_EQ(app.run(), 0);
    EXPECT_EQ(app.trace, "eudeudeud");
    EXPECT_EQ(app.turns, 3);
}

TEST(App, StopFinishesTheCurrentFrame)
{
    DummyApp app(1);

    app.run();

    /* stop() est appele DANS update() : display() doit quand meme avoir
     * lieu. Une frame commencee se termine. */
    EXPECT_EQ(app.trace, "eud");
}

TEST(App, StopCarriesTheExitCode)
{
    DummyApp app(2, 84);

    EXPECT_EQ(app.run(), 84);
}

/**
 * @brief running() est vrai DES LA CONSTRUCTION, pas seulement dans run().
 *
 * Une IApp hebergee ne passe jamais par run() : c'est son hote qui appelle
 * event/update/display. Si le drapeau n'etait leve que dans run(), elle
 * naitrait "deja arretee" et l'hote la fermerait au tick suivant.
 */
TEST(App, IsRunningBeforeRunIsEverCalled)
{
    DummyApp app;

    EXPECT_TRUE(app.running());
}

TEST(App, StopIsVisibleFromOutside)
{
    DummyApp app;

    EXPECT_TRUE(app.running());
    app.stop(5);
    EXPECT_FALSE(app.running());
}

/* ---- ITickable : la boucle empruntee ------------------------------- */

/**
 * @brief Un hote peut ticker une IApp sans jamais appeler run().
 *
 * ITickable est PUBLIC, et c'est la seule raison pour laquelle une borne
 * peut heberger un jeu. En protege, elle ne pourrait qu'appeler run() - qui
 * ne rend jamais la main - et perdrait le controle jusqu'a ce que le jeu
 * decide de sortir.
 */
TEST(Hosted, AHostCanDriveItStepByStep)
{
    DummyApp app(0);
    ITickable &guest = app;   //ne compile que si l'heritage est public

    for (int frame = 0; frame < 2; frame++) {
        guest.event();
        guest.update();
        guest.display();
    }

    EXPECT_EQ(app.trace, "eudeud");
    EXPECT_TRUE(app.running());   //personne ne lui a demande de s'arreter
}

/**
 * @brief Un jeu qui s'arrete rend la main a son hote, il ne le ferme pas.
 *
 * L'hote lit running() apres l'avoir ticke ; c'est tout le protocole.
 */
TEST(Hosted, TheGuestAsksToStopWithoutClosingTheHost)
{
    DummyApp app(1);

    app.event();
    app.update();
    app.display();

    EXPECT_FALSE(app.running());
    EXPECT_EQ(app.trace, "eud");
}

/* ---- IAppModule : le jeu comme module ------------------------------ */

TEST(AppModule, AnnouncesTheGameContract)
{
    DummyAppModule module;

    EXPECT_STREQ(module.type(), IAppModule::contract);
    EXPECT_STREQ(IAppModule::contract, "game");
    EXPECT_STREQ(IAppModule::entry, "getAppModule");
}

/**
 * @brief On lui passe le REGISTRE, pas une fenetre ni un vendor.
 *
 * C'est ce qui permet au jeu de suivre une bascule de bibliotheque sans que
 * la borne ait a le prevenir : il va chercher lui-meme ce qui est en
 * service, a chaque tour.
 */
TEST(AppModule, HandsTheGameTheRegistry)
{
    DummyAppModule module;
    DummyRegistryStub registry;

    IApp *app = module.createApp(registry);

    ASSERT_NE(app, nullptr);
    EXPECT_EQ(module.received, &registry);
    module.deleteApp(app);
}

/**
 * @brief createApp et deleteApp vont par paire, et par la MEME dll.
 *
 * L'application est allouee par la bibliotheque du jeu et sa vtable y vit :
 * la detruire ailleurs sauterait dans du code qui peut avoir ete demappe.
 */
TEST(AppModule, CreateAndDeletePairUp)
{
    DummyAppModule module;
    DummyRegistryStub registry;

    for (int i = 0; i < 3; i++)
        module.deleteApp(module.createApp(registry));

    EXPECT_EQ(module.made, 3);
    EXPECT_EQ(module.freed, 3);
}
