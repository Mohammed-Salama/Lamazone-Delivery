#include "forward-renderer.hpp"
#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"
#include<iostream>

#include <glm/gtx/euler_angles.hpp>
namespace our {


    glm::mat4 ForwardRenderer::toMat4(glm::vec3 scale,glm::vec3 rotation,glm::vec3 position) {
        //DONE: (Req 2) Write this function
        glm::mat4 sca = glm::scale(glm::mat4(1.0f), scale);
        glm::mat4 rot = glm::yawPitchRoll(rotation.y,rotation.x, rotation.z);
        glm::mat4 tra = glm::translate(glm::mat4(1.0f), position);
        return tra*rot*sca; // Since the transformation matrix on the right is applied first.
        // i.e. T * R * S 
    }

    void ForwardRenderer::initialize(glm::ivec2 windowSize, const nlohmann::json& config){
        // First, we store the window size for later use
        this->windowSize = windowSize;

        // Then we check if there is a sky texture in the configuration
        if(config.contains("sky")){
            // First, we create a sphere which will be used to draw the sky
            this->skySphere = mesh_utils::sphere(glm::ivec2(16, 16));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* skyShader = new ShaderProgram();
            skyShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            skyShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            skyShader->link();
            
            //DONE: (Req 9) Pick the correct pipeline state to draw the sky.
            // Hints: the sky will be draw after the opaque objects so we would need depth testing but which depth function should we pick?
            // We will draw the sphere from the inside, so what options should we pick for the face culling.

            PipelineState skyPipelineState{};
            //enabling depth testing
            skyPipelineState.depthTesting.enabled=true;
            skyPipelineState.depthTesting.function = GL_LEQUAL;
            
            //enabling face culling
            skyPipelineState.faceCulling.enabled=true;
            //face to cull is front because we see the sphere from inside
            skyPipelineState.faceCulling.culledFace = GL_FRONT;
            skyPipelineState.faceCulling.frontFace = GL_CCW;
            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string skyTextureFile = config.value<std::string>("sky", "");
            Texture2D* skyTexture = texture_utils::loadImage(skyTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* skySampler = new Sampler();
            skySampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            skySampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            skySampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Combine all the aforementioned objects (except the mesh) into a material. 
            this->skyMaterial = new TexturedMaterial();
            this->skyMaterial->shader = skyShader;
            this->skyMaterial->texture = skyTexture;
            this->skyMaterial->sampler = skySampler;
            this->skyMaterial->pipelineState = skyPipelineState;
            this->skyMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->skyMaterial->alphaThreshold = 1.0f;
            this->skyMaterial->transparent = false;
        }


         // Then we check if there is a sky texture in the configuration
        if(config.contains("wall")){
            // First, we create a sphere which will be used to draw the sky
            this->wallPlane = mesh_utils::loadOBJ(config.value<std::string>("wall-mesh", ""));
            
            // We can draw the sky using the same shader used to draw textured objects
            ShaderProgram* cityShader = new ShaderProgram();
            cityShader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
            cityShader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
            cityShader->link();
            
            PipelineState cityPipelineState{};
            //enabling depth testing
            cityPipelineState.depthTesting.enabled=true;
            cityPipelineState.depthTesting.function = GL_LEQUAL;
            
            // //enabling face culling
            // cityPipelineState.faceCulling.enabled=true;
            // //face to cull is front because we see the sphere from inside
            // cityPipelineState.faceCulling.culledFace = GL_FRONT;
            // cityPipelineState.faceCulling.frontFace = GL_CCW;
            
            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string wallTextureFile = config.value<std::string>("wall", "");
            Texture2D* wallTexture = texture_utils::loadImage(wallTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* wallSampler = new Sampler();
            wallSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            wallSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            wallSampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            wallSampler->set(GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Combine all the aforementioned objects (except the mesh) into a material. 
            this->wallMaterial = new TexturedMaterial();
            this->wallMaterial->shader = cityShader;
            this->wallMaterial->texture = wallTexture;
            this->wallMaterial->sampler = wallSampler;
            this->wallMaterial->pipelineState = cityPipelineState;
            this->wallMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->wallMaterial->alphaThreshold = 1.0f;
            this->wallMaterial->transparent = false;





            // Load the sky texture (note that we don't need mipmaps since we want to avoid any unnecessary blurring while rendering the sky)
            std::string groundTextureFile = config.value<std::string>("ground", "");
            Texture2D* groundTexture = texture_utils::loadImage(groundTextureFile, false);

            // Setup a sampler for the sky 
            Sampler* groundSampler = new Sampler();
            groundSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            groundSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            groundSampler->set(GL_TEXTURE_WRAP_S, GL_REPEAT);
            groundSampler->set(GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Combine all the aforementioned objects (except the mesh) into a material. 
            this->groundMaterial = new TexturedMaterial();
            this->groundMaterial->shader = cityShader;
            this->groundMaterial->texture = groundTexture;
            this->groundMaterial->sampler = groundSampler;
            this->groundMaterial->pipelineState = cityPipelineState;
            this->groundMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            this->groundMaterial->alphaThreshold = 1.0f;
            this->groundMaterial->transparent = false;
        }



        // Then we check if there is a postprocessing shader in the configuration
        if(config.contains("postprocess")){
            //DONE: (Req 10) Create a framebuffer
            glGenFramebuffers(1, &postprocessFrameBuffer);
            //DONE: (Req 10) Create a color and a depth texture and attach them to the framebuffer
            // Hints: The color format can be (Red, Green, Blue and Alpha components with 8 bits for each channel).
            // The depth format can be (Depth component with 24 bits).
            colorTarget = texture_utils::empty(GL_RGBA8,windowSize);
            depthTarget = texture_utils::empty(GL_DEPTH_COMPONENT24,windowSize);
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);// Bind the Framebuffer to attach the textures to it.
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTarget->getOpenGLName(), 0);
            glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTarget->getOpenGLName(), 0);
            //DONE: (Req 10) Unbind the framebuffer just to be safe
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            // Create a vertex array to use for drawing the texture
            glGenVertexArrays(1, &postProcessVertexArray);

            // Create a sampler to use for sampling the scene texture in the post processing shader
            Sampler* postprocessSampler = new Sampler();
            postprocessSampler->set(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            postprocessSampler->set(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            postprocessSampler->set(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Create the post processing shader
            ShaderProgram* postprocessShader = new ShaderProgram();
            postprocessShader->attach("assets/shaders/fullscreen.vert", GL_VERTEX_SHADER);
            postprocessShader->attach(config.value<std::string>("postprocess", ""), GL_FRAGMENT_SHADER);
            postprocessShader->link();

            // Create a post processing material
            postprocessMaterial = new TexturedMaterial();
            postprocessMaterial->shader = postprocessShader;
            postprocessMaterial->texture = colorTarget;
            postprocessMaterial->sampler = postprocessSampler;
            // The default options are fine but we don't need to interact with the depth buffer
            // so it is more performant to disable the depth mask
            postprocessMaterial->pipelineState.depthMask = false;
        }
    }

    void ForwardRenderer::destroy(){
        // Delete all objects related to the sky
        if(skyMaterial){
            delete skySphere;
            delete skyMaterial->shader;
            delete skyMaterial->texture;
            delete skyMaterial->sampler;
            delete skyMaterial;
        }
        if(wallMaterial){
            delete wallPlane;
            delete wallMaterial->shader;
            delete wallMaterial->texture;
            delete wallMaterial->sampler;
            delete wallMaterial;
        }

        if(groundMaterial){
            delete groundMaterial;
            delete groundMaterial->shader;
            delete groundMaterial->texture;
            delete groundMaterial->sampler;
            delete groundMaterial;
        }
        // Delete all objects related to post processing
        if(postprocessMaterial){
            glDeleteFramebuffers(1, &postprocessFrameBuffer);
            glDeleteVertexArrays(1, &postProcessVertexArray);
            delete colorTarget;
            delete depthTarget;
            delete postprocessMaterial->sampler;
            delete postprocessMaterial->shader;
            delete postprocessMaterial;
        }
    }

    void ForwardRenderer::render(World* world){
        // First of all, we search for a camera and for all the mesh renderers
        CameraComponent* camera = nullptr;
        std::vector<LightComponent*> lights;
        opaqueCommands.clear();
        transparentCommands.clear();
        for(auto entity : world->getEntities()){
            // If we hadn't found a camera yet, we look for a camera in this entity
            if(!camera) camera = entity->getComponent<CameraComponent>();

            if(auto lightRenderer = entity->getComponent<LightComponent>(); lightRenderer){
                lights.push_back(lightRenderer);
            }

            // If this entity has a mesh renderer component
            if(auto meshRenderer = entity->getComponent<MeshRendererComponent>(); meshRenderer){
                // We construct a command from it
                RenderCommand command;
                command.localToWorld = meshRenderer->getOwner()->getLocalToWorldMatrix();
                command.center = glm::vec3(command.localToWorld * glm::vec4(0, 0, 0, 1));
                command.mesh = meshRenderer->mesh;
                command.material = meshRenderer->material;
                // if it is transparent, we add it to the transparent commands list
                if(command.material->transparent){
                    transparentCommands.push_back(command);
                } else {
                // Otherwise, we add it to the opaque command list
                    opaqueCommands.push_back(command);
                }
            }

            
        }

        // If there is no camera, we return (we cannot render without a camera)
        if(camera == nullptr) return;

        //DONE: (Req 8) Modify the following line such that "cameraForward" contains a vector pointing the camera forward direction
        // HINT: See how you wrote the CameraComponent::getViewMatrix, it should help you solve this one
        // Transfrom from the local space to the world space.
        glm::mat4 M = camera->getOwner()->getLocalToWorldMatrix();
        glm::vec3 cameraPos    = glm::vec3(M * glm::vec4(0, 0, 0, 1 ));
        glm::vec3 cameraForward = glm::vec3(M * glm::vec4(0, 0,-1, 1 )) - glm::vec3(M * glm::vec4(0, 0, 0, 1 ));    //forward_vector = center - pos
        std::sort(transparentCommands.begin(), transparentCommands.end(), [cameraForward,cameraPos](const RenderCommand& first, const RenderCommand& second){
            //DONE: (Req 8) Finish this function
            // HINT: the following return should return true "first" should be drawn before "second".
            //First we calculate the the projection of each component on the cameraForward vector to compare the distance.
            // Proj(a on b) = (a.b)/(|b|)^2  since |b| is constant we can eliminate it.
            double firstDistance  =  glm::abs(glm::dot(first.center-cameraPos,cameraForward));
            double secondDistance =  glm::abs(glm::dot(second.center-cameraPos,cameraForward));
            if(firstDistance>secondDistance)
                return true;

            return false;
        });

        //DONE: (Req 8) Get the camera ViewProjection matrix and store it in VP
        glm::mat4 VP =  camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        //DONE: (Req 8) Set the OpenGL viewport using windowSize
        glViewport(0,0,this->windowSize[0],this->windowSize[1]);

        //DONE: (Req 8) Set the clear color to black and the clear depth to 1
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepth(1.0);
        
        //DONE: (Req 8) Set the color mask to true and the depth mask to true (to ensure the glClear will affect the framebuffer)
        glDepthMask(true);
        glColorMask(true, true, true, true);
        

        // If there is a postprocess material, bind the framebuffer
        if(postprocessMaterial){
            //DONE: (Req 10) bind the framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postprocessFrameBuffer);
            
        }

        //DONE: (Req 8) Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
       if(this->wallMaterial && this->groundMaterial){

        groundMaterial->setup();
        glm::mat4 M = camera->getOwner()->getLocalToWorldMatrix();

        int dist = 100;
        
        for(int i=0;i<300;i++){

            // street model
            glm::mat4 model = toMat4(glm::vec3(50,50,50),glm::vec3(glm::radians(90.0),0,0),glm::vec3(0,0,-1*i*dist));

            glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            groundMaterial->shader->set("transform",  VP  * model );//* model);

            wallPlane->draw();
        }

 
        dist = 30;
        for(int i=0;i<900;i++){

             wallMaterial->setup();
            if(i%10==0){

            // model3 : intersection wall (right)
 
            glm::mat4 model3 = toMat4(glm::vec3(50,20,5),glm::vec3(0,glm::radians(-180.0),glm::radians(-180.0)),glm::vec3(95,20,-1*(i*dist+15)));

            glm::mat4 VP3 = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP3  * model3 );//* model);

            wallPlane->draw();


            // model4 : intersection wall (left)

            glm::mat4 model4 = toMat4(glm::vec3(50,20,5),glm::vec3(0,glm::radians(-180.0),glm::radians(-180.0)),glm::vec3(-95,20,-1*(i*dist+15)));

            glm::mat4 VP4 = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP4  * model4 );//* model);

            wallPlane->draw();


            groundMaterial->setup();
            // model5 : intersection street (right)

            glm::mat4 model5 = toMat4(glm::vec3(50,20,5),glm::vec3(glm::radians(-90.0),glm::radians(-180.0),glm::radians(-180.0)),glm::vec3(100,0,-1*(i*dist)));

            glm::mat4 VP5 = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP5  * model5 );//* model);

            wallPlane->draw();


            // model6 : intersection street (left)

            glm::mat4 model6 = toMat4(glm::vec3(50,20,5),glm::vec3(glm::radians(-90.0),glm::radians(-180.0),glm::radians(-180.0)),glm::vec3(-100,0,-1*(i*dist)));

            glm::mat4 VP6 = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP6  * model6 );//* model);

            wallPlane->draw();



                continue;
            }


            // street walls (right)

            glm::mat4 model = toMat4(glm::vec3(15,20,5),glm::vec3(0,glm::radians(-90.0),glm::radians(-180.0)),glm::vec3(45,20,-1*i*dist));

            glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP  * model );//* model);

            wallPlane->draw();

             // street walls (left)

            glm::mat4 model2 = toMat4(glm::vec3(15,20,5),glm::vec3(0,glm::radians(-90.0),glm::radians(-180.0)),glm::vec3(-45,20,-1*i*dist));

            glm::mat4 VP2 = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
        
            wallMaterial->shader->set("transform",  VP2  * model2 );//* model);

            wallPlane->draw();

        }
        
       
       }
       
        //DONE: (Req 8) Draw all the opaque commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for(auto& model:opaqueCommands){
            glm::mat4 M = model.localToWorld;
            glm::mat4 MVP = VP * M;
            glm::mat4 M_IT = glm::transpose(glm::inverse(M));
            model.material->setup();
            if (model.material->getType()=="lighted"){
                model.material->shader->set("eye", cameraPos);
                model.material->shader->set("VP", VP);
                model.material->shader->set("M", M);
                model.material->shader->set("M_IT",M_IT);

                int n = lights.size();
                std::cout<<n<<"\n";
                model.material->shader->set("light_count",n);
                model.material->shader->set("sky.top",glm::vec3(0, 0, 0));
                model.material->shader->set("sky.middle",glm::vec3(0, 0, 0));
                model.material->shader->set("sky.bottom",glm::vec3(0, 0, 0.0));
               
                for (int i = 0 ; i < n;i++){
                    model.material->shader->set("lights["+std::to_string(i)+"].type", float(lights[i]->lightType));
                    model.material->shader->set("lights["+std::to_string(i)+"].position", lights[i]->position);
                    model.material->shader->set("lights["+std::to_string(i)+"].direction", lights[i]->direction);
                    model.material->shader->set("lights["+std::to_string(i)+"].diffuse", lights[i]->diffuse);
                    model.material->shader->set("lights["+std::to_string(i)+"].specular", lights[i]->specular);
                    model.material->shader->set("lights["+std::to_string(i)+"].attenuation", lights[i]->attenuation);
                    model.material->shader->set("lights["+std::to_string(i)+"].cone_angle", lights[i]->cone_angles);
                }
            }
            else {
                model.material->shader->set("transform", MVP);
            }
            model.mesh->draw();
        }
        // If there is a sky material, draw the sky
        if(this->skyMaterial){
            //DONE: (Req 9) setup the sky material
            skyMaterial->setup();
            //DONE: (Req 9) Get the camera position
            glm::mat4 M = camera->getOwner()->getLocalToWorldMatrix();
            
            //DONE: (Req 9) Create a model matrix for the sky such that it always follows the camera (sky sphere center = camera position)
            glm::mat4 model = glm::translate(
                glm::mat4(1.0f),
                glm::vec3(M * glm::vec4(0, 0,0, 1 ))
            );
            glm::mat4 VP = camera->getProjectionMatrix(this->windowSize) * camera->getViewMatrix();
            
            //DONE: (Req 9) We want the sky to be drawn behind everything (in NDC space, z=1)
            // We can acheive this by multiplying by an extra matrix after the projection but what values should we put in it?

            /* How this matrix makes Z = 1
            
            | X |       | 1 0 0 0 |     | X |  
            | Y |       | 0 1 0 0 |     | Y |
            | 1 |   =   | 0 0 0 1 |  x  | Z |
            | 1 |       | 0 0 0 1 |     | 1 |

            */
            glm::mat4 alwaysBehindTransform = glm::mat4(
            //  Row1, Row2, Row3, Row4
                1.0f, 0.0f, 0.0f, 0.0f, // Column1
                0.0f, 1.0f, 0.0f, 0.0f, // Column2
                0.0f, 0.0f, 0.0f, 0.0f, // Column3
                0.0f, 0.0f, 1.0f, 1.0f  // Column4
            );
           
            
            //DONE: (Req 9) set the "transform" uniform
            skyMaterial->shader->set("transform",    alwaysBehindTransform * VP  * model    );//* model);
            //DONE: (Req 9) draw the sky sphere
            skySphere->draw();
        }
        //DONE: (Req 8) Draw all the transparent commands
        // Don't forget to set the "transform" uniform to be equal the model-view-projection matrix for each render command
        for(auto& model:transparentCommands){
            glm::mat4 M = model.localToWorld;
            glm::mat4 MVP = VP * M;
            glm::mat4 M_IT = glm::transpose(glm::inverse(M));
            model.material->setup();
            if (model.material->getType()=="lighted"){
                model.material->shader->set("eye", cameraPos);
                model.material->shader->set("VP", VP);
                model.material->shader->set("M", M);
                model.material->shader->set("M_IT", M_IT);

                int n = lights.size();
                model.material->shader->set("light_count", n);
                for (int i = 0 ; i < n;i++){
                    model.material->shader->set("lights["+std::to_string(i)+"].type", float(lights[i]->lightType));
                    model.material->shader->set("lights["+std::to_string(i)+"].position", lights[i]->position);
                    model.material->shader->set("lights["+std::to_string(i)+"].direction", lights[i]->direction);
                    model.material->shader->set("lights["+std::to_string(i)+"].diffuse", lights[i]->diffuse);
                    model.material->shader->set("lights["+std::to_string(i)+"].specular", lights[i]->specular);
                    model.material->shader->set("lights["+std::to_string(i)+"].attenuation", lights[i]->attenuation);
                    model.material->shader->set("lights["+std::to_string(i)+"].cone_angle", lights[i]->cone_angles);
                }
            }
            else {
                model.material->shader->set("transform", MVP);
            }
            model.mesh->draw();
        }

        // If there is a postprocess material, apply postprocessing
        if(postprocessMaterial){
            //DONE: (Req 10) Return to the default framebuffer
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            
            //DONE: (Req 10) Setup the postprocess material and draw the fullscreen triangle
            postprocessMaterial->setup();
            glBindVertexArray(postProcessVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }
    }

}