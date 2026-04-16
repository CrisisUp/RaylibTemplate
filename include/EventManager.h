#ifndef EVENT_MANAGER_H
#define EVENT_MANAGER_H

#include <functional>
#include <map>
#include <vector>
#include <string>

namespace Ralph {

enum class EventType {
    PLAYER_DAMAGED,
    COIN_COLLECTED,
    LEVEL_COMPLETED,
    GAME_OVER,
    PLAY_SOUND,
    TOGGLE_PAUSE
};

struct Event {
    EventType type;
    std::string soundPath; // Para eventos de som
    float value;           // Para intensidades ou pontos
};

using EventCallback = std::function<void(const Event&)>;

class EventManager {
public:
    static EventManager& Instance() {
        static EventManager instance;
        return instance;
    }

    // Se inscreve para ouvir um tipo de evento
    void Subscribe(EventType type, EventCallback callback) {
        subscribers[type].push_back(callback);
    }

    // Dispara um evento para todos os ouvintes
    void Emit(const Event& event) {
        if (subscribers.find(event.type) != subscribers.end()) {
            for (auto& callback : subscribers[event.type]) {
                callback(event);
            }
        }
    }

private:
    EventManager() {}
    std::map<EventType, std::vector<EventCallback>> subscribers;
};

} // namespace Ralph

#endif
