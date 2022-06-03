#include "light.hpp"
#include "../ecs/entity.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
namespace our {
    // Reads camera parameters from the given json object
    void LightComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        // not sure what should be with lighttype
        std::string lightTypeStr = data.value("lightType", "DIRECTIONAL");
        if(lightTypeStr == "DIRECTIONAL"){
            lightType = LightType::DIRECTIONAL;
        } else if(lightTypeStr == "SPOT") {
            lightType = LightType::SPOT;
        }
        else{
            lightType =LightType::POINT;
        }
        // i should add values here
    }
    // some functions to compute values


}