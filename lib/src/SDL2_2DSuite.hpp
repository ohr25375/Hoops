#pragma once

#include <SDL2/SDL.h>

#include <queue>
#include <random>

#include "SDL2_Addon.hpp"
#include "keypress.hpp"
#include "screen.hpp"
#include "vector2.hpp"

namespace SDL2_2DSuite_Internal {
    class Controls {
    public:
        KEYS keys;
        MOUSE mouse;
        void update();

    private:
    };

    class Framerate {
    public:
        double getCurrentFramesPerSecond() const { return 1000.0 / (frameTimeSum / FPS); };
        bool doDelay();
        uint64_t getComputeTime() const { return computeTime; };
        uint64_t getUpdatesPerFrame() const { return updatesPerFrame; };
        uint64_t getTicksPerFrame() const { return ticksInFrame; };
        void forceUpdatesPerFrame(const uint32_t upf);
        std::string getDebugText() const;

        double FPS              = 60;
        bool isRenderStepLinked = false;

    private:
        bool doLinkedDelay();
        bool doUnlinkedDelay();

        uint64_t frameTimeSum    = 0;
        uint64_t computeTime     = 0;
        uint64_t ticksInFrame    = 0;
        uint32_t updatesPerFrame = 0;
        std::queue<uint64_t> frameTimes;

        bool isRenderStep = false;
    };
}  // namespace SDL2_2DSuite_Internal

namespace SDL2_2DSuite {
    class Essentials {
    private:
        std::random_device rd;

    public:
        std::mt19937 mrandom = std::mt19937(rd());

        void init();
        void pollEvents(bool& isRunning);

        bool isRenderStep() { return frameRate.doDelay(); };

        SDL2_2DSuite_Internal::Framerate frameRate;
        SDL_SCREEN::SCREEN screen;
        SDL2_2DSuite_Internal::Controls controls;
    };
}  // namespace SDL2_2DSuite