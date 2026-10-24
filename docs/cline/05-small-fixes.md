# ARX – Step 5: Small fixes (web build)

## Background

Branch: `gameplay-wiring`. The web build is playable. See `docs/web-port-status.md` for the current state.

This brief has **four small, independent fixes**.

Rules for all four:
- Make one commit per fix, using the commit message given.
- Change **only** the lines described. Don't reformat or tidy anything else.
- Don't edit anything under `data/`.
- After each fix, build with `.\web\build.cmd`. It must finish without errors. Warnings are fine.
- If something doesn't match what this brief describes (wrong line, different code), **stop and report** instead of guessing.

---

## Fix 1 – Stun counter bug (`encounter.cpp`)

Around **line 932**, in the `encounterMenu == 11` ("You are stunned") block:

```cpp
if (curOpponent = encounterQuantity) { stunnedTurnsRemaining--; } /* only reduce the stuuned time on last attacker */
```

This uses `=` (assignment) instead of a comparison. As written, the condition is almost always true, and it also overwrites `curOpponent`.

1. Look at how `curOpponent` is used elsewhere in `encounter.cpp` (for example `updateOpponents()` and `processOpponentAction()`). Work out whether it counts **0 to encounterQuantity-1** or **1 to encounterQuantity**.
2. Change the condition to compare against the **last** opponent:
   - if it's 0-based: `if (curOpponent == encounterQuantity - 1)`
   - if it's 1-based: `if (curOpponent == encounterQuantity)`
3. In your summary, say which one you chose and why.

Commit message: `Fix stun counter: compare curOpponent instead of assigning it`

---

## Fix 2 – Automap legend off the right edge (`automap.cpp`)

Around **line 259**:

```cpp
s_mapLegend.draw(512.0f + 16.0f, 16.0f);
```

The legend image is 128 px wide, and the window is 640 px wide (`windowWidth`). Drawing it at x = 528 means the right 16 px are cut off.

Change it so the legend's right edge lines up with the window's right edge:

```cpp
s_mapLegend.draw(static_cast<float>(windowWidth - s_mapLegend.getWidth()), 16.0f);
```

If `windowWidth` isn't visible in `automap.cpp`, add `extern int windowWidth;` near the top. Check first how other files declare it.

Commit message: `Automap: keep the legend inside the window`

---

## Fix 3 – Pack the game's music and sounds (`web/build.cmd`)

Only a few audio files are packed into the web build at the moment: the gate sounds and the death song. All of `data/audio` is 53 MB, which is too big to pack whole, so **pack only the files the code uses**.

1. Find every audio file the code refers to. In PowerShell, from the repo root:
   ```powershell
   Select-String -Path *.cpp -Pattern '"data/audio/[^"]*"' -AllMatches | ForEach-Object { $_.Matches.Value } | Sort-Object -Unique
   ```
   Also include:
   - the `B\` version of each music file, if it exists (`data\audio\B\...`, the alternate soundtrack)
   - the matching lyrics `.txt` file for each song, if one exists (for example `tavern.ogg` and `tavern.txt`)
2. In `web/build.cmd`, add one `--preload-file "%REPO_ROOT%\data\audio\<file>@data/audio/<file>"` for each file that **exists on disk**, in the same style as the existing audio lines. Don't add duplicates of lines that are already there.
3. Build, then report:
   - the list of files you added
   - the size of `build-web\arxx_web_v2.data` before and after

   If the `.data` file is over **100 MB**, stop and report before committing.

Commit message: `Web build: pack the music and sound files the game uses`

---

## Fix 4 – Debug file writes that would fail on Android

On Android the `data/` folder is read-only (it's inside the APK). Two developer-only debug exports write into `data/`:

- `actor.cpp` around **line 195**: `outdata.open(filename.c_str());` and the code that writes to `outdata` after it.
- `level.cpp` around **line 613**: `outdata.open("data/images/Scenario_" ...`, which writes a map text export, and the code that writes to `outdata` after it.

For each one, wrap **the whole block** (from the `open` up to and including the matching `close()`) in:

```cpp
#ifndef __ANDROID__
   ... existing code unchanged ...
#endif
```

Don't change what they do on Windows or the web.

Commit message: `Skip debug data exports on Android (data/ is read-only there)`

---

## Test checklist (Carl)

After all four fixes, run `git push` and `.\web\build.cmd`, restart the server, press Ctrl+F5, and check the title-screen build stamp. Then check:

- **Stun:** in a long fight, if you get stunned, it wears off after a few turns rather than lasting the whole fight or ending at once.
- **Automap:** press M. The legend's right edge is fully visible.
- **Music:** walk into a tavern, guild or chapel. Music plays where the original had it. Press F1 in a tavern to see the lyrics.
- **Everything else:** W encounters, shops, saving and loading still work.
