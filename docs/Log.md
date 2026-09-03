# icore — changelog

Markers: 🟢 added · 🔴 breaking · 🔵 fix · ⚪ internal or docs · 🟡 proposed
in the plan, no code written yet.

## v0.1.0

- 🟢 `ICore`, `IApp`, `IAppModule`, `ITickable`
- 🟢 `launch<T>()` builds, `run()`s, destroys **before** returning the
  exit code, so dlls close cleanly
- 🟢 26 tests, including the ordering trace and
  `LaunchDestroysBeforeReturning`
- ⚪ the non-semver `v1` tag is deleted, replaced with `v0.1.0` — no
  GitHub release referenced it, no consumer pinned it — last moment this
  was free
