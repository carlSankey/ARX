#include "FileSystem.h"

#include <SDL.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

#include <algorithm>
#include <cstdio>
#include <iterator>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace arx {
namespace fs {

namespace {

// SDL_RWops for reading, opened through SDL so the same code can see files
// packed inside an Android APK. Returns false (after logging) on failure.
SDL_RWops* openForRead(const std::string& path) {
    SDL_RWops* rw = SDL_RWFromFile(path.c_str(), "rb");
    if (!rw) {
        std::cerr << "arx::fs: could not open '" << path
                  << "' for reading: " << SDL_GetError() << std::endl;
        return nullptr;
    }
    return rw;
}

// Read the whole stream into a byte buffer. Returns false on a short read
// (the contents read so far are left in 'out' so callers can decide).
bool readAll(SDL_RWops* rw, std::vector<uint8_t>& out) {
    Sint64 size = SDL_RWsize(rw);
    if (size < 0) {
        // Unknown size (e.g. a pipe): read in chunks until EOF.
        uint8_t chunk[4096];
        for (;;) {
            size_t got = SDL_RWread(rw, chunk, 1, sizeof(chunk));
            if (got == 0) {
                break;
            }
            out.insert(out.end(), chunk, chunk + got);
            if (got < sizeof(chunk)) {
                break;
            }
        }
        return true;
    }

    out.resize(static_cast<size_t>(size));
    if (size == 0) {
        return true;
    }
    size_t got = SDL_RWread(rw, out.data(), 1, static_cast<size_t>(size));
    if (got != static_cast<size_t>(size)) {
        out.resize(got);
        return false;
    }
    return true;
}

// Cached result of SDL_GetPrefPath. The string SDL hands back is allocated
// with SDL_malloc, so it is copied into a std::string once and the original
// is released with SDL_free.
const std::string& prefPath() {
    static std::string cached;
    static bool lookedUp = false;
    if (!lookedUp) {
        lookedUp = true;
        char* p = SDL_GetPrefPath("ARXProject", "ARX");
        if (p) {
            cached = p;
            SDL_free(p);
        } else {
            std::cerr << "arx::fs: SDL_GetPrefPath failed: " << SDL_GetError()
                      << std::endl;
        }
    }
    return cached;
}

// Join a relative user path onto the pref path and make sure the folder
// exists, so callers can just write to the result.
std::string prepareUserFile(const std::string& relative) {
    std::string base = prefPath();
    if (base.empty()) {
        return std::string();
    }
    if (base.back() != '/' && base.back() != '\\') {
        base += '/';
    }

    std::error_code ec;
    std::filesystem::path target = std::filesystem::path(base) / relative;
    std::filesystem::path folder = target.parent_path();
    if (!folder.empty()) {
        std::filesystem::create_directories(folder, ec);
        if (ec) {
            std::cerr << "arx::fs: could not create folder '"
                      << folder.string() << "': " << ec.message() << std::endl;
            return std::string();
        }
    }
    return target.string();
}

} // namespace

bool readText(const std::string& path, std::string& out) {
    std::vector<uint8_t> bytes;
    SDL_RWops* rw = openForRead(path);
    if (!rw) {
        return false;
    }
    bool ok = readAll(rw, bytes);
    SDL_RWclose(rw);
    if (!ok) {
        std::cerr << "arx::fs: short read on '" << path << "'" << std::endl;
    }

    // Byte-for-byte: no newline translation, so '\r' survives for parsers that
    // see it. Opening succeeded, so this reports true even on a short read -
    // the same way the ifstream it replaces would have.
    out.assign(bytes.begin(), bytes.end());
    return true;
}

bool readBinary(const std::string& path, std::vector<uint8_t>& out) {
    SDL_RWops* rw = openForRead(path);
    if (!rw) {
        return false;
    }
    bool ok = readAll(rw, out);
    SDL_RWclose(rw);
    if (!ok) {
        std::cerr << "arx::fs: short read on '" << path << "'" << std::endl;
    }
    return ok;
}

std::istringstream openText(const std::string& path, bool& ok) {
    std::string text;
    ok = readText(path, text);
    return std::istringstream(text);
}

std::string userPath(const std::string& relative) {
    return prepareUserFile(relative);
}

bool writeTextAtomic(const std::string& userRelative, const std::string& content) {
    std::string target = prepareUserFile(userRelative);
    if (target.empty()) {
        return false;
    }

    // Write to "<file>.tmp" first, then rename over the original, so a phone
    // being killed mid-save can't corrupt the real file.
    const std::string tmp = target + ".tmp";
    {
        std::ofstream out(tmp, std::ios::binary | std::ios::trunc);
        if (!out) {
            std::cerr << "arx::fs: could not open '" << tmp
                      << "' for writing" << std::endl;
            return false;
        }
        out.write(content.data(), static_cast<std::streamsize>(content.size()));
        out.flush();
        if (!out) {
            std::cerr << "arx::fs: failed while writing '" << tmp << "'"
                      << std::endl;
            return false;
        }
    }

    std::error_code ec;
    std::filesystem::rename(tmp, target, ec);
    if (ec) {
        std::cerr << "arx::fs: could not rename '" << tmp << "' to '" << target
                  << "': " << ec.message() << std::endl;
        return false;
    }
    return true;
}

bool readUserText(const std::string& userRelative, std::string& out) {
    std::string path = prefPath();
    if (path.empty()) {
        return false;
    }
    path += userRelative;
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        return false;   // not saved yet - not an error
    }
    out.assign(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
    return true;
}

bool readUserOrBundled(const std::string& userRelative, std::string& out) {
    bool ok = readUserText(userRelative, out);
    if (!ok) {
        ok = readText("data/" + userRelative, out);
    }
    if (ok) {
        out.erase(std::remove(out.begin(), out.end(), '\r'), out.end());
    }
    return ok;
}

#ifdef __EMSCRIPTEN__
// SDL's user folder on the web is /libsdl/<org>/<app>/ in Emscripten's
// in-memory file system, which is wiped on every page load. Mount IndexedDB
// (IDBFS) there and copy it in before the game starts.
void initPersistentStorage() {
    static bool done = false;
    if (done) {
        return;
    }
    done = true;
    EM_ASM({
        Module.arxFsReady = 0;
        try {
            if (!FS.analyzePath('/libsdl').exists) FS.mkdir('/libsdl');
            FS.mount(IDBFS, {}, '/libsdl');
            FS.syncfs(true, function(err) {
                if (err) console.error('ARX: loading saves from browser storage failed', err);
                Module.arxFsReady = 1;
            });
        } catch (e) {
            console.error('ARX: browser storage unavailable', e);
            Module.arxFsReady = 1;
        }
    });
    // Wait (up to ~5s) for IndexedDB to load. Needs -sASYNCIFY.
    for (int i = 0; i < 500; ++i) {
        if (EM_ASM_INT({ return Module.arxFsReady; })) {
            break;
        }
        emscripten_sleep(10);
    }
}

void flushPersistentStorage() {
    EM_ASM({
        FS.syncfs(false, function(err) {
            if (err) console.error('ARX: saving to browser storage failed', err);
        });
    });
}
#else
void initPersistentStorage() {}
void flushPersistentStorage() {}
#endif

} // namespace fs
} // namespace arx
