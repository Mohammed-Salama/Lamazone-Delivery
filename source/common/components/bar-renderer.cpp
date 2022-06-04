#include "bar-renderer.hpp"
#include "../asset-loader.hpp"

namespace our {
    // Receives the mesh & material from the AssetLoader by the names given in the json object
    void BarRendererComponent::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;
        float maxHeight = data.value("maxHeight", maxHeight);
        float maxWidth = data.value("maxWidth", maxWidth);
        float x = data.value("x", x);
        float y = data.value("y", y);
        bar = new our::Bar(x, y, maxHeight, maxWidth);
    }
}