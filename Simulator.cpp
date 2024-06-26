#include "Simulator.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/polar_coordinates.hpp>
#include "Scenes/SceneIndex.h"

void Simulator::init()
{
    // get vector of all avaiable scene names
    for (auto &scene : scenesCreators)
    {
        sceneNames.push_back(scene.first);
    }
    if (sceneNames.size() <= 0)
        throw std::runtime_error("No scenes available");
    currentSceneName = sceneNames[0];
    currentScene = scenesCreators[currentSceneName]();
    currentScene->init();
}

void Simulator::simulateStep()
{
    currentScene->simulateStep();
}

void Simulator::onGUI()
{
    ImGui::Begin("Game Physics");
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::Text("Scene");
    if (ImGui::BeginCombo("Scene", currentSceneName.c_str()))
    {
        for (auto &sceneName : sceneNames)
        {
            bool isSelected = (currentSceneName == sceneName);
            if (ImGui::Selectable(sceneName.c_str(), isSelected))
            {
                currentSceneName = sceneName;
                currentScene = scenesCreators[currentSceneName]();
                currentScene->init();
            }
            if (isSelected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }
    if (ImGui::Button("Reset Scene"))
    {
        currentScene = scenesCreators[currentSceneName]();
    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader(currentSceneName.c_str()))
    {
        currentScene->onGUI();
    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Rendering"))
    {
        ImGui::ColorEdit3("Background Color", glm::value_ptr(renderer.backgroundColor));
        ImGui::Separator();

        ImGui::DragFloat3("Light Position", glm::value_ptr(renderer.m_lightingUniforms.direction), 0.01f);
        ImGui::Separator();
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(renderer.m_lightingUniforms.ambient));
        ImGui::DragFloat("Ambient Intensity", &renderer.m_lightingUniforms.ambient_intensity, 0.01f, 0.0, 1.0);
        ImGui::Separator();
        ImGui::ColorEdit3("Specular Color", glm::value_ptr(renderer.m_lightingUniforms.specular));
        ImGui::DragFloat("Specular Intensity", &renderer.m_lightingUniforms.specular_intensity, 0.01f, 0.0, 1.0);
        ImGui::DragFloat("Specular Alpha", &renderer.m_lightingUniforms.alpha, 0.1f, 0.0, 100.0);
        ImGui::Separator();
        ImGui::DragFloat("Diffuse Intensity", &renderer.m_lightingUniforms.diffuse_intensity, 0.01f, 0.0, 1.0);
    }

    ImGui::End();
}

void Simulator::onDraw()
{
    renderer.drawWireCube({0, 0, 0}, {5, 5, 5}, {1, 1, 1});
    currentScene->onDraw(renderer);
};