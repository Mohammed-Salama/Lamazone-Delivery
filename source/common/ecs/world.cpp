#include "world.hpp"
#include<set>
#include <valarray> // valarray
#include<vector>
namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;
        std::set<std::string> dynamicObjects = {"box","battery","arrow"};
        for(const auto& entityData : data){

            std::cout<<"here?"<<std::endl;
            auto name = entityData.value("name", "");
            if(auto iter = dynamicObjects.find(name); iter!=dynamicObjects.end()){

                nlohmann::json tempEntityData = entityData;
                // dynamic objects cannot have childerens (IMPORTANT)

                
                for(int i=0;i<10;i++){

                    nlohmann::json newPos = {};
                    if(*iter=="box")
                        newPos  = {40,1,-500*i};
                    else if(*iter=="battery")
                        newPos =  {-5,1,-200*i};
                    else if(*iter=="arrow")
                        newPos =  {40,10,-500*i};   //ARROW
                    


                    tempEntityData.at("position") =newPos;
                    Entity* entity = add();
                    entity->parent = parent;
                    entity->deserialize(tempEntityData);

                }
                continue;
            }

            Entity* entity = add();
            entity->parent = parent;
            entity->deserialize(entityData);
            if(entityData.contains("children"))
                this->deserialize(entityData["children"], entity);

        }
    }

}