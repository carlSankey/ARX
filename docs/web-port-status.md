# ARX Web Port – Status Report

**Updated:** 24 October 2026
**Branch:** `gameplay-wiring` (latest commit `8f44a74`)
**Goal:** get the SDL2 web build fully playable, so it becomes a solid base for the Android port.

> **Starting a new Claude conversation?** Say: *"Read `docs/web-port-status.md` on the `gameplay-wiring` branch of github.com/carlSankey/ARX, then…"*. That gives the full context cheaply.

---

## Where we are

The web build is **playable from start to finish** in the City and the Dungeon. You can:

- create a character, or resume a saved one
- explore, with smooth walking and see-through arches
- fight animated monsters, with random encounters about every 30 seconds
- shop in stocked taverns, smithies, shops and the bank, with their pictures
- get, drop and use items, and cast spells
- save, and die (death screen, then back to the title)

The same SDL2 code will be used for Android, so every fix here carries straight over.

Still to do: a few small fixes (see `docs/cline/05-small-fixes.md`), touch controls, and the Android project itself. See **Next steps** below.

---

## How the code is organised

| Branch | What it holds |
|---|---|
| `master` | The original Windows/SFML game (reference for how things *should* behave) |
| `arxweb` | First SDL2/WebGL web port |
| `desktop-build` | CMake build, OpenGL ES 3.0 context, GL loader |
| `file-access` | `arx::fs` helper so data files can be read from inside an Android APK |
| **`gameplay-wiring`** | **Current work: everything below** |

### Building the web version (Windows)

```powershell
cd C:\Source\ARXX
.\web\build.cmd
.\web\restartweb.cmd        # serves build-web on http://localhost:8080
```

Then open the game in the browser and press **Ctrl+F5**.

**Check you're running the right build:** the title screen's bottom line shows `Build <commit> <date time>`. It should match `git log --oneline -1`.

### Debugging

- The game's console log (`std::cout`) is **off** in the web build, for speed. Build with `-DARX_LOGS=1` to turn it back on. Errors (`std::cerr`) always show.
- The per-frame renderer logs are behind `-DARX_FRAME_LOGS=1`.
- The web build **cannot catch C++ exceptions**: any `throw` stops the game with "Exception thrown, see JavaScript console". Don't add `throw` to game code.

### Tuning (top of `gameTick()` in `game.cpp`)

| Constant | Default | What it does |
|---|---|---|
| `kEncounterCheckSeconds` | 15 | How often a random encounter is rolled (about 45% of rolls start one) |
| `kGameMinuteSeconds` | 4.8 | Real seconds per game minute (as the original) |
| `kWalkStepsPerSecond` | 60 | Walking speed while a key is held (20 steps = 1 square) |

---

## What works now

### Title screen
- **(1)/(2)** create a City or Dungeon character. New characters start at the Floating Gate, or by the Dungeon's City entrance, as in the original.
- **(3)** Resume a character: the ten save slots
- **(5)/(6)/(7)** graphics, audio and font options

### Exploring
- Arrow keys or **I/J/K/L**. Hold to walk smoothly, tap for one step.
- **W** wait for an encounter; **A** mini map; **M** automap; **P** pause
- **F1–F8**, **Space**, **,** and **.** switch the info panels
- **ESC** opens Options: **S** save, **Q** quit to main menu
- The game clock, day and night, and random encounters (none while you're in a shop or menu)

### Encounters
- Monsters chosen from the original tables, with animated pictures
- Battle Options (Attack, Charge, Aimed Attack, Transact, Switch weapon, Run)

### Shops and locations
- Taverns, shops, smithies, the bank, inns, healers, guilds, the chapel, fountains, lifts and stairs
- Stock at game start (from `items.csv`), opening hours and location pictures
- No random encounters while inside

### Items and spells
- **G** Get, **D** Drop (every page), **U** Use, **C** Cast ("Thou knowest no spells!" if none)

### Saving and loading
- Saves go to the user folder: `%APPDATA%\ARXProject\ARX\saves` on Windows, the app's private storage on Android. On the web they're kept in the browser (IndexedDB) and survive a refresh.
- The old bundled saves in `data/saves` (Dave, Max, CityPete…) still show until you save over them.

### Death
- HP below zero: "You're DEAD." with the death song and lyrics (F1 replays), then back to the title. Save slots are kept.

---

## What was fixed (gameplay-wiring branch)

### 25 Sep – 17 Oct
| Commit | Fix |
|---|---|
| `153e370` | Asyncify, so the original blocking shop and encounter loops don't freeze the browser |
| `f3653fb` | Game loop wiring: square detection (shops, stairs…), W, timed encounters; built-in SHA-256; screen clearing |
| `2c534c7` | CRLF line endings in the CSV readers |
| `5424a3c` | Tamper protection removed (F.B.I. Agent, 65536 weight, dead spells) |
| `06df95a` | Drop menu pages 2–4 looping; menu flicker |
| `f538b2d` | Every key press handled twice |
| `4ee0283` | Cast menu, plus a "no spells" message |
| `86c7096` | ESC options (it silently stopped shops and encounters), U, M, P, I/J/K/L, F1–F8 |
| `e466e63` | Monster pictures |
| `296a3fd` | Shop and location pictures |
| `b1de3c9` | Pack all images in the web build; title-screen build stamp |

### 22–24 Oct
| Commit | Fix |
|---|---|
| `25bd9be` | **Shop stock** at game start. **Per-frame shops** (bank, smithy, shop, inn, healer, chapel, fountain, lift, stairs) no longer fight the game loop: the smithy looked like it threw you out, there were stray digits on the stats bar, and encounters could happen mid-purchase |
| `801f793` | **Fewer encounters** (every 15 s instead of 4.8 s); none straight after leaving a shop; **smooth, time-based walking**; per-frame logging off |
| `44901f8` | **Console log silenced** in the web build (`-DARX_LOGS=1` to restore) |
| `04dfbff` | **Saving and loading:** user folder, browser storage, "Resume a character". Also fixed the typed name being replaced with "Xebec", and the automap not being restored on load |
| `1f92c1d` | **Death:** death screen, music and lyrics, then back to the title. Quit to main menu now works |
| `18fb31a` | **Web crashes ("Exception thrown"):** a monster snatching when you have nothing threw an exception. Also the lyrics "EOF" loop with CRLF files, the map "EOF" loops, and `stringToBool` on save data |
| `7e87019` | **See-through arches** (the shader forced every pixel solid). **Correct start squares** (the City started on a Main Street; the Dungeon started inside the Wizards of Chaos guild) |
| `8f44a74` | **Black boxes behind Dungeon text:** the arch drawing switched off alpha blending for everything drawn afterwards |

---

## Known issues

- **Stun counter:** `encounter.cpp` ~line 932 uses `=` instead of `==` (fix 1 in `docs/cline/05-small-fixes.md`)
- **Automap legend** cut off at the right edge (fix 2)
- **Most music** isn't packed into the web build (fix 3)
- **Debug exports** in `actor.cpp` and `level.cpp` write into `data/`, which will fail on Android (fix 4)
- **Zones:** the original game loop updated `plyr.zone` every frame. This isn't done yet, because the start-up code forces zone 99. It may affect Dungeon lighting and zone-specific encounters. Needs investigation.
- **Graphics mode "Small 3D"** asks for `encounters_alternate.png`, which isn't in the repo, so it uses the 16-bit monster sheet
- **Missing texture files** reported at start-up: `Scenario_N/sunrise*.png`, `textures_original/Goblinwall.png` and `Goblindoor.png`
- **`spells.csv` hashes** never matched, even on `master`. Harmless now that the check is removed.
- **Shop duplicates:** a shop can list the same item twice. This is original behaviour (there's a comment in the code about it).
- The `loadDamonBinary`, `loadCitySmithyBinary` and `loadDwarvenBinary` loaders have broken open checks and never loaded anything, even in the original build. Stock comes from `items.csv`. They're deliberately not called.

---

## Next steps

1. **Small fixes:** `docs/cline/05-small-fixes.md`, written for Cline or a free model
2. **Touch controls:** on-screen buttons that push the key names the game already uses (`arx::inputPush("D")` and so on). The left side has movement and the map. The right side has Get, Drop, Use, Cast, Wait and Menu. A bottom row has 1–4, Yes/No, Forward/Back and ESC. Use the Android number keyboard for quantities and names. Draw it from `updateDisplay()` so it appears on every screen, including blocking shops. Test it with a mouse in the web build first.
3. **Android project:** Gradle + NDK based on SDL2's `android-project`; the game built as `libmain.so` from `cmake/ArxSources.cmake`; `data/` packed as APK assets (reads already go through `arx::fs` / SDL); GLES3; landscape only.

---

## Working with patches (notes for Carl)

```powershell
cd C:\Source\ARXX
Copy-Item "$env:USERPROFILE\Downloads\<name>.patch" .
git am <name>.patch
git push
.\web\build.cmd
```

- Always `cd C:\Source\ARXX` first. A new PowerShell window starts in `system32`.
- If `git am` fails, run `git am --abort` straight away. Otherwise every later patch is refused.
- **"does not match index"** means you have uncommitted local changes in that file. Run `git stash` to set them aside, then `git am` again.
- If it says **"patch does not apply"** for a patch you've already applied, run `git am --skip`.
- Use `$env:USERPROFILE` in PowerShell. `%USERPROFILE%` only works in the old Command Prompt.
- After building, check the title-screen build stamp matches `git log --oneline -1`.
- Old `.patch` files can be deleted (`Remove-Item arx-*.patch`). They're all in the git history.
