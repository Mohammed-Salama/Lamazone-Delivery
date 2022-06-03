#include "game-logic-controller.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"

#include <chrono>
#include <ctime>

namespace our {
    // Reads the maximum energy & maximum speed from the given json object
    void GameLogicControllerComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        maxEnergy = data.value("maxEnergy", maxEnergy);
        energy = maxEnergy;
        maxSpeed = data.value("maxSpeed", maxSpeed);
        speed = maxSpeed;
        energyDecrementValue = data.value("energyDecrementValue", energyDecrementValue);
        lastDecrementTime = std::chrono::system_clock::now();
    }
}