# Alternate Reality X (ARX) - Web Port

## About This Project

This is a web port of Alternate Reality X (ARX), a computer role playing game based on the classic 1980s game series by Philip Price. The web port allows the game to run directly in modern web browsers using Emscripten and WebGL, making it accessible across multiple platforms without requiring native compilation.

Whilst many features of the original City and Dungeon games are faithfully reproduced, ARX is not intended to be an identical copy and contains some new features and differences in gameplay.

Currently you can explore the City and Dungeon levels, visit many of the special locations, have encounters, buy weapons and find treasure but there is still a lot to be added before it will be close to completion.

I'm also planning to add in the originally planned, unfinished scenarios. These include the Arena, the Palace, The Wilderness, Revelation and Destiny. These are a longer term development goal.

I hope you enjoy the game.

## Web Build Features

The web port includes the following improvements and features:

- **Cross-platform compatibility** - Runs in any modern web browser
- **WebGL rendering** - Hardware-accelerated 3D graphics
- **Responsive UI** - Scales to different window sizes
- **Initial stats** - Food, torches, and water start at 3
- **Improved status display** - Status text shows prominently under the banner
- **Enhanced item management** - Drop functionality with quantity selection
- **Fixed UI issues** - Proper menu transitions and text display

## Building the Web Version

### Prerequisites

- Emscripten SDK (emsdk)
- CMake
- Git

### Build Instructions

1. Clone the repository:
   ```bash
   git clone https://github.com/carlSankey/ARX.git
   cd ARX
   git checkout arxweb
   ```

2. Set up Emscripten environment:
   ```bash
   emsdk
   ```

3. Build the web version:
   ```bash
   cd web
   build.cmd  # Windows
   # or
   ./build.sh  # Linux/Mac
   ```

4. The output will be in the parent directory as `arxx_web.html` and `arxx_web.js`

5. Serve the files using a local web server:
   ```bash
   python -m http.server 8000
   # or
   npx serve .
   ```

6. Open your browser to `http://localhost:8000/arxx_web.html`

## Original C++ Version

The original C++ source code is still available in this repository. You can compile it for native platforms using:

- **Code::Blocks** project files (ARXX.cbp)
- **Dev-C++** project files
- **Makefile** for GCC

You will need to set up the library paths in the project files to match your system configuration.

## Releases

All releases will be under the folder "Releases". They will be zipped up there for downloading.

## Source Code

The source code is under the `src` folder. You can look at it and compile it.

## Support

If you have any Amiga images from the game that are missing from this version, please let us know and send them to us, so we can get them into the ARX system. Many of the Amiga graphics are being used for the 16-bit version if you play using 16-bit vs. 8-bit. Most of the 8-bit is from Atari.

Just create an Issue and attach the graphics to it. It can just be a screen shot and we can get it into the system.

If you have any of the Amiga songs captured, please create an issue and upload the sound file so we can get them into the game.

Thanks for all your support.

## Contributing

Since the original coder has not been able to update the code in about a year, I've stepped in to see this completed. There are many bug fixes that have needed to be done and enhancements. That is what I'm trying to do here.

I will be releasing new versions all the time. I will post that here and on the original forum that CRPG created for Alternate Reality X.

## License

Alternate Reality X is based on the classic game series by Philip Price. This project is a fan-made continuation and enhancement of the original work.

## Links

- **GitHub:** https://github.com/carlSankey/ARX
- **Web Branch:** https://github.com/carlSankey/ARX/tree/arxweb

---

*Alternate Reality X is a tribute to the classic RPG series and is not affiliated with the original creators.*