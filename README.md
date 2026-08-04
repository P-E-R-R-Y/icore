# icore

**Le socle d'un programme.** Trois types, un fichier, aucune dépendance.

| type | ce qu'il dit | tu écris |
|---|---|---|
| `ICore` | ça se lance et ça rend un code | constructeur, destructeur, **`run()`** |
| `ITickable` | ça se fait ticker par quelqu'un | **`event`** / **`update`** / **`display`** |
| `IApp` | les deux : possède une boucle et y participe | constructeur, destructeur, les trois étapes |

`run()` et `event/update/display` sont **deux rôles différents** : posséder une
boucle, ou participer à celle de quelqu'un d'autre. Une CLI n'a que le premier,
un plugin chargé à chaud n'aura que le second.

## Une action — `ICore`

```cpp
class MyCli final : public ICore {
public:
    MyCli(int argc, char **argv) { ... }   // l'init EST le constructeur
    ~MyCli() override { ... }              // le nettoyage, le destructeur
    int run() override { ...; return 0; }
};

int main(int argc, char **argv) { return launch<MyCli>(argc, argv); }
```

## Une boucle — `IApp`

`run()` est **déjà écrit**, et `final` : le cycle est identique d'une
application à l'autre.

```cpp
int IApp::run() final {
    _running = true;
    while (_running) { event(); update(); display(); }
    return _exitCode;
}
```

```cpp
class MyApp final : public IApp {
public:
    explicit MyApp(unsigned long frames) : _frames(frames) {}
    ~MyApp() override { ... }
protected:
    void event() override   {}
    void update() override  { if (_frame >= _frames) stop(); }
    void display() override { std::printf("frame %lu\n", _frame++); }
};

int main() { return launch<MyApp>(3); }
```

## `launch<T>(args...)`

Construit puis lance. Les arguments partent **tels quels** au constructeur —
`argc/argv`, une config, ou rien. Le `main` est identique pour une action ou
une boucle, et l'objet est détruit **avant** que le code de sortie remonte.

```cpp
template <typename T, typename... Args>
int launch(Args &&...args) { return T(std::forward<Args>(args)...).run(); }
```

## L'API de `IApp`

| membre | rôle |
|---|---|
| `run()` | la boucle — `final`, on ne la redéfinit pas |
| `stop(code = 0)` | demande l'arrêt ; la frame en cours se termine normalement |
| `running()` | la boucle tourne-t-elle |

`IApp` hérite de `ITickable` en **`protected`** : les trois étapes sont
appelées par sa propre boucle, et personne d'autre ne peut ticker une
application à contretemps.

## Ce qui n'est pas dedans, et pourquoi

- **`init()` / `destroy()`** : ce sont le constructeur et le destructeur. Un
  objet qui existe est un objet prêt ; ça supprime l'état « construit mais pas
  initialisé », source de la plupart des bugs de cycle de vie.
- **Un compteur de frames.** Une boucle n'a pas d'opinion sur ce qu'elle
  compte : un serveur s'en moque. Celui qui en veut un le tient lui-même,
  comme dans l'exemple.
- **Le cadencement** (sleep, delta time, frame limiter). La politique de temps
  dépend de l'application : un jeu veut 60 fps, un serveur veut bloquer sur le
  réseau, un test veut tourner à fond. En l'état la boucle tourne aussi vite
  que possible — c'est à `event()` de la cadencer.
- **Les modules, contrats, slots, chargement dynamique.** Ils viendront se
  brancher sur cette boucle, dans d'autres dépôts. Jamais l'inverse.

## Construire les exemples

```sh
cmake -B build -S . && cmake --build build
./build/icore_cli hello world
./build/icore_app 2
```
