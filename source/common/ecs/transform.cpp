#include "entity.hpp"
#include "../deserialize-utils.hpp"

#include <glm/gtx/euler_angles.hpp>

namespace our {

    // This function computes and returns a matrix that represents this transform
    // Remember that the order of transformations is: Scaling, Rotation then Translation
    // HINT: to convert euler angles to a rotation matrix, you can use glm::yawPitchRoll
    glm::mat4 Transform::toMat4() const {
        //DONE: (Req 2) Write this function
        glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 rot = glm::yawPitchRoll(rotation.y,rotation.x, rotation.z);
        glm::mat4 tra = glm::translate(glm::mat4(1.0f), position);
        return tra*rot*sca; // Since the transformation matrix on the right is applied first.
        // i.e. T * R * S 
    }

    glm::mat4 Transform::toMat4Scaled(float value, float max) {
        //DONE: (Req 2) Write this function
        glm::mat4 sca = glm::scale(glm::mat4(1.0f), glm::vec3(scale.x*value/max, scale.y, scale.z));
        glm::mat4 rot = glm::yawPitchRoll(rotation.y,rotation.x, rotation.z);
        glm::mat4 tra = glm::translate(glm::mat4(1.0f), position);
        return tra*rot*sca; // Since the transformation matrix on the right is applied first.
        // i.e. T * R * S 
    }

     // Deserializes the entity data and components from a json object
    void Transform::deserialize(const nlohmann::json& data){
        position = data.value("position", position);
        rotation = glm::radians(data.value("rotation", glm::degrees(rotation)));
        scale    = data.value("scale", scale);
        initialScale    = scale;
    }

}