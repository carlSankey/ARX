// BgfxRenderer - Modern GPU renderer for 3D corridor view.
// Replaces legacy OpenGL immediate-mode with bgfx.
// Enable with: #define ARX_USE_BGFX 1

#include "BgfxRenderer.h"
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <bx/math.h>

#ifdef _WIN32
#include <windows.h>
#endif

BgfxRenderer::BgfxRenderer() = default;

BgfxRenderer::~BgfxRenderer()
{
    shutdown();
}

bool BgfxRenderer::init(sf::RenderWindow& window)
{
    if (m_initialized)
        return true;

#ifdef _WIN32
    bgfx::PlatformData platformData;
    platformData.ndt = nullptr;
    platformData.nwh = (void*)window.getNativeHandle();
    platformData.context = nullptr;
    platformData.backBuffer = nullptr;
    platformData.backBufferDS = nullptr;
    bgfx::setPlatformData(platformData);
#endif

    bgfx::Init init;
    // Use OpenGL backend - SFML uses OpenGL, so both must share the same API
    init.type = bgfx::RendererType::OpenGL;
    init.vendorId = BGFX_PCI_ID_NONE;
    init.resolution.width = static_cast<uint32_t>(window.getSize().x);
    init.resolution.height = static_cast<uint32_t>(window.getSize().y);
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init))
    {
        return false;
    }

    m_width = init.resolution.width;
    m_height = init.resolution.height;
    m_initialized = true;

    // Enable debug text (optional - helps verify bgfx is working)
    bgfx::setDebug(BGFX_DEBUG_NONE);

    return true;
}

void BgfxRenderer::shutdown()
{
    if (!m_initialized)
        return;

    bgfx::shutdown();
    m_initialized = false;
}

void BgfxRenderer::frame()
{
    if (!m_initialized)
        return;

    // Advance to next frame. Pass false to let SFML handle swap.
    bgfx::frame(false);
}

void BgfxRenderer::resize(uint32_t width, uint32_t height)
{
    if (!m_initialized)
        return;

    m_width = width;
    m_height = height;
    bgfx::reset(width, height, BGFX_RESET_VSYNC);
}
