#pragma once

// Minimal OpenGL ES 2.0 entry-point loader for the SDL2 builds.
//
// The renderer (renderer/View3D.cpp, renderer/Sprite2D.cpp, display.cpp) is
// written for WebGL / OpenGL ES 2.0. Emscripten provides those GL entry points
// for the web build, but on native Windows neither opengl32.lib nor SDL2.dll
// export the ES 2.0 functions, so every one the game calls is resolved at
// runtime with SDL_GL_GetProcAddress(). This is the same idea as a generated
// glad header, hand-rolled so no extra dependency is needed. (The vcpkg glad
// port cannot generate the GLES2 spec: its portfile hard-codes GLAD_SPEC=gl,
// so only glad/glad.h for the desktop GL spec is ever produced.)
//
// Usage: #include this header in any translation unit that calls ES 2.0
// functions, then use the normal ES 2.0 names; they are #defined onto the
// loaded pointers for the native build. arx::gles::loadFunctions() is called
// once, right after the SDL GL context is created - see
// platform/SDL2Window.cpp.

#include <SDL_opengles2.h>

namespace arx {
namespace gles {

// Resolves every ES 2.0 entry point used by the game from the current GL
// context. Returns false and logs the missing names if any are unavailable.
// On Emscripten the entry points are provided by the JS glue and this is a
// no-op that returns true.
bool loadFunctions();

} // namespace gles
} // namespace arx

#ifndef __EMSCRIPTEN__

// --- Entry-point pointers: one per ES 2.0 call the game makes ---
// decltype() of the real ES 2.0 prototypes keeps the signatures correct, so
// these cannot drift from the Khronos/SDL headers. They are declared before
// the #defines below so the names are still the real functions here.
extern decltype(&glActiveTexture)            ax_glActiveTexture;
extern decltype(&glAttachShader)             ax_glAttachShader;
extern decltype(&glBindBuffer)               ax_glBindBuffer;
extern decltype(&glBindTexture)              ax_glBindTexture;
extern decltype(&glBlendFunc)                ax_glBlendFunc;
extern decltype(&glBufferData)               ax_glBufferData;
extern decltype(&glClear)                    ax_glClear;
extern decltype(&glClearColor)               ax_glClearColor;
extern decltype(&glCompileShader)            ax_glCompileShader;
extern decltype(&glCreateProgram)            ax_glCreateProgram;
extern decltype(&glCreateShader)             ax_glCreateShader;
extern decltype(&glDeleteBuffers)            ax_glDeleteBuffers;
extern decltype(&glDeleteProgram)            ax_glDeleteProgram;
extern decltype(&glDeleteShader)             ax_glDeleteShader;
extern decltype(&glDeleteTextures)           ax_glDeleteTextures;
extern decltype(&glDepthMask)                ax_glDepthMask;
extern decltype(&glDisable)                  ax_glDisable;
extern decltype(&glDisableVertexAttribArray) ax_glDisableVertexAttribArray;
extern decltype(&glDrawArrays)               ax_glDrawArrays;
extern decltype(&glEnable)                   ax_glEnable;
extern decltype(&glEnableVertexAttribArray)  ax_glEnableVertexAttribArray;
extern decltype(&glFlush)                    ax_glFlush;
extern decltype(&glGenBuffers)               ax_glGenBuffers;
extern decltype(&glGenTextures)              ax_glGenTextures;
extern decltype(&glGetAttribLocation)        ax_glGetAttribLocation;
extern decltype(&glGetIntegerv)              ax_glGetIntegerv;
extern decltype(&glGetProgramInfoLog)        ax_glGetProgramInfoLog;
extern decltype(&glGetProgramiv)             ax_glGetProgramiv;
extern decltype(&glGetShaderInfoLog)         ax_glGetShaderInfoLog;
extern decltype(&glGetShaderiv)              ax_glGetShaderiv;
extern decltype(&glGetUniformLocation)       ax_glGetUniformLocation;
extern decltype(&glLinkProgram)              ax_glLinkProgram;
extern decltype(&glPixelStorei)              ax_glPixelStorei;
extern decltype(&glScissor)                  ax_glScissor;
extern decltype(&glShaderSource)             ax_glShaderSource;
extern decltype(&glTexImage2D)               ax_glTexImage2D;
extern decltype(&glTexParameteri)            ax_glTexParameteri;
extern decltype(&glUniform1i)                ax_glUniform1i;
extern decltype(&glUniform4f)                ax_glUniform4f;
extern decltype(&glUniformMatrix4fv)         ax_glUniformMatrix4fv;
extern decltype(&glUseProgram)               ax_glUseProgram;
extern decltype(&glVertexAttribPointer)      ax_glVertexAttribPointer;
extern decltype(&glViewport)                 ax_glViewport;


// --- Map the standard ES 2.0 names onto the loaded pointers ---
#define glActiveTexture ax_glActiveTexture
#define glAttachShader ax_glAttachShader
#define glBindBuffer ax_glBindBuffer
#define glBindTexture ax_glBindTexture
#define glBlendFunc ax_glBlendFunc
#define glBufferData ax_glBufferData
#define glClear ax_glClear
#define glClearColor ax_glClearColor
#define glCompileShader ax_glCompileShader
#define glCreateProgram ax_glCreateProgram
#define glCreateShader ax_glCreateShader
#define glDeleteBuffers ax_glDeleteBuffers
#define glDeleteProgram ax_glDeleteProgram
#define glDeleteShader ax_glDeleteShader
#define glDeleteTextures ax_glDeleteTextures
#define glDepthMask ax_glDepthMask
#define glDisable ax_glDisable
#define glDisableVertexAttribArray ax_glDisableVertexAttribArray
#define glDrawArrays ax_glDrawArrays
#define glEnable ax_glEnable
#define glEnableVertexAttribArray ax_glEnableVertexAttribArray
#define glFlush ax_glFlush
#define glGenBuffers ax_glGenBuffers
#define glGenTextures ax_glGenTextures
#define glGetAttribLocation ax_glGetAttribLocation
#define glGetIntegerv ax_glGetIntegerv
#define glGetProgramInfoLog ax_glGetProgramInfoLog
#define glGetProgramiv ax_glGetProgramiv
#define glGetShaderInfoLog ax_glGetShaderInfoLog
#define glGetShaderiv ax_glGetShaderiv
#define glGetUniformLocation ax_glGetUniformLocation
#define glLinkProgram ax_glLinkProgram
#define glPixelStorei ax_glPixelStorei
#define glScissor ax_glScissor
#define glShaderSource ax_glShaderSource
#define glTexImage2D ax_glTexImage2D
#define glTexParameteri ax_glTexParameteri
#define glUniform1i ax_glUniform1i
#define glUniform4f ax_glUniform4f
#define glUniformMatrix4fv ax_glUniformMatrix4fv
#define glUseProgram ax_glUseProgram
#define glVertexAttribPointer ax_glVertexAttribPointer
#define glViewport ax_glViewport

#endif // !__EMSCRIPTEN__
