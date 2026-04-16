#include "Game.h"
#include "Profiling.h"
#include "Logger.h"
#include <csignal>
#include <iostream>

// Função para capturar sinais de crash (Segmentation Fault, etc)
void SignalHandler(int signal) {
    std::string signalName;
    switch (signal) {
        case SIGSEGV: signalName = "SIGSEGV (Segmentation Fault)"; break;
        case SIGABRT: signalName = "SIGABRT (Abort)"; break;
        case SIGFPE:  signalName = "SIGFPE (Floating Point Exception)"; break;
        case SIGILL:  signalName = "SIGILL (Illegal Instruction)"; break;
        default:      signalName = "Unknown Signal"; break;
    }
    
    LOG_CRITICAL("CRASH DETECTADO! Sinal: {}", signalName);
    Logger::Shutdown(); // Garante que o log seja escrito no disco
    std::exit(signal);
}

int main() {
    // Inicializa o sistema de logs
    Logger::Init();
    LOG_INFO("Iniciando Ralph Arcade v1.0.0");

    // Configura os handlers de sinal para capturar crashes
    std::signal(SIGSEGV, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGFPE,  SignalHandler);
    std::signal(SIGILL,  SignalHandler);

    try {
        Game game;
        LOG_INFO("Motor do jogo inicializado corretamente.");
        
        PlayMusicStream(game.music);
        
        while (!WindowShouldClose()) {
            game.Update();
            game.Draw();
            PROFILE_FRAME_MARK;
        }
    } catch (const std::exception& e) {
        LOG_CRITICAL("Exceção não capturada: {}", e.what());
    } catch (...) {
        LOG_CRITICAL("Erro desconhecido capturado no main!");
    }

    LOG_INFO("Finalizando jogo normalmente.");
    Logger::Shutdown();
    return 0;
}
