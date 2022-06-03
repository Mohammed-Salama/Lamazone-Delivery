#pragma once

#include "../ecs/component.hpp"

#include <glm/mat4x4.hpp>

namespace our {

    // An enum that defines the type of the Light (SPOT or DIRECTIONAL) 
    enum class LightType {
        DIRECTIONAL,
        SPOT,
        POINT
        
    };

    class LightComponent : public Component {
    public:
        LightType lightType; // The type of the Light
        glm::vec3 diffuse = {0,0,0};
        glm::vec3 specular = {0,0,0};
        glm::vec3 attenuation = {0,0,0}; // x*d^2 + y*d + z
        glm::vec2 cone_angles = {0,0}; // x: inner_angle, y: outer_angle

        // The ID of this component type is "Light"
        static std::string getID() { return "Light"; }

        // Reads light parameters from the given json object
        // we need to add json values
        void deserialize(const nlohmann::json& data) override;

    };

}