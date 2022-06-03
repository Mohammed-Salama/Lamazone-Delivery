#pragma once

#include "../ecs/component.hpp"

#include <glm/glm.hpp> 
#include <chrono>
#include <ctime>    

namespace our {

    // This component denotes that the GameLogicControllerComponent will handle all the player related logic.
    // This component is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/systems/game-logic-controller.hpp"
    class GameLogicControllerComponent : public Component {
    public:
        float maxEnergy = 100.0f;           // The maximum energy of the entity.
        float energy = 100.0f;              // The current energy of the entity.
        float maxSpeed = 5.0f;              // The maximum speed the entity can move with. 
        float speed = 3.0f;                 // The current speed the entity is moving with.
        float energyDecrementValue=1.0f;    // The value by which the energy decrease each second.
        // The last time the energy was decremented.
        std::chrono::time_point<std::chrono::system_clock> lastDecrementTime;

        bool lost = false;                  // Indicates whether a player has lost or not.

        // The ID of this component type is "Game Logic Controller"
        static std::string getID() { return "Game Logic Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}