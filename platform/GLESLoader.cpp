// Minimal OpenGL ES 2.0 entry-point loader for the SDL2 builds.
// See platform/GLESLoader.h for why this exists.

#include <SDL.h>
#include "GLESLoader.h"

#ifdef __EMSCRIPTEN__

// Web build: the ES 2.0 entry points are provided by the Emscripten GL glue,
// so there is nothing to resolve at runtime.
namespace arx {
namespace gles {

bool loadFunctions()
{
    return true;
}

} // namespace gles
} // namespace arx

#else

#include <iostream>
#include <type_traits>

// GLESLoader.h (included above) declares the entry-point pointers inside
// arx::gles, typed with decltype of the real ES 2.0 prototypes, and then
// #defines the standard gl* names onto them. Defining each variable below from
// its own already-declared type keeps it identical to the prototype without
// repeating a single signature here; the ax_ names are untouched by the gl*
// aliases, so decltype(ax_glX) below is the real function pointer type.
namespace arx {
namespace gles {

decltype(ax_glActiveTexture)            ax_glActiveTexture = nullptr;
decltype(ax_glAttachShader)             ax_glAttachShader = nullptr;
decltype(ax_glBindBuffer)               ax_glBindBuffer = nullptr;
decltype(ax_glBindTexture)              ax_glBindTexture = nullptr;
decltype(ax_glBlendFunc)                ax_glBlendFunc = nullptr;
decltype(ax_glBufferData)               ax_glBufferData = nullptr;
decltype(ax_glClear)                    ax_glClear = nullptr;
decltype(ax_glClearColor)               ax_glClearColor = nullptr;
decltype(ax_glCompileShader)            ax_glCompileShader = nullptr;
decltype(ax_glCreateProgram)            ax_glCreateProgram = nullptr;
decltype(ax_glCreateShader)             ax_glCreateShader = nullptr;
decltype(ax_glDeleteBuffers)            ax_glDeleteBuffers = nullptr;
decltype(ax_glDeleteProgram)            ax_glDeleteProgram = nullptr;
decltype(ax_glDeleteShader)             ax_glDeleteShader = nullptr;
decltype(ax_glDeleteTextures)           ax_glDeleteTextures = nullptr;
decltype(ax_glDepthMask)                ax_glDepthMask = nullptr;
decltype(ax_glDisable)                  ax_glDisable = nullptr;
decltype(ax_glDisableVertexAttribArray) ax_glDisableVertexAttribArray = nullptr;
decltype(ax_glDrawArrays)               ax_glDrawArrays = nullptr;
decltype(ax_glEnable)                   ax_glEnable = nullptr;
decltype(ax_glEnableVertexAttribArray)  ax_glEnableVertexAttribArray = nullptr;
decltype(ax_glFlush)                    ax_glFlush = nullptr;
decltype(ax_glGenBuffers)               ax_glGenBuffers = nullptr;
decltype(ax_glGenTextures)              ax_glGenTextures = nullptr;
decltype(ax_glGetAttribLocation)        ax_glGetAttribLocation = nullptr;
decltype(ax_glGetIntegerv)              ax_glGetIntegerv = nullptr;
decltype(ax_glGetProgramInfoLog)        ax_glGetProgramInfoLog = nullptr;
decltype(ax_glGetProgramiv)             ax_glGetProgramiv = nullptr;
decltype(ax_glGetShaderInfoLog)         ax_glGetShaderInfoLog = nullptr;
decltype(ax_glGetShaderiv)              ax_glGetShaderiv = nullptr;
decltype(ax_glGetUniformLocation)       ax_glGetUniformLocation = nullptr;
decltype(ax_glLinkProgram)              ax_glLinkProgram = nullptr;
decltype(ax_glPixelStorei)              ax_glPixelStorei = nullptr;
decltype(ax_glScissor)                  ax_glScissor = nullptr;
decltype(ax_glShaderSource)             ax_glShaderSource = nullptr;
decltype(ax_glTexImage2D)               ax_glTexImage2D = nullptr;
decltype(ax_glTexParameteri)            ax_glTexParameteri = nullptr;
decltype(ax_glUniform1i)                ax_glUniform1i = nullptr;
decltype(ax_glUniform4f)                ax_glUniform4f = nullptr;
decltype(ax_glUniformMatrix4fv)         ax_glUniformMatrix4fv = nullptr;
decltype(ax_glUseProgram)               ax_glUseProgram = nullptr;
decltype(ax_glVertexAttribPointer)      ax_glVertexAttribPointer = nullptr;
decltype(ax_glViewport)                 ax_glViewport = nullptr;

bool loadFunctions()
{
    bool ok = true;

    // Each name below is the real ES 2.0 symbol to look up; the pointer it is
    // stored in is the ax_-prefixed variable. The gl* macro names are in
    // effect after GLESLoader.h, but string literals are unaffected by macros.
    auto load = [&ok](auto& fn, const char* name) {
        fn = reinterpret_cast<std::remove_reference_t<decltype(fn)>>(
            SDL_GL_GetProcAddress(name));
        if (!fn) {
            std::cerr << "GLESLoader: missing OpenGL ES 2.0 entry point: "
                      << name << std::endl;
            ok = false;
        }
    };

    load(ax_glActiveTexture,            "glActiveTexture");
    load(ax_glAttachShader,             "glAttachShader");
    load(ax_glBindBuffer,               "glBindBuffer");
    load(ax_glBindTexture,              "glBindTexture");
    load(ax_glBlendFunc,                "glBlendFunc");
    load(ax_glBufferData,               "glBufferData");
    load(ax_glClear,                    "glClear");
    load(ax_glClearColor,               "glClearColor");
    load(ax_glCompileShader,            "glCompileShader");
    load(ax_glCreateProgram,            "glCreateProgram");
    load(ax_glCreateShader,             "glCreateShader");
    load(ax_glDeleteBuffers,            "glDeleteBuffers");
    load(ax_glDeleteProgram,            "glDeleteProgram");
    load(ax_glDeleteShader,             "glDeleteShader");
    load(ax_glDeleteTextures,           "glDeleteTextures");
    load(ax_glDepthMask,                "glDepthMask");
    load(ax_glDisable,                  "glDisable");
    load(ax_glDisableVertexAttribArray, "glDisableVertexAttribArray");
    load(ax_glDrawArrays,               "glDrawArrays");
    load(ax_glEnable,                   "glEnable");
    load(ax_glEnableVertexAttribArray,  "glEnableVertexAttribArray");
    load(ax_glFlush,                    "glFlush");
    load(ax_glGenBuffers,               "glGenBuffers");
    load(ax_glGenTextures,              "glGenTextures");
    load(ax_glGetAttribLocation,        "glGetAttribLocation");
    load(ax_glGetIntegerv,              "glGetIntegerv");
    load(ax_glGetProgramInfoLog,        "glGetProgramInfoLog");
    load(ax_glGetProgramiv,             "glGetProgramiv");
    load(ax_glGetShaderInfoLog,         "glGetShaderInfoLog");
    load(ax_glGetShaderiv,              "glGetShaderiv");
    load(ax_glGetUniformLocation,       "glGetUniformLocation");
    load(ax_glLinkProgram,              "glLinkProgram");
    load(ax_glPixelStorei,              "glPixelStorei");
    load(ax_glScissor,                  "glScissor");
    load(ax_glShaderSource,             "glShaderSource");
    load(ax_glTexImage2D,               "glTexImage2D");
    load(ax_glTexParameteri,            "glTexParameteri");
    load(ax_glUniform1i,                "glUniform1i");
    load(ax_glUniform4f,                "glUniform4f");
    load(ax_glUniformMatrix4fv,         "glUniformMatrix4fv");
    load(ax_glUseProgram,               "glUseProgram");
    load(ax_glVertexAttribPointer,      "glVertexAttribPointer");
    load(ax_glViewport,                 "glViewport");

    return ok;
}

} // namespace gles
} // namespace arx

#endif // !__EMSCRIPTEN__
