# icore — journal

Marqueurs : 🟢 ajout · 🔴 rupture · 🔵 correctif · ⚪ interne ou doc · 🟡 propose
dans le plan, code non ecrit.

## v0.1.0

- 🟢 `ICore`, `IApp`, `IAppModule`, `ITickable`
- 🟢 `launch<T>()` construit, `run()`, detruit **avant** de rendre le
  code de sortie, pour que les dll se ferment proprement
- 🟢 26 tests, dont la trace d'ordre et `LaunchDestroysBeforeReturning`
- ⚪ le tag `v1` (non-semver) est supprime, remplace par `v0.1.0` :
  aucune release GitHub, aucun consommateur ne le nommait — dernier
  moment ou ce geste etait gratuit
