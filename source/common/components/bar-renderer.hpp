#pragma once

#include "../ecs/component.hpp"
#include "../mesh/bar.hpp"
#include "../material/material.hpp"
#include "../asset-loader.hpp"

namespace our {

    class BarRendererComponent : public Component {
    public:
        Bar* bar; // The bar that should be drawn

        // The ID of this component type is "Bar Renderer"
        static std::string getID() { return "Bar Renderer"; }

        // Receives the bar variables from the AssetLoader by the names given in the json object
        void deserialize(const nlohmann::json& data) override;
    };

}