#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/game-logic-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

//#include <bits/stdc++.h>
#include <chrono>
#include <ctime>
#include <windows.h>
#pragma comment(lib, "Winmm.lib")

namespace our
{

    // The free camera cameracontroller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic. 
    // For more information, see "common/components/free-camera-cameracontroller.hpp"
    class GameLogicControllerSystem {
        Application* app; // The application in which the state runs
    public:
        bool gameOver = false; // Indicates whether the game is over or not.
        double previousz=0;
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application* app){
            this->app = app;
        }

        bool collision_detection(Entity* entity, Entity* player, Entity* detected, bool ignoreZ=false, bool ignoreScale=false) {
            double dx,dy,dz;
            dx= player->localTransform.position.x + entity->localTransform.position.x - detected->localTransform.position.x;
            dy = player->localTransform.position.y + entity->localTransform.position.y - detected->localTransform.position.y;
            if(ignoreZ) dz = 0;
            else
                dz = player->localTransform.position.z + entity->localTransform.position.z - detected->localTransform.position.z;
            // //std::cout<<player->localTransform.position.x<<" "<<player->localTransform.position.y<<" "<<player->localTransform.position.z<<std::endl;
            // //std::cout<<detected->localTransform.position.x<<" "<<detected->localTransform.position.y<<" "<<detected->localTransform.position.z<<std::endl;
            double distance = sqrt(dx*dx + dy*dy + dz*dz)/2;
            double radiusx=0, raduisy=0;
            Mesh *meshx;
            Mesh *meshy;
            meshx = player->getComponent<MeshRendererComponent>()->mesh;
            meshy = detected->getComponent<MeshRendererComponent>()->mesh;

            double scale1 = player->localTransform.scale.x> player->localTransform.scale.y? player->localTransform.scale.x:player->localTransform.scale.y;
            scale1= scale1> player->localTransform.scale.z? scale1 : player->localTransform.scale.z;
            //std::max({player->localTransform.scale.x, player->localTransform.scale.y, player->localTransform.scale.z});
            double scale2 =detected->localTransform.scale.x> detected->localTransform.scale.y?detected->localTransform.scale.x:detected->localTransform.scale.y;
            scale2= scale2> detected->localTransform.scale.z?scale2:detected->localTransform.scale.z;
           // scale2= std::max(scale2,detected->localTransform.scale.z);

            // std::max({detected->localTransform.scale.x, detected->localTransform.scale.y, detected->localTransform.scale.z});
            if(ignoreScale)
            {
                radiusx = meshx->raduis;
                raduisy = meshy->raduis;
            }
            else 
            {
                radiusx = meshx->raduis * scale1;
                raduisy = meshy->raduis * scale2;
            }
            // //std::cout<<"radiusx"<<radiusx<<"raduisy"<<raduisy<<"distance"<<distance<<std::endl;
            return distance <=radiusx+raduisy;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent 
        void update(World* world, float deltaTime) {
            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent* camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for(auto entity : world->getEntities()){
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if(camera && controller) break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if(!(camera && controller)) return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity* entity = camera->getOwner();
            previousz = entity->localTransform.position.z;

            // NOT Assume that the first child in the player entity is the car/truck.
            Entity* player = nullptr;
            GameLogicControllerComponent *game = nullptr;
            for(auto detected : world->getEntities()){
                if(detected->materialName == "player"){
                    player = detected;
                    game = player->getComponent<GameLogicControllerComponent>();
                    break;
                }
            }
            
            if(!(player && game)) return;

            // Player already lost.
            if(game->lost) return;

            float maxSpeed = game->maxSpeed;
            float maxEnergy = game->maxEnergy;
            float &speed = game->speed;
            float &energy = game->energy;
            float decrement = game->energyDecrementValue;
            float &totalScore = game->totalScore;
            float pointsPerPackage = game->pointsPerPackage;
            float energyLostPerHit = game->energyLostPerHit;
            float cooldownTime = game->cooldownTime;
            bool &deliveryInProgress = game->deliveryInProgress;
            bool &wasHit = game->wasHit;

            std::chrono::time_point<std::chrono::system_clock> &lastDecrementTime = game->lastDecrementTime;
            std::chrono::time_point<std::chrono::system_clock> &lastHitTime = game->lastHitTime;
            std::chrono::time_point<std::chrono::system_clock> startTime = game->startTime;

            std::chrono::time_point<std::chrono::system_clock> timeNow = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds_normal = timeNow-lastDecrementTime;
            std::chrono::duration<double> elapsed_seconds_hit;
            std::chrono::duration<double> elapsed_seconds_start = timeNow - startTime;
            if(wasHit)
            {
                elapsed_seconds_hit = timeNow-lastHitTime;
                // //std::cout<<"Elapsed Time="<<elapsed_seconds_hit.count()<<std::endl;
                if(elapsed_seconds_hit.count()> cooldownTime)
                {
                    wasHit = false;
                    //std::cout<<"Cooldown Finshed!!!"<<std::endl;
                }
            }


            if(elapsed_seconds_normal.count()> 1)
            {
                //std::cout<<"Current Energy="<<energy<<" Speed ="<<speed<<std::endl;
                energy -= decrement;
                if(energy <= 0)
                {
                    game->lost = true;
                    gameOver = true;
                    energy = 0.0f;
                    speed = 0.0f;
                    return;
                }
                else 
                {
                    speed = maxSpeed * energy / maxEnergy;
                    lastDecrementTime = timeNow;
                }
            }

            // Player always moves forward according to the speed.
            glm::mat4 matrix = entity->localTransform.toMat4();
            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0));
            glm::vec3 current_sensitivity = controller->positionSensitivity;
            entity->localTransform.position += front * (deltaTime * speed);
             // detect collision with buildings
            if(entity->localTransform.position.x>32  || entity->localTransform.position.x<-32 )
            {
                if(entity->localTransform.position.x>32)
                  entity->localTransform.position.x=32;
                  else
                   entity->localTransform.position.x=-32;
                    if(!wasHit && (elapsed_seconds_start.count() > cooldownTime)){
                           wasHit = true;
                           lastHitTime = std::chrono::system_clock::now();
                           //std::cout<<"Cooldown Started!"<<std::endl;
                            //  //std::cout<<"Hit at time="<<lastHitTime<<std::endl;
                            energy -= energyLostPerHit;
                            if(energy <= 0)
                            {
                                  energy = 0;
                                  game->lost = true;
                                 gameOver = true;
                               return;
                             }
                                    
                             //std::cout<<"Energy"<<energy<<std::endl;
                                }
            }
            // limit flying of car
    
            if(entity->localTransform.position.y!=6)
            {
                entity->localTransform.position.y=6;
            }
            // car can't back
            if(entity->localTransform.position.z>previousz)
            {
                entity->localTransform.position.z=previousz;
            }

            for(auto detected : world->getEntities())
               {
                   // detect collision with cars
                    if(detected->materialName=="car")
                    {   
                            if(collision_detection(entity, player, detected))
                            {
                                if(!wasHit && (elapsed_seconds_start.count() > cooldownTime)){
                                    wasHit = true;
                                    lastHitTime = std::chrono::system_clock::now();
                                     //std::cout<<"Cooldown Started!"<<std::endl;
                                    //  //std::cout<<"Hit at time="<<lastHitTime<<std::endl;
                                    energy -= energyLostPerHit;
                                    if(energy <= 0)
                                    {
                                        energy = 0;
                                        game->lost = true;
                                        gameOver = true;
                                        return;
                                    }
                                    
                                    //std::cout<<"Energy"<<energy<<std::endl;
                                }
                            }  // remove all enities before car
                            else  if(detected->localTransform.position.z >entity->localTransform.position.z)
                            {
                              world->markForRemoval(detected);       
                            }
                          
                    }
                    //detect collision with battery
                    if(detected->materialName=="battery")
                    {   
                            // //std::cout<<"I am here"<<std::endl;
                            if(collision_detection(entity, player, detected))
                            {
                                //std::cout<<"Collision!!";  
                                //std::cout<<"Max Energy= "<<maxEnergy<<" Max Speed= "<<maxSpeed<<" Energy= "<<energy<<std::endl;
                                world->markForRemoval(detected);
                                int energyBoost = (10 *(rand() % 10));
                                energy += energyBoost;
                                totalScore += int(pointsPerPackage/5);
                                if(energy > game->maxEnergy) energy = game->maxEnergy;
                                //std::cout<<"Energy Boosted by "<<energyBoost<<std::endl;
                                // std::cout<<"Current Score="<<totalScore<<" deliveryInProgress ="<<deliveryInProgress<<std::endl;
                            }   // remove all enities before car
                            else  if(detected->localTransform.position.z >entity->localTransform.position.z)
                            {
                            world->markForRemoval(detected);       
                            }
                    }
                    // detect collion with packages
                    else if((!deliveryInProgress) && detected->materialName=="package")
                    {
                        if(collision_detection(entity, player, detected))
                        {
                            std::cout<<"Package Picked Up!!"<<std::endl;  
                            world->markForRemoval(detected);
                            deliveryInProgress = true;
                            //std::cout<<"Current Score="<<totalScore<<" deliveryInProgress ="<<deliveryInProgress<<std::endl;
                        }  // remove all enities before car
                        else  if(detected->localTransform.position.z >entity->localTransform.position.z)
                        {
                         world->markForRemoval(detected);       
                        }
                    }
                    // detect collision with dileveryzone
                    else if(deliveryInProgress && detected->materialName=="deliveryZone")
                    {
                        if(collision_detection(entity, player, detected))
                        {
                            std::cout<<"Package Delivered!!!!"<<std::endl;
                            deliveryInProgress = false;
                            totalScore += pointsPerPackage;
                            std::cout<<"Current Score="<<totalScore<<" deliveryInProgress ="<<deliveryInProgress<<std::endl;
                        }   // remove all enities before car
                        else  if(detected->localTransform.position.z >entity->localTransform.position.z)
                        {
                         world->markForRemoval(detected);       
                        }
                    }
                  
                  
                }
              
        }

        // // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit(){
        }

    };

}
