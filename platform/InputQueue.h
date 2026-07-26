#pragma once

// InputQueue - platform-neutral keyboard event queue.
//
// The SFML/SDL event pump pushes string keys into this queue each frame.
// Game code calls inputPoll() to consume one key per call (returns "" if empty).
//
// This replaces the pattern:
//   while (key != "SPACE") { key = getSingleKey(); }   <- blocks on native, CRASHES on web
// with:
//   key = inputPoll();   // returns "" if nothing queued, caller returns to frame loop
//
// On native the old getSingleKey() still works (it polls the queue too), so
// existing blocking loops continue to function correctly during the port.

#include <string>

namespace arx {

// --- Key event queue (arrow keys, letters, digits, F-keys, etc.) ---

// Push a key string onto the back of the queue.
// Called by readKey() / SDL event handler each frame.
void inputPush(const std::string& key);

// Pop and return the front key, or "" if the queue is empty.
std::string inputPoll();

// Return the front key WITHOUT removing it, or "" if empty.
std::string inputPeek();

// Discard all queued keys.
void inputClear();

// --- Text input queue (character-by-character from TextEntered / SDL_TEXTINPUT) ---

// Push a single character as a string onto the text queue.
// Called by getTextChar() event handlers.
void textPush(const std::string& ch);

// Pop and return the front text character, or "" if the queue is empty.
std::string textPoll();

// Discard all queued text characters.
void textClear();

} // namespace arx
