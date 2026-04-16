#include "Game.h"
#include "Profiling.h"
#include "Logger.h"
#include <csignal>
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <chrono>

#ifndef EMSCRIPTEN
#include <backward.hpp>
namespace backward {
    backward::SignalHandling sh;
}
#endif

int main(int argc, char* argv[]) {
    // Analisa argumentos da linha de comando
    bool isScreenshotTest = false;
    bool isBenchmark = false;
    int testFrames = 0;
    std::string screenshotPath = "test_capture.png";

    std::vector<std::string> args(argv, argv + argc);
    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--screenshot-test") {
            isScreenshotTest = true;
            if (i + 1 < args.size()) testFrames = std::stoi(args[i + 1]);
        }
        if (args[i] == "--benchmark") {
            isBenchmark = true;
            if (i + 1 < args.size()) testFrames = std::stoi(args[i + 1]);
        }
        if (args[i] == "--output") {
            if (i + 1 < args.size()) screenshotPath = args[i + 1];
        }
    }

    // Inicializa o sistema de logs
    Ralph::Logger::Init();
    LOG_INFO("Iniciando Ralph Arcade v1.0.0");

    try {
        if (isBenchmark) {
            SetTargetFPS(0); 
        }

        Ralph::Game game;
        LOG_INFO("Motor do jogo inicializado corretamente.");
        
        PlayMusicStream(game.music);
        
        int currentFrame = 0;
        auto startTime = std::chrono::high_resolution_clock::now();

        while (!WindowShouldClose()) {
            game.Update();
            game.Draw();
            
            currentFrame++;

            if (isScreenshotTest && currentFrame >= testFrames) {
                Image img = LoadImageFromScreen();
                ExportImage(img, screenshotPath.c_str());
                UnloadImage(img);
                LOG_INFO("Screenshot de teste EXPORTADO para: {}", screenshotPath);
                break;
            }

            if (isBenchmark && currentFrame >= testFrames) {
                auto endTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsed = endTime - startTime;
                double avgFps = currentFrame / elapsed.count();
                
                std::cout << "\n--- BENCHMARK RESULTS ---\n";
                std::cout << "Frames: " << currentFrame << "\n";
                std::cout << "Time: " << elapsed.count() << "s\n";
                std::cout << "Avg FPS: " << avgFps << "\n";
                std::cout << "-------------------------\n" << std::endl;
                
                LOG_INFO("Benchmark concluído. FPS Médio: {:.2f}", avgFps);
                break;
            }
            
            PROFILE_FRAME_MARK;
        }
    } catch (const std::exception& e) {
        LOG_CRITICAL("Exceção não capturada: {}", e.what());
#ifndef EMSCRIPTEN
        backward::StackTrace st; st.load_here(32);
        backward::Printer p; p.print(st);
#endif
    } catch (...) {
        LOG_CRITICAL("Erro desconhecido capturado no main!");
    }

    LOG_INFO("Finalizando jogo normalmente.");
    Ralph::Logger::Shutdown();
    return 0;
}
