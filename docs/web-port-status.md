# ARX Web Port – Status Report

**Updated:** 17 October 2026
**Branch:** `gameplay-wiring` (latest commit `b1de3c9`)
**Goal:** get the SDL2 web build fully playable, so it becomes a solid base for the Android port.

---

## Where we are

The web build is now **playable in the City**. You can create a character, walk around, and do the following:

- fight monsters, with their pictures showing
- enter taverns and shops
- pick up, drop and use items
- cast spells
- open the options and automap screens

The same SDL2 code will be used for Android, so every fix here carries straight over.

The remaining gaps are:

- shop stock at game start
- saving and loading
- death handling
- touch controls

These are listed under **Next steps** below.

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

**Check you're running the right build:** the title screen's bottom line shows `Build <commit> <date time>`. It should match `git log --oneline -1`. If it shows `+changes`, the build included uncommitted edits.

---

## What works now

### Exploring
- City movement with the arrow keys or **I/J/K/L**, including doors (smooth scrolling, so press forward until you're through)
- Game clock advances while you explore
- Random encounters every few seconds, as in the original
- **W** waits for (starts) an encounter
- **A** toggles the mini map; **M** opens the full automap
- **F1–F8**, **Space**, **,** and **.** switch the info panels
- **P** pauses
- **ESC** opens the Options screen (Save, Quit to main menu, key list)

### Encounters
- Monsters are chosen from the original day and night tables. The F.B.I. Agent no longer appears every time.
- Animated monster pictures are drawn in the 3D view
- The Battle Options menu works (Attack, Charge, Aimed Attack, Transact, Switch weapon, Run)
- Opponents act on their own timer, as in the original

### Shops and locations
- Walking into a tavern, shop, bank, smithy, inn, healer, guild and so on opens it
- Opening hours are respected (for example, "Sorry, we are closed… open from 16:00")
- Location pictures are shown above the menus
- Leaving puts you back outside the door

### Items and spells
- **G** (Get) offers each item on the floor in turn: Yes, No or ESC
- **D** (Drop) works on every page: food, water, torches and timepieces; compasses, keys, crystals and gems; coins; and carried items
- **U** opens the original Use menu
- **C** opens the original Cast menu, or says *"Thou knowest no spells!"* if you know none

---

## What was fixed (gameplay-wiring branch)

| Commit | Fix |
|---|---|
| `153e370` | **Asyncify** in the web build, so the original blocking shop and encounter loops no longer freeze the browser tab |
| `f3653fb` | **Game loop wiring:** the web loop never worked out which square you were on, so shops, taverns, stairs and fountains never triggered. It also had no W key and no timed random encounters. Also adds a built-in SHA-256 (OpenSSL isn't available on web or Android), fixes text being drawn over the previous screen, and adds `module.cpp` and `arena.cpp` to the build |
| `2c534c7` | **CRLF line endings:** Windows checkouts gave every CSV line a hidden `\r`, so every monster and item failed its hash check (every encounter was an F.B.I. Agent, and items weighed 65536, giving "Immobilized!") |
| `5424a3c` | **Tamper protection removed:** data now ships inside the app, so the hash penalties (F.B.I. Agent, 65536 weight, zero-damage spells) only caused problems. The CSV files are unchanged |
| `06df95a` | **Drop menu:** pages 2–4 were attached to the wrong `if`, so the menu looped forever. Also stopped the menu flickering |
| `f538b2d` | **Double key presses:** every key was handled twice (Up moved two steps, one Y in Get picked up two items) |
| `4ee0283` | **Cast:** C opened an empty, undrawn menu. It now uses the original `castSpells()` screen, plus a "no spells" message |
| `86c7096` | **ESC, Use and missing keys:** ESC set the options state but nothing ran it, which silently switched off shops, encounters and the clock. Also added U (Use menu), M, P, I/J/K/L and F1–F8 |
| `e466e63` | **Monster pictures:** `drawAtariAnimation()` was an empty stub |
| `296a3fd` | **Shop pictures:** `loadShopImage()` was an empty stub |
| `b1de3c9` | **Web packaging and build stamp:** `build.cmd` only packed a hand-picked list of images, so the encounter and location pictures and the automap tiles were missing in the browser. It now packs all of `data/images`. Also adds the title-screen build stamp |

Earlier work on the branch: the `arx::fs` file helper, and map, 3D-view, monster, item and spell data reads routed through it (Android-safe).

---

## Known issues

- **Shop stock is empty or thin.** Damon, the City Smithy, the Dwarven Smithy and the tavern menus aren't stocked at game start in the web build, because the stocking calls are commented out in `platform/AppLoop.cpp`.
- **Saving and loading are untested.** Options → Save exists, but saves aren't routed to the user folder yet, and "Resume a character" on the main menu isn't implemented.
- **Death isn't handled.** If HP drops below zero, nothing calls `playerDies()`.
- **`spells.csv` hashes never matched**, even on `master`. This doesn't matter now that the check is removed, but it's worth knowing.
- **Graphics mode "Small 3D"** asks for `encounters_alternate.png`, which isn't in the repo, so it uses the 16-bit monster sheet.
- **Automap legend** runs off the right-hand edge at 640 px wide.
- **`encounter.cpp` line 932:** `if (curOpponent = encounterQuantity)` should almost certainly be `==` (the compiler warns about it). It affects how long stun lasts.
- **Zones:** the original game loop updated `plyr.zone` every frame. This isn't done yet, because the start-up code deliberately forces zone 99. It may affect dungeon lighting.
- **Most music and sound** isn't packed into the web build yet (only the gate sounds are).

---

## Next steps

### Finish the web build
1. **Shop stock:** enable the Damon, smithy, tavern and job-opening set-up at game start (file-access batch 3: `damon.cpp`, `smithy.cpp`, `dwarvenSmithy.cpp`, `lyrics.cpp`).
2. **Saving and loading:** route saves and settings through `arx::fs::userPath`, and wire up "Resume a character".
3. **Death:** call `playerDies()` and return to the main menu.
4. **Small fixes:** the automap legend, the `=`/`==` stun bug, zones, and packing the audio.

### Android
5. **Touch overlay:** on-screen buttons that push the same key names the game already uses. The left side has movement and the map. The right side has Get, Drop, Use, Cast, Wait and Menu. A bottom row has 1–4, Yes/No, Forward/Back and ESC. Android's number keyboard handles quantities and names. It can be tested with a mouse in the web build first.
6. **Android project:** Gradle + NDK based on SDL2's `android-project`, the game built as `libmain.so`, `data/` packed as APK assets, GLES3, landscape only.

---

## Working with patches (notes for Carl)

Fixes are delivered as `.patch` files and applied like this:

```powershell
cd C:\Source\ARXX
Copy-Item "$env:USERPROFILE\Downloads\<name>.patch" .
git am <name>.patch
git push
.\web\build.cmd
```

- Always `cd C:\Source\ARXX` first. A new PowerShell window starts in `system32`.
- If `git am` says **"patch does not apply"**, run `git am --abort` straight away. Otherwise git stays stuck and refuses every later patch ("previous rebase directory still exists").
- Use `$env:USERPROFILE` in PowerShell. `%USERPROFILE%` only works in the old Command Prompt.
- After building, check the title-screen build stamp matches `git log --oneline -1`.
