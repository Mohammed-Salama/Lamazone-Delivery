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
        float totalScore=0.0f;              // The total score achieved by the player this turn.
        float pointsPerPackage=10.0f;       // The number added to the total score once a package is delivered.
        bool deliveryInProgress=false;      // Indicates whether the player is already delivering a package or not.
        float energyLostPerHit=10.0f;       // The value subtracted from the current energy on hitting a car or a building.
        float cooldownTime=2.0f;            // Cooldown time, as in the time after which the player can lose energy again.
        bool wasHit=false;                  // Indicates whether cooldown is ongoing or not.
        
        // The last time the energy was decremented naturally by passing time.
        std::chrono::time_point<std::chrono::system_clock> lastDecrementTime;
        // The last time the player hit a car or a building. Used in hit cooldown logic.
        std::chrono::time_point<std::chrono::system_clock> lastHitTime;
        // The start cooldown since a player can't hit something at the beginning of the turn.
        std::chrono::time_point<std::chrono::system_clock> startTime;

        bool lost = false;                  // Indicates whether a player has lost or not.

        // The ID of this component type is "Game Logic Controller"
        static std::string getID() { return "Game Logic Controller"; }

        // Reads sensitivities & speedupFactor from the given json object
        void deserialize(const nlohmann::json& data) override;
    };

}