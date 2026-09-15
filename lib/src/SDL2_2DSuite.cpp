#include "SDL2_2DSuite.hpp"

#include <iomanip>
#include <sstream>

namespace SDL2_2DSuite_Internal {
    void Controls::update() {
        keys.update();
        mouse.update();
    }

    bool Framerate::doDelay() {
        bool isPreviousStepRenderStep = isRenderStep;
        isRenderStep                  = isRenderStepLinked ? doLinkedDelay() : doUnlinkedDelay();
        updatesPerFrame               = isPreviousStepRenderStep ? 0 : updatesPerFrame + 1;
        return isRenderStep;
    }

    void Framerate::forceUpdatesPerFrame(const uint32_t upf) {
        updatesPerFrame = upf;
    }

    std::string Framerate::getDebugText() const {
        std::stringstream futps;
        futps << "FPS: " << std::fixed << std::setprecision(2) << getCurrentFramesPerSecond() << "/" << FPS 
              << " | UPF: " << std::fixed << std::setprecision(2) << updatesPerFrame
              << " | TPF: " << std::fixed << std::setprecision(2) << ticksInFrame;
        return futps.str();
    }

    bool Framerate::doLinkedDelay() {
        auto currentTime = SDL_GetTicks64();
        static auto previous = currentTime;
        static auto previousComputeStepTime = currentTime;
        static uint64_t nextExpectedFrameTime = currentTime + (1000.0 / FPS);
        static double extraTick = 0;
        double fpsinc = 1000.0 / FPS;
        double delay = 0;

        computeTime = currentTime - previousComputeStepTime;
        previousComputeStepTime = currentTime;

        if (currentTime < nextExpectedFrameTime) {
            delay = nextExpectedFrameTime - currentTime;
        }
        nextExpectedFrameTime += (int32_t)fpsinc + (int32_t)extraTick;
        extraTick             += (fpsinc - (int32_t)fpsinc) - (int32_t)extraTick;
        ticksInFrame = currentTime - previous;
        frameTimes.push(ticksInFrame);
        frameTimeSum += ticksInFrame;
        while (frameTimes.size() > FPS) {
            frameTimeSum -= frameTimes.front();
            frameTimes.pop();
        }
        previous = currentTime;
        if ((uint32_t)delay == 0) {
            SDL_Log("Frame Skipped: Performance may be an issue.");
        }
        SDL_Delay(delay);
        return true;
    }

    bool Framerate::doUnlinkedDelay() {
        auto currentTime                      = SDL_GetTicks64();
        static auto previous                  = currentTime;
        static auto previousComputeStepTime   = currentTime;
        static uint64_t nextExpectedFrameTime = currentTime + (1000.0 / FPS);
        static double extraTick               = 0;
        double fpsinc                         = 1000.0 / FPS;

        computeTime             = currentTime - previousComputeStepTime;
        previousComputeStepTime = currentTime;

        if (currentTime < nextExpectedFrameTime) return false;

        auto dif               = currentTime - nextExpectedFrameTime;
        nextExpectedFrameTime += (int32_t)fpsinc + (int32_t)extraTick;
        extraTick             += (fpsinc - (int32_t)fpsinc) - (int32_t)extraTick;
        if (dif > fpsinc * 2) return false;

        ticksInFrame = currentTime - previous;
        frameTimes.push(ticksInFrame);
        frameTimeSum += ticksInFrame;
        while (frameTimes.size() > FPS) {
            frameTimeSum -= frameTimes.front();
            frameTimes.pop();
        }
        previous = currentTime;
        return true;
    }
}  // namespace SDL2_2DSuite_Internal

namespace SDL2_2DSuite {
    void Essentials::init() {
        screen.init();
    }
    void Essentials::pollEvents(bool& isRunning) {
        auto& keys = controls.keys;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT   : isRunning = false; break;
                case SDL_KEYDOWN: keys[event.key.keysym.sym].state = true; break;
                case SDL_KEYUP  : keys[event.key.keysym.sym].state = false; break;
            }
        }
    }
}  // namespace SDL2_2DSuite
