#pragma once

#include <unordered_set>
#include "entity.hpp"
#include<iostream>
#include<string>

#include "../components/mesh-renderer.hpp"
#include "../material/material.hpp"
#include "../mesh/mesh.hpp"


namespace our {

    // This class holds a set of entities
    class World {
        std::unordered_set<Entity*> entities; // These are the entities held by this world
        std::unordered_set<Entity*> markedForRemoval; // These are the entities that are awaiting to be deleted
                                                      // when deleteMarkedEntities is called
    public:

        World() = default;

        // This will deserialize a json array of entities and add the new entities to the current world
        // If parent pointer is not null, the new entities will be have their parent set to that given pointer
        // If any of the entities has children, this function will be called recursively for these children
        void deserialize(const nlohmann::json& data, Entity* parent = nullptr);

        // This adds an entity to the entities set and returns a pointer to that entity
        // WARNING The entity is owned by this world so don't use "delete" to delete it, instead, call "markForRemoval"
        // to put it in the "markedForRemoval" set. The elements in the "markedForRemoval" set will be removed and
        // deleted when "deleteMarkedEntities" is called.
        Entity* add() {
            Entity* entity = new Entity();
            entity->world = this;
            entities.insert(entity);
            return entity;
        }

        // This returns and immutable reference to the set of all entites in the world.
        const std::unordered_set<Entity*>& getEntities() {
            return entities;
        }

        // This marks an entity for removal by adding it to the "markedForRemoval" set.
        // The elements in the "markedForRemoval" set will be removed and deleted when "deleteMarkedEntities" is called.
        void markForRemoval(Entity* entity){
            if(entities.find(entity) != entities.end())
                markedForRemoval.insert(entity);
        }

        // This removes the elements in "markedForRemoval" from the "entities" set.
        // Then each of these elements are deleted.
        void deleteMarkedEntities(){
            for(auto entity: markedForRemoval){
                entities.erase(entity);
                delete entity;
            }
            markedForRemoval.clear();
        }

        //This deletes all entities in the world
        void clear(){
            for(auto entity: entities){
                delete entity;
            }
            entities.clear();
            markedForRemoval.clear();
        }
       bool collisionlogic(Entity* x, Entity* y)
       {
          double dx,dy,dz;
          dx= x->localTransform.position.x - y->localTransform.position.x;
          dy = x->localTransform.position.y - y->localTransform.position.y;
          dz = x->localTransform.position.z - y->localTransform.position.z;
          double distance = sqrt(dx*dx + dy*dy + dz*dz);
          double radiusx, raduisy;
          Mesh *meshx;
          Mesh *meshy;
          meshx= x->getComponent<MeshRendererComponent>()->mesh;
          meshy =y->getComponent<MeshRendererComponent>()->mesh;
          radiusx = meshx->raduis;
          raduisy =meshy->raduis;
          if(distance <=radiusx+raduisy)
                std::cout<<"Collision!!";  
         std::cout<<"radiusx"<<radiusx<<"raduisy"<<raduisy<<"distance"<<distance<<std::endl;
          return distance <=radiusx+raduisy;
  
       } 
       void detectCollision()
       {

           int i= 0; 
            for(auto entity:entities ){
               int j=0;
               
               for(auto detected : entities)
               {
                //    std::cout<<entity->name<<std::endl;
                   if(i!=j)
                   {
                    //    std::string entitymaterialName =entity->getComponent<MeshRendererComponent>()->material;
                       if((entity->materialName=="battery" && detected->materialName=="box1") || (entity->materialName=="box1" && detected->materialName=="battery" ))
                       {
                
                             // check colllsion logic
                           if(collisionlogic(entity,detected))
                           {

                            std::cout<<"I am here"<<std::endl;
                               // add value for energy
                             if(entity->materialName=="battery"){
              
                              markForRemoval(entity);
                             }
                             else
                             {
                                 markForRemoval(detected);
                             }

                           }
                       }
                       else if((entity->materialName=="building" && detected->materialName=="box1") || (entity->materialName=="box1" && detected->materialName=="building" ))
                       {

                              if(collisionlogic(entity,detected)){
                                // decrease energy       
                              }
                       }
                        else if((entity->materialName=="box1" && detected->materialName=="box1") || (entity->materialName=="box1" && detected->materialName=="box1" ))
                       {

                              if(collisionlogic(entity,detected)){
                                // decrease energy       
                              }
                       }
                   }
                    
                    j++;
               }
               i++;
            }
            deleteMarkedEntities();

       }


        

        //Since the world owns all of its entities, they should be deleted alongside it.
        ~World(){
            clear();
        }

        // The world should not be copyable
        World(const World&) = delete;
        World &operator=(World const &) = delete;
    };

}