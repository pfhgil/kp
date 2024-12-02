//
// Created by stuka on 29.11.2024.
//

#include "main.h"

#include <SGCore/Scene/Scene.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>
#include <SGCore/Scene/EntityBaseInfo.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Transformations/Controllable3D.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Graphics/API/ICubemapTexture.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Render/Atmosphere/Atmosphere.h>
#include <SGCore/Render/SpacePartitioning/IgnoreOctrees.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>
#include <SGCore/Utils/StringInterpolation/InterpolationResolver.h>
#include <SGCore/ImGuiWrap/Views/IView.h>
#include "MainView.h"
#include "Styles/StylesManager.h"
#include "Client.h"

using namespace SGCore;

void Main::update(const double& dt, const double& fixedDt) noexcept
{
    ImGuiWrap::ImGuiLayer::beginFrame();
    ImGuiWrap::IView::getRoot()->render();
    ImGuiWrap::ImGuiLayer::endFrame();
}

void Main::fixedUpdate(const double& dt, const double& fixedDt) noexcept
{

}

void Main::init() noexcept
{
    Client::start();

    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());
    PathInterpolationMarkup::getGlobalMarkup()["projectPath"] = "F:/Pixelfield/kp4";

    SGE::StylesManager::init();

    s_mainView = MakeRef<MainView>();
    ImGuiWrap::IView::getRoot()->addChild(s_mainView);

    ImGui::LoadIniSettingsFromDisk(Utils::toUTF8((InterpolatedPath("${projectPath}/Resources/configs/ui.ini").resolved()).u16string()).c_str());
}

SGCore::Ref<MainView> Main::getMainView() noexcept
{
    return s_mainView;
}

int main()
{
    CoreMain::onInit += Main::init;
    CoreMain::getRenderTimer().onUpdate += Main::update;
    CoreMain::getFixedTimer().onUpdate += Main::fixedUpdate;
    CoreMain::start();
}
