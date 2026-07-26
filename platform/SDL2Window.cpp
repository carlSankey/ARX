#include "SDL2Window.h"

#ifdef ARX_USE_SDL2

#include <iostream>
#include <SDL_opengl.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/html5.h>
#endif

namespace arx {

SDL2Window::~SDL2Window()
{
    destroy();
}

bool SDL2Window::create(const WindowConfig& config)
{
    if (m_window) {
        return true; // Already created
    }

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return false;
    }

#ifdef __EMSCRIPTEN__
    // Emscripten with -sUSE_WEBGL2=1 handles GL context creation automatically
    // Do NOT set GL version attributes - they conflict with WebGL2 initialization
#else
    // OpenGL 3.3 Core for native
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // Enable text input so SDL_TEXTINPUT events are generated
    SDL_StartTextInput();

    uint32_t flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
    if (config.fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN;
    }

    m_window = SDL_CreateWindow(
        config.title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        config.width,
        config.height,
        flags
    );

    if (!m_window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_glContext = SDL_GL_CreateContext(m_window);
    if (!m_glContext) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
        SDL_Quit();
        return false;
    }

    SDL_GL_SetSwapInterval(config.vsync ? 1 : 0);

    m_width = config.width;
    m_height = config.height;

    return true;
}

void SDL2Window::destroy()
{
    if (m_glContext) {
        SDL_GL_DeleteContext(m_glContext);
        m_glContext = nullptr;
    }
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    SDL_Quit();
}

std::optional<Event> SDL2Window::pollEvent()
{
    SDL_Event sdlEvent;
    if (!SDL_PollEvent(&sdlEvent)) {
        return std::nullopt;
    }

    Event event;

    switch (sdlEvent.type) {
    case SDL_QUIT:
        event.type = EventType::Closed;
        return event;

    case SDL_KEYDOWN:
        event.type = EventType::KeyPressed;
        event.key = sdlKeyToArxKey(sdlEvent.key.keysym.sym);
        return event;

    case SDL_KEYUP:
        event.type = EventType::KeyReleased;
        event.key = sdlKeyToArxKey(sdlEvent.key.keysym.sym);
        return event;

    case SDL_TEXTINPUT:
        event.type = EventType::TextInput;
        event.text = sdlEvent.text.text;
        return event;

    case SDL_MOUSEBUTTONDOWN:
        event.type = EventType::MouseButtonPressed;
        event.mouseButton = sdlEvent.button.button;
        return event;

    case SDL_MOUSEBUTTONUP:
        event.type = EventType::MouseButtonReleased;
        event.mouseButton = sdlEvent.button.button;
        return event;

    case SDL_WINDOWEVENT:
        if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED) {
            event.type = EventType::Resized;
            event.width = sdlEvent.window.data1;
            event.height = sdlEvent.window.data2;
            m_width = event.width;
            m_height = event.height;
            return event;
        }
        break;
    }

    return std::nullopt;
}

void SDL2Window::display()
{
    if (m_window && m_glContext) {
        SDL_GL_SwapWindow(m_window);
    }
}

void SDL2Window::clear()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void* SDL2Window::getNativeHandle() const
{
#ifdef _WIN32
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (SDL_GetWindowWMInfo(m_window, &wmInfo)) {
        return wmInfo.info.win.window;
    }
#endif
    return nullptr;
}

Key SDL2Window::sdlKeyToArxKey(SDL_Keycode sdl)
{
    switch (sdl) {
    case SDLK_a: return Key::A;
    case SDLK_b: return Key::B;
    case SDLK_c: return Key::C;
    case SDLK_d: return Key::D;
    case SDLK_e: return Key::E;
    case SDLK_f: return Key::F;
    case SDLK_g: return Key::G;
    case SDLK_h: return Key::H;
    case SDLK_i: return Key::I;
    case SDLK_j: return Key::J;
    case SDLK_k: return Key::K;
    case SDLK_l: return Key::L;
    case SDLK_m: return Key::M;
    case SDLK_n: return Key::N;
    case SDLK_o: return Key::O;
    case SDLK_p: return Key::P;
    case SDLK_q: return Key::Q;
    case SDLK_r: return Key::R;
    case SDLK_s: return Key::S;
    case SDLK_t: return Key::T;
    case SDLK_u: return Key::U;
    case SDLK_v: return Key::V;
    case SDLK_w: return Key::W;
    case SDLK_x: return Key::X;
    case SDLK_y: return Key::Y;
    case SDLK_z: return Key::Z;
    case SDLK_0: return Key::Num0;
    case SDLK_1: return Key::Num1;
    case SDLK_2: return Key::Num2;
    case SDLK_3: return Key::Num3;
    case SDLK_4: return Key::Num4;
    case SDLK_5: return Key::Num5;
    case SDLK_6: return Key::Num6;
    case SDLK_7: return Key::Num7;
    case SDLK_8: return Key::Num8;
    case SDLK_9: return Key::Num9;
    case SDLK_ESCAPE: return Key::Escape;
    case SDLK_SPACE: return Key::Space;
    case SDLK_RETURN: return Key::Enter;
    case SDLK_BACKSPACE: return Key::Backspace;
    case SDLK_TAB: return Key::Tab;
    case SDLK_LEFT: return Key::Left;
    case SDLK_RIGHT: return Key::Right;
    case SDLK_UP: return Key::Up;
    case SDLK_DOWN: return Key::Down;
    case SDLK_F1: return Key::F1;
    case SDLK_F2: return Key::F2;
    case SDLK_F3: return Key::F3;
    case SDLK_F4: return Key::F4;
    case SDLK_F5: return Key::F5;
    case SDLK_F6: return Key::F6;
    case SDLK_F7: return Key::F7;
    case SDLK_F8: return Key::F8;
    case SDLK_F9: return Key::F9;
    case SDLK_F10: return Key::F10;
    case SDLK_F11: return Key::F11;
    case SDLK_F12: return Key::F12;
    default: return Key::Unknown;
    }
}

IWindow* createPlatformWindow()
{
    return new SDL2Window();
}

} // namespace arx

#endif // ARX_USE_SDL2
