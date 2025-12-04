#include "Scene01.h"
#include "thirdparty/glm/gtx/quaternion.hpp"

void Scene01::onDraw(Renderer& renderer){
    renderer.drawWireCube(glm::vec3(0), glm::vec3(5), glm::vec3(1));

    renderer.drawCube(  glm::vec3(0,0,0), 
                        glm::quat(glm::vec3(pitch, roll, yaw)), // rotation now given via Euler angles
                        glm::vec3(0.5,0.5,0.5), 
                        glm::vec4(1,0,0,1));

    // Achsen und Rotation + deren Visualisierung
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

    renderer.drawLine(glm::vec3(0), forward, glm::vec4(1, 0, 0, 1));
    renderer.drawLine(glm::vec3(0), right, glm::vec4(0, 1, 0, 1));
    renderer.drawLine(glm::vec3(0), up, glm::vec4(0, 0, 1, 1));

    // Partikel
    auto cmap = Colormap("viridis");

    for (auto& particle : particles){
        renderer.drawSphere(particle.position, 0.1f, /*particle.color*/ glm::vec4(cmap(particle.lifetime / particleLifetime), 1)); // Danke, dass ihr mir bzgl. magic numbers zustimmt :D
    }
    
    cameraMatrix = renderer.camera.viewMatrix;
    fwd = inverse(cameraMatrix) * glm::vec4(0, 0, 1, 0);
    right = inverse(cameraMatrix) * glm::vec4(1, 0, 0, 0);
    up = inverse(cameraMatrix) * glm::vec4(0, 1, 0, 0);
}

void Scene01::simulateStep(){
/*
    pitch += pitch_increment;
    roll += roll_increment;
    yaw += yaw_increment;
*/
    glm::vec3 gravityAccel = glm::vec3(0, 0, -9.81f);
    
    float realtimeDt = ImGui::GetIO().DeltaTime;

    for (auto& particle : particles){
        particle.position += /* 0.01f */ realtimeDt * particle.velocity; // ̶d̶̶a̶̶s̶̶ ̶̶i̶̶s̶̶t̶̶ ̶̶g̶̶o̶̶t̶̶t̶̶l̶̶o̶̶s̶̶e̶̶r̶̶ ̶̶c̶̶o̶̶d̶̶e̶̶.̶̶ ̶̶w̶̶d̶̶y̶̶m̶̶ ̶̶v̶̶e̶̶l̶̶o̶̶c̶̶i̶̶t̶̶y̶̶ ̶̶m̶̶i̶̶t̶̶ ̶̶e̶̶i̶̶n̶̶e̶̶r̶̶ ̶̶m̶̶a̶̶g̶̶i̶̶c̶̶ ̶̶n̶̶u̶̶m̶̶b̶̶e̶̶r̶̶ ̶̶m̶̶u̶̶l̶̶t̶̶i̶̶p̶̶l̶̶i̶̶z̶̶i̶̶e̶̶r̶̶e̶̶n̶̶?̶̶?̶
        particle.lifetime += /* 0.01f */ realtimeDt; // ̶i̶̶c̶̶h̶̶ ̶̶n̶̶e̶̶h̶̶m̶̶e̶̶ ̶̶a̶̶n̶̶ ̶̶d̶̶a̶̶s̶̶ ̶̶i̶̶s̶̶t̶̶ ̶̶e̶̶i̶̶n̶̶e̶̶ ̶̶p̶̶r̶̶i̶̶m̶̶i̶̶t̶̶i̶̶v̶̶e̶̶ ̶̶d̶̶e̶̶l̶̶t̶̶a̶̶t̶̶i̶̶m̶̶e̶̶ ̶̶a̶̶p̶̶p̶̶r̶̶o̶̶x̶̶i̶̶m̶̶a̶̶t̶̶i̶̶o̶̶n̶̶?̶̶ ̶̶o̶̶d̶̶e̶̶r̶̶ ̶̶w̶̶i̶̶r̶̶d̶̶ ̶̶d̶̶e̶̶r̶̶ ̶̶c̶̶o̶̶d̶̶e̶̶ ̶̶h̶̶i̶̶e̶̶r̶̶ ̶̶u̶̶n̶̶a̶̶b̶̶h̶̶ä̶̶n̶̶g̶̶i̶̶g̶̶ ̶̶v̶̶o̶̶n̶̶ ̶̶d̶̶e̶̶r̶̶ ̶̶b̶̶i̶̶l̶̶d̶̶r̶̶a̶̶t̶̶e̶̶ ̶̶a̶̶u̶̶s̶̶g̶̶e̶̶f̶̶ü̶̶h̶̶r̶̶t̶̶?̶
        particle.velocity += /* 0.01f */ realtimeDt * gravityAccel;
    }

    auto setLifetime = particleLifetime;
    particles.erase(std::remove_if(particles.begin(), particles.end(), [](const Particle& particle){
        return particle.lifetime > particleLifetime;
    }), particles.end());


    if(ImGui::IsKeyDown(ImGuiKey_Space))
        launchSphere();
    if(ImGui::IsKeyDown(ImGuiKey_W))
        pitch += pitch_increment;
    if(ImGui::IsKeyDown(ImGuiKey_S))
        pitch -= pitch_increment;
    if(ImGui::IsKeyDown(ImGuiKey_A))
        roll += roll_increment;
    if(ImGui::IsKeyDown(ImGuiKey_D))
        roll -= roll_increment;
    if(ImGui::IsKeyDown(ImGuiKey_Q))
        yaw += yaw_increment;
    if(ImGui::IsKeyDown(ImGuiKey_E))
        yaw -= yaw_increment;
    
    if(ImGui::IsMouseReleased(ImGuiMouseButton_Right)){   
        auto drag = ImGui::GetMouseDragDelta(1);
        if(!(drag.x == 0 && drag.y == 0)){
            auto dx = drag.x * right;
            auto dy = -drag.y * up;
            for (auto& particle : particles){
                particle.velocity += (dx + dy) * realtimeDt;
                particle.lifetime = 0.f;
            }
        }
    }

    lastLaunch++;
}

void Scene01::launchSphere(){
    if(lastLaunch < launch_delay)
        return;
    lastLaunch = 0;
    glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
    glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));
    glm::vec3 right = glm::vec3(rotation * glm::vec4(1, 0, 0, 0));
    glm::vec3 up = glm::vec3(rotation * glm::vec4(0, 1, 0, 0));

    glm::vec4 color = glm::vec4(dis(gen), dis(gen), dis(gen), 1);
    float velocityMagnitude = 4.5f + dis(gen);
    glm::vec3 velocity = forward * velocityMagnitude;

/*
    velocity += right * (dis(gen) - 0.5f) * 2.f;
    velocity += up * (dis(gen) - 0.5f) * 2.f;
*/
    // Maybe have the spray form a circle instead of a square? Do this by (pseudo-) randomly genrating a direction first and then rng-ing its intensity after normalization.
    glm::vec3 bloom = glm::normalize(right * (dis(gen) - 0.5f) * 2.f + up * (dis(gen) - 0.5f) * 2.f);
    velocity += bloom * (dis(gen) - 0.5f) * 2.f;

    particles.push_back(Particle{glm::vec3(0), velocity, color, .0});
}

void Scene01::onGUI(){
    ImGui::SliderFloat("Pitch Increment", &pitch_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Roll Increment", &roll_increment, -0.01f, 0.01f);
    ImGui::SliderFloat("Yaw Increment", &yaw_increment, -0.01f, 0.01f);

    ImGui::SliderInt("Launch Delay", &launch_delay, 0, 100);

    /*

    auto launch = ImGui::Button("Launch");

    if(launch){
        glm::mat4 rotation = glm::toMat4(glm::quat(glm::vec3(pitch, roll, yaw)));
        glm::vec3 forward = glm::vec3(rotation * glm::vec4(0, 0, 1, 0));

        particles.push_back(Particle{
            glm::vec3(0), // Initial Position
            forward * 5.f, // Initial Velocity
            glm::vec4(dis(gen), dis(gen), dis(gen), 1), // Color
            .0 // Particles are created with their own time counter set to 0
        });
    }

    */
}