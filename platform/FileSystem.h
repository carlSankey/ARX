#pragma once

// Android-safe file access for ARX.
//
// On Android the data/ folder is packed inside the APK, which is read-only and
// invisible to std::ifstream / fopen. SDL's own file functions (SDL_RWFromFile)
// can read it, and SDL_image / SDL_mixer already use them, so images and sounds
// work as-is. This helper routes every other read and write through SDL so the
// same code builds and runs on desktop, the web and Android.
//
// Reads are byte-for-byte faithful: no newline translation, so files with
// Windows line endings parse exactly as they do today.

#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

namespace arx {
namespace fs {

// ---- Read-only game data (data/...) ----
// Uses SDL_RWFromFile, so it works on desktop, web and inside an Android APK.
// 'path' is the same relative path the game uses today, e.g.
// "data/map/core/items.txt".
bool readText(const std::string& path, std::string& out);
bool readBinary(const std::string& path, std::vector<uint8_t>& out);

// Convenience for existing line-by-line parsing code: returns a stream over
// the file contents. ok is set to false if the file couldn't be read.
std::istringstream openText(const std::string& path, bool& ok);

// ---- Writable user files (saves, settings) ----
// Returns an absolute path inside SDL_GetPrefPath("ARXProject", "ARX"),
// creating any sub-folders, e.g. userPath("saves/save1.txt").
// Windows: %APPDATA%\ARXProject\ARX\...   Android: the app's private storage.
std::string userPath(const std::string& relative);

// Write a whole file safely: write to "<file>.tmp", then rename over the
// original, so a phone being killed mid-save can't corrupt it.
bool writeTextAtomic(const std::string& userRelative, const std::string& content);

} // namespace fs
} // namespace arx
