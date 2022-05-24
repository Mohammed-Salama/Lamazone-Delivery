#include "material.hpp"

#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our {

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const {
        //DONE: (Req 6) Write this function
        // start setup of pipeline
        pipelineState.setup();
        // to use shader
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json& data){
        if(!data.is_object()) return;

        if(data.contains("pipelineState")){
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint 
    void TintedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        // setup material
       Material::setup();
       // set tint for shader
       shader->set("tint",tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex" 
    void TexturedMaterial::setup() const {
        //DONE: (Req 6) Write this function
        // call tint setup to add  tint to shadder
        TintedMaterial::setup();
        // set threshold
        shader->set("alphaThreshold",alphaThreshold);
        // set texture to shadder
        shader->set("tex",0);
        // bind texture & sampler
        texture->bind();
        sampler->bind(0);
        
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json& data){
        TintedMaterial::deserialize(data);
        if(!data.is_object()) return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }
    void LitMaterial::setup() const {

       // if i call setup of material , i will miss color , if i call tint  , i will also miss color
       // if i call texture , i will get color & tint
       // but not sure by 100%
       TexturedMaterial::setup();
       //  i should send some values to its shader?
      // Does we need to add sampler here with 0,1,2 ...etc??
       // bind textures of light
       albedo->bind();
       specular->bind();
       ambient_occlusion->bind();
       roughness->bind();
       emissive->bind();

    }

        // This function read the material data from a json object
        // i didn't do json object so this is not completed
    void LitMaterial::deserialize(const nlohmann::json& data){
        Material::deserialize(data);
        if(!data.is_object()) return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", ""));
        specular = AssetLoader<Texture2D>::get(data.value("specular", ""));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", ""));
    }

}