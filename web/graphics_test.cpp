// Bare-minimum Emscripten graphics test
// Compile with: emcc graphics_test.cpp -o graphics_test.html -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=png

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <emscripten.h>
#include <emscripten/html5.h>

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* background_texture = nullptr;

void render_loop() {
    // Set draw color to black (R, G, B, A) for the outer background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    
    // Clear the entire screen with black
    SDL_RenderClear(renderer);
    
    // Create the inner view panel rectangle (50% of screen size, centered)
    SDL_Rect inner_panel;
    inner_panel.w = 800 / 2;   // 400 pixels wide
    inner_panel.h = 600 / 2;   // 300 pixels tall
    inner_panel.x = (800 - inner_panel.w) / 2;  // center horizontally
    inner_panel.y = (600 - inner_panel.h) / 2;  // center vertically
    
    // Render the background image ONLY within the inner view panel
    if (background_texture) {
        SDL_RenderCopy(renderer, background_texture, nullptr, &inner_panel);
    }
    
    // Set draw color to red (R, G, B, A) for the inner view panel border
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    
    // Draw the border (1 pixel thick) around the view panel
    SDL_RenderDrawRect(renderer, &inner_panel);
    
    // Present the rendered frame to the canvas
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    // Initialize SDL with video subsystem
    SDL_Init(SDL_INIT_VIDEO);
    
    // Initialize SDL_image for PNG support
    IMG_Init(IMG_INIT_PNG);
    
    // Create a window (800x600, hidden, uses existing canvas)
    window = SDL_CreateWindow("", 0, 0, 800, 600, SDL_WINDOW_HIDDEN);
    
    // Create a hardware-accelerated renderer targeting the canvas
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    
    // Load the background image
    background_texture = IMG_LoadTexture(renderer, "data/images/backgrounds/cityBackE.png");
    
    // Set the main loop callback - runs every frame
    emscripten_set_main_loop(render_loop, 0, true);
    
    return 0;
}
