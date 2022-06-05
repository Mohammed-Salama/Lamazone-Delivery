#include "world.hpp"
#include<set>
#include <valarray> // valarray
#include<vector>
#include<map>
namespace our {

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json& data, Entity* parent){
        if(!data.is_array()) return;
        std::set<std::string> dynamicObjects = {"box1","box2","box3","box4","battery","deliveryArrow","pickArrow"};
        std::set<std::string> boxSet = {"box1","box2","box3","box4"};
        std::map<std::string,nlohmann::json> boxes;
        nlohmann::json deliveryArrow;
        nlohmann::json pickArrow;
        nlohmann::json battery;
        
        for(const auto& entityData : data){

            auto name = entityData.value("name", "");
            if(auto iter = dynamicObjects.find(name); iter!=dynamicObjects.end()){

                nlohmann::json tempEntityData = entityData;

                if(boxSet.find(name)!=boxSet.end())
                    boxes[name] = tempEntityData;
                else if (name=="battery")
                    battery = tempEntityData;
                else if(name=="deliveryArrow")
                    deliveryArrow = tempEntityData;
                else if(name=="pickArrow")
                    pickArrow = tempEntityData;

                continue;
            }

            Entity* entity = add();
            entity->parent = parent;
            entity->deserialize(entityData);
            if(entityData.contains("children"))
                this->deserialize(entityData["children"], entity);

        }

        //Assert that all needed components are stored.
        if(boxes.size()==0 || deliveryArrow.size()==0 || pickArrow.size()==0 || battery.size()==0)
            return;

        // Generate the road objects:
        int numberOfmissions = 10;
        int intialPos = 300;
        int distanceIncrement = 200;
        int minX = -40;
        int maxX =  40;
        for(int i=0,pos=intialPos;i<numberOfmissions*3;i++,pos+=distanceIncrement){      

            nlohmann::json entityData;

            int randomX = minX + ( std::rand() % ( maxX - minX + 1 ) );
            nlohmann::json newPos =       {randomX,1,-1*pos};
            nlohmann::json newPosArrows = {randomX,10,-1*pos};

            if(i%3==0){                                               //Add pick zone

                entityData = boxes["box"+std::to_string(std::rand()%boxes.size() + 1)];
                entityData.at("position") = newPos;
                Entity* entity = add();
                entity->parent = parent;
                entity->deserialize(entityData);

                entityData = pickArrow;
                entityData.at("position") = newPosArrows;
                entity = add();
                entity->parent = parent;
                entity->deserialize(entityData);


            }
            else if(i%3==1){                                         //Add battery

                entityData = battery;
                entityData.at("position") = newPos;
                Entity* entity = add();
                entity->parent = parent;
                entity->deserialize(entityData);

            }
            else{                                                    //Add delivery zone

                entityData = deliveryArrow;
                entityData.at("position") = newPosArrows;
                Entity* entity = add();
                entity->parent = parent;
                entity->deserialize(entityData);

            }

        
        }


    }

}