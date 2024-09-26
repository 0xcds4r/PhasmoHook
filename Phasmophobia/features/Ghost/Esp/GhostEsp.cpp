#include "GhostEsp.hpp"
#include "../Ghost.hpp"
#include "../../../GUI.h"
#include "../../../library/Console.hpp"

auto GhostEsp::GetInfo() const -> const GuiInfo& { return *new GuiInfo{ reinterpret_cast<const char*>(u8"(Ghost)"), reinterpret_cast<const char*>(u8"(Esp)"), true, false, true }; }

auto DrawGhostBoxAndInfo(const ImVec2& point, float distance, float boxSizeMax, const char* str_info) -> void {
    float boxSize = boxSizeMax * std::clamp(1.0f / distance, 0.1f, 1.00f);
    float boxWidth = boxSize * 1.75f;
    float boxHeight = boxSize * 5.00f;

    if (distance >= 1.00f) {
        DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
            { point.x - (ImGui::CalcTextSize(str_info).x * 0.55F),
            ((point.y - distance) - boxHeight / 1.05f) }, str_info, ImColor(rand() % 255, rand() % 255, rand() % 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));
    }
}

auto GhostEsp::DrawGhostStateInfo(float distance) -> void 
{
    if (!GhostAI::ghost) {
        LOG_FMT_DEBUG("GhostAI::ghost is null");
        return;
    }

    try {
        auto screenWidth = init_space::Info::w;
        auto screenHeight = init_space::Info::h;

        ImVec2 textPos = { screenWidth * 0.5f, screenHeight - 30.0f };
        ImVec2 textPosAtStart = { screenWidth * 0.005f, screenHeight - 30.0f };

        static bool bHunting = false;

        // Проверка на нулевые указатели перед доступом к полям
        if (GhostAI::ghost->isHunting && !bHunting) {
            Gui::AddNotify("[!] HUNTING START", "The ghost has started the hunt", 3.5f, ImColor(255, 255, 255, 255), ImColor(249, 36, 14, 150));
            bHunting = true;
        }

        if (!GhostAI::ghost->isHunting && bHunting) {
            Gui::AddNotify("[!] HUNTING END", "The ghost stopped the hunt", 3.5f, ImColor(255, 255, 255, 255), ImColor(0, 185, 204, 150));
            bHunting = false;
        }

        if (GhostAI::ghost->ghostInfo) {
            auto ghostState = !GhostAI::ghost->isHunting ? GhostAI::ghost->ghostState : GhostAI::GhostState::Hunting;
            auto ghostType = static_cast<GhostAI::GhostType>(static_cast<int>(GhostAI::ghost->ghostInfo->ghostType) + 1);
            auto favRoom = GhostAI::favRoom.c_str();

            const char* strInfo = std::format("Ghost Type: {} | Fav Room: {} | Ghost State: {} | Ghost Distance: {:.2f}",
                ghostType, favRoom, ghostState, distance
            ).c_str();

            ImVec2 textSize = ImGui::CalcTextSize(strInfo);
            textPos.x -= textSize.x * 0.5f;

            DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
                textPos, strInfo, ImColor(255, 255, 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));

            /*if (LevelController::gLevelController) {
                auto pRoom = GhostAI::ghost->ghostInfo->room;
                if (pRoom && pRoom->name) {
                    textPos.y -= 30.0f;
                    std::string formatMessage = std::format("Room: {}", pRoom->name->ToString().c_str());
                    DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
                        textPos, formatMessage.c_str(), ImColor(255, 255, 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));
                }
            }*/
        }
        else {
            LOG_FMT_DEBUG("GhostAI::ghostInfo is null");
        }
    }
    catch (const std::runtime_error& e) {
        LOG_FMT_DEBUG("Runtime error: {}", e.what());
    }
    catch (const std::exception& e) {
        LOG_FMT_DEBUG("Exception: {}", e.what());
    }
    catch (...) {
        LOG_FMT_DEBUG("Unknown exception occurred in DrawGhostStateInfo");
    }
}


const std::vector<std::tuple<HumanBodyBones, HumanBodyBones, const char*>> bonePairs = {
        { HumanBodyBones::Head, HumanBodyBones::Spine, "Head -> UpperChest" },
        { HumanBodyBones::Spine, HumanBodyBones::Hips, "Spine -> Hips" },
        { HumanBodyBones::LeftShoulder, HumanBodyBones::LeftUpperArm, "LeftShoulder -> LeftUpperArm" },
        { HumanBodyBones::LeftUpperArm, HumanBodyBones::LeftLowerArm, "LeftUpperArm -> LeftLowerArm" },
        { HumanBodyBones::LeftLowerArm, HumanBodyBones::LeftHand, "LeftLowerArm -> LeftHand" },
        { HumanBodyBones::RightShoulder, HumanBodyBones::RightUpperArm, "RightShoulder -> RightUpperArm" },
        { HumanBodyBones::RightUpperArm, HumanBodyBones::RightLowerArm, "RightUpperArm -> RightLowerArm" },
        { HumanBodyBones::RightLowerArm, HumanBodyBones::RightHand, "RightLowerArm -> RightHand" },
        { HumanBodyBones::LeftUpperLeg, HumanBodyBones::LeftLowerLeg, "LeftUpperLeg -> LeftLowerLeg" },
        { HumanBodyBones::LeftLowerLeg, HumanBodyBones::LeftFoot, "LeftLowerLeg -> LeftFoot" },
        { HumanBodyBones::RightUpperLeg, HumanBodyBones::RightLowerLeg, "RightUpperLeg -> RightLowerLeg" },
        { HumanBodyBones::RightLowerLeg, HumanBodyBones::RightFoot, "RightLowerLeg -> RightFoot" }
};

auto DrawBoneConnections(II::Animator* pAnimator, float distance) -> void 
{
    if (!pAnimator) {
        LOG_FMT_DEBUG("pAnimator is null");
        return;
    }

    auto pMainCamera = II::Camera::GetMain();
    if (!pMainCamera) {
        LOG_FMT_DEBUG("Main camera is null");
        return;
    }

    try {
        static float firstDistance = distance;

        if (bonePairs.empty()) {
            LOG_FMT_DEBUG("bonePairs is empty");
            return;
        }

        for (const auto& [boneStart, boneEnd, label] : bonePairs) {
            auto pStartTransform = pAnimator->GetBoneTransformInternal(boneStart);
            auto pEndTransform = pAnimator->GetBoneTransformInternal(boneEnd);

            if (pStartTransform && pEndTransform) {
                auto pointStart = pMainCamera->WorldToScreenPoint(pStartTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
                auto pointEnd = pMainCamera->WorldToScreenPoint(pEndTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);

                if (pointStart.z > 0 && pointEnd.z > 0) {
                    constexpr float minDist = 1.0f;
                    float maxDist = firstDistance;

                    float normalizedDistance = std::clamp((distance - minDist) / (maxDist - minDist), 0.0f, 1.0f);
                    ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f - normalizedDistance, normalizedDistance, 0.0f, 1.0f));

                    pointStart.y = init_space::Info::h - pointStart.y;
                    pointEnd.y = init_space::Info::h - pointEnd.y;

                    if (pointStart.x <= 0.0f || pointStart.x >= init_space::Info::w - 1.0f ||
                        pointEnd.x <= 0.0f || pointEnd.x >= init_space::Info::w - 1.0f ||
                        pointStart.y <= 0.0f || pointStart.y >= init_space::Info::h - 1.0f ||
                        pointEnd.y <= 0.0f || pointEnd.y >= init_space::Info::h - 1.0f) 
                    {
                        //LOG_FMT_DEBUG("Point(s) out of bounds: pointStart: ({:.2f}, {:.2f}), pointEnd: ({:.2f}, {:.2f})",
                        //    pointStart.x, pointStart.y, pointEnd.x, pointEnd.y);
                        continue;
                    }

                    ImGui::GetBackgroundDrawList()->AddLine(
                        { pointStart.x, pointStart.y },
                        { pointEnd.x, pointEnd.y },
                        color,
                        1.5f
                    );
                }
            }
        }
    }
    catch (const std::runtime_error& e) {
        LOG_FMT_DEBUG("Runtime error in DrawBoneConnections: {}", e.what());
    }
    catch (const std::exception& e) {
        LOG_FMT_DEBUG("Exception in DrawBoneConnections: {}", e.what());
    }
    catch (...) {
        LOG_FMT_DEBUG("Unknown exception occurred in DrawBoneConnections");
    }
}

auto DrawTextFromPosition(const char* text, II::Vector3 position) -> void
{
    auto pMainCamera = II::Camera::GetMain();
    if (!pMainCamera) {
        LOG_FMT_DEBUG("Main camera is null");
        return;
    }

    try {
        auto cameraPosition = pMainCamera->GetTransform()->GetPosition();
        auto pointStart = pMainCamera->WorldToScreenPoint(position, UnityResolve::UnityType::Camera::Eye::Mono);

        if (pointStart.z <= 0) {
            return;
        }

        pointStart.y = init_space::Info::h - pointStart.y;

        if (pointStart.x < 0.0f || pointStart.x >= init_space::Info::w ||
            pointStart.y < 0.0f || pointStart.y >= init_space::Info::h) {
            return;
        }

        ImVec2 textPosition = { pointStart.x, pointStart.y };
        const char* fmtText = std::format("{}", text).c_str();
        ImGui::GetBackgroundDrawList()->AddText(textPosition, ImColor(255, 255, 255, 255), fmtText);

        //ImGui::GetBackgroundDrawList()->AddLine({ init_space::Info::w * 0.5f, init_space::Info::h * 0.5f },
            //{ textPosition.x, textPosition.y }, ImColor(255, 255, 255, 255), 2.0f);
    }
    catch (const std::runtime_error& e) {
        LOG_FMT_DEBUG("Runtime error in DrawTextFromPosition: {}", e.what());
    }
    catch (const std::exception& e) {
        LOG_FMT_DEBUG("Exception in DrawTextFromPosition: {}", e.what());
    }
    catch (...) {
        LOG_FMT_DEBUG("Unknown exception occurred in DrawTextFromPosition");
    }
}


auto DrawCameraLinesAndCircles(II::Animator* pAnimator, float distance) -> void
{
    if (!pAnimator) {
        LOG_FMT_DEBUG("pAnimator is null");
        return;
    }

    auto pStartTransform = pAnimator->GetBoneTransformInternal(HumanBodyBones::Spine);
    if (!pStartTransform) {
        LOG_FMT_DEBUG("pStartTransform is null");
        return;
    }

    auto pMainCamera = II::Camera::GetMain();
    if (!pMainCamera) {
        LOG_FMT_DEBUG("Main camera is null");
        return;
    }

    try { 
        auto pointStart = pMainCamera->WorldToScreenPoint(pStartTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);

        if (pointStart.z <= 0) {
            return; 
        }

        pointStart.y = init_space::Info::h - pointStart.y;

        if (pointStart.x < 0.0f || pointStart.x >= init_space::Info::w ||
            pointStart.y < 0.0f || pointStart.y >= init_space::Info::h) {
            return;
        }

        constexpr float minDist = 1.0f;
        static float firstDistance = distance;
        float maxDist = firstDistance;

        float normalizedDistance = std::clamp((distance - minDist) / (maxDist - minDist), 0.0f, 1.0f);
        ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f - normalizedDistance, normalizedDistance, 0.0f, 1.0f));

        ImGui::GetBackgroundDrawList()->AddLine({ init_space::Info::w * 0.5f, init_space::Info::h * 0.5f },
            { pointStart.x, pointStart.y }, color, 2.0f);

        ImVec2 textPosition = { init_space::Info::w * 0.5125f, init_space::Info::h * 0.5125f };
        ImGui::GetBackgroundDrawList()->AddText(textPosition, color, std::format("{:.2f}", distance).c_str());
    }
    catch (const std::runtime_error& e) {
        LOG_FMT_DEBUG("Runtime error in DrawCameraLinesAndCircles: {}", e.what());
    }
    catch (const std::exception& e) {
        LOG_FMT_DEBUG("Exception in DrawCameraLinesAndCircles: {}", e.what());
    }
    catch (...) {
        LOG_FMT_DEBUG("Unknown exception occurred in DrawCameraLinesAndCircles");
    }
}


auto GhostEsp::Draw() -> void {
    if (!GhostAI::ghost) return;

    try {
        auto [ghostPos, cameraPos, distance, boxSizeMax] = CalculateGhostData();
        //if (cameraPos.x == 0.0f && cameraPos.y == 0.0f && cameraPos.z == 0.0f) return;
        //if (ghostPos.x == 0.0f && ghostPos.y == 0.0f && ghostPos.z == 0.0f) return;

        auto camera = II::Camera::GetMain();
        if (!camera) return;

        auto point = camera->WorldToScreenPoint(
            ghostPos,
            UnityResolve::UnityType::Camera::Eye::Mono
        );

        bool isInView = point.z > 0.0f;
        point.y = init_space::Info::h - point.y;

        if (isInView) {
            HandleVisibleGhost(ghostPos, distance);
        }
        else {
            if (bStateInfo) {
                DrawGhostStateInfo(distance);
            }
        }
    }
    catch (const std::runtime_error& e) {
        LOG_FMT_DEBUG("Runtime error: {}", e.what());
    }
    catch (const std::exception& e) {
        LOG_FMT_DEBUG("Exception: {}", e.what());
    }
    catch (...) {
        LOG_FMT_DEBUG("Unknown exception occurred");
    }
}

auto GhostEsp::CalculateGhostData() -> std::tuple<II::Vector3, II::Vector3, float, float> {
    auto ghostTransform = GhostAI::ghost->GetTransform();
    if (!ghostTransform) return {};

    auto camera = II::Camera::GetMain();
    if (!camera) return {};

    auto ghostPos = ghostTransform->GetPosition();
    auto cameraTransform = camera->GetTransform();
    if (!cameraTransform) return {};

    auto cameraPos = cameraTransform->GetPosition();
    auto ghostScale = ghostTransform->GetLocalScale();

    auto result = ghostPos - cameraPos;
    float distance = std::sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
    float ghostScaleMagnitude = std::sqrt(ghostScale.x * ghostScale.x + ghostScale.y * ghostScale.y + ghostScale.z * ghostScale.z);

    return { ghostPos, cameraPos, std::abs(distance - ghostScaleMagnitude), ghostScaleMagnitude * 100.0f };
}

void GhostEsp::HandleVisibleGhost(const II::Vector3& ghostPos, float distance) {
    if (bStateInfo) {
        DrawGhostStateInfo(distance);
    }

    if (bBoneWallHack || bEsp) {
        DrawGhostBonesAndEMF(distance);
    }
}

void GhostEsp::DrawGhostBonesAndEMF(float distance) {
    FOR_EACH_COMPONENT(GhostAI::ghost, II::Animator, "UnityEngine.AnimationModule.dll", "Animator", pAnimator) {
        if (pAnimator) {
            if (bBoneWallHack) {
                DrawBoneConnections(pAnimator, distance);
            }

            if (bEsp) {
                DrawCameraLinesAndCircles(pAnimator, distance);
            }
        }
    }

    ManageEMFList();
    DrawDNAIfPresent();
}

void GhostEsp::ManageEMFList() {
    std::vector<EMF*> toRemove{};
    if (!GhostAI::pEMF.empty()) {
        auto now = std::chrono::steady_clock::now();

        for (const auto& [emf, timestamp] : GhostAI::pEMF) {
            if (emf) {
                FOR_EACH_COMPONENT(emf, II::Transform, "UnityEngine.CoreModule.dll", "Transform", pTransform) {
                    if (pTransform && bDrawEMF) {
                        DrawTextFromPosition("EMF", pTransform->GetPosition());
                    }
                }

                auto seconds_passed = std::chrono::duration_cast<std::chrono::seconds>(now - timestamp).count();

                if (seconds_passed >= 20) {
                    toRemove.push_back(emf);
                }
            }
        }

        for (EMF* emf : toRemove) {
            if (emf) {
                GhostAI::pEMF.erase(
                    std::remove_if(GhostAI::pEMF.begin(), GhostAI::pEMF.end(),
                        [emf](const auto& tuple) {
                            return std::get<0>(tuple) == emf;
                        }), GhostAI::pEMF.end());
            }
        }

        toRemove.clear();
    }
}

void GhostEsp::DrawDNAIfPresent() {
    if (GhostAI::pDNA && bDrawBone) {
        FOR_EACH_COMPONENT(GhostAI::pDNA, II::Transform, "UnityEngine.CoreModule.dll", "Transform", pTransform) {
            if (pTransform) {
                DrawTextFromPosition("Bone", pTransform->GetPosition());
            }
        }
    }
}


auto GhostEsp::Render() -> void {
    if (ImGui::Checkbox("Ghost info", &bStateInfo)) {
        Gui::AddNotify("Ghost Information", bStateInfo ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
    }
    if (ImGui::Checkbox("ESP Line", &bEsp)) {
        Gui::AddNotify("Esp Line", bEsp ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
    }
    if (ImGui::Checkbox("Ghost WallHack", &bBoneWallHack)) {
        Gui::AddNotify("Ghost WallHack", bBoneWallHack ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
    }
    if (ImGui::Checkbox("Bone", &bDrawBone)) {
        Gui::AddNotify("Bone", bDrawBone ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
    }
    if (ImGui::Checkbox("EMF Data", &bDrawEMF)) {
        Gui::AddNotify("EMF Data", bDrawEMF ? "Enabled" : "Disabled", 3.5f, ImColor(255, 255, 255, 255), ImColor(32, 68, 93, 220));
    }
}
auto GhostEsp::Update() -> void { Feature::Update(); }
auto GhostEsp::Save(nlohmann::json& json) -> void {
    Feature::Save(json);
}
auto GhostEsp::Load(nlohmann::json& json) -> void {
    Feature::Load(json);
}

auto GhostEsp::GetInstance() -> GhostEsp* {
	static GhostEsp ghost_esp;
	return &ghost_esp;
}

/*for (const auto pAnimator : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::Animator*>(I::Get("UnityEngine.AnimationModule.dll")->Get("Animator"))) {
              auto pHeadTransform = pAnimator->GetBoneTransformInternal(HumanBodyBones::Head);
              if (pHeadTransform)
              {
                  if (auto pointHead = II::Camera::GetMain()->WorldToScreenPoint(pHeadTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono); pointHead.z > 0)
                  {
                      pointHead.y = init_space::Info::h - pointHead.y;
                      ImGui::GetBackgroundDrawList()->AddCircle(
                          { pointHead.x, pointHead.y - distance },  // Circle center at the second point
                          5.0f,  // Radius of the circle
                          ImColor(0, 255, 0),  // Random color
                          0,  // Segments, 0 means auto
                          2.0f  // Thickness of the circle's outline
                      );
                  }
              }
          }*/
          // point.y -= 20.0f;
                       //ImVec2 topLeft = { point.x - boxWidth / 2, point.y - boxHeight / 2 };
                       //ImVec2 bottomRight = { point.x + boxWidth / 2, point.y + boxHeight / 2 };
                       //point.y += 20.0f;

                       //if (distance >= 1.00f) ImGui::GetBackgroundDrawList()->AddRect(topLeft, bottomRight, ImColor(rand() % 255, rand() % 255, rand() % 255), 0.0f, 0, 2.0f);

                       // todo
                       //for (const auto pSkinnedMeshRenderer : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::SkinnedMeshRenderer*>(I::Get("UnityEngine.CoreModule.dll")->Get("SkinnedMeshRenderer"))) {
                           //auto bones = pSkinnedMeshRenderer->GetBones();
                           // public static global::UnityEngine.Transform[] GetBonesOfTypeAndSide(global::RootMotion.BipedNaming.BoneType boneType, global::RootMotion.BipedNaming.BoneSide boneSide, global::UnityEngine.Transform[] bones)
                           //GetBonesOfTypeAndSide(Head, Center, bones);
                           // todo
                       //}

                       //for (const auto pAnimator : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::Animator*>(I::Get("UnityEngine.AnimationModule.dll")->Get("Animator"))) {

                       //}

          //for (const auto pSkinnedMeshRenderer : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::SkinnedMeshRenderer*>(I::Get("UnityEngine.CoreModule.dll")->Get("SkinnedMeshRenderer"))) {
                            //auto bones = pSkinnedMeshRenderer->GetBones();
                            //if (bones.empty()) {
                            //    std::cerr << "No bones found!" << std::endl;
                            //    continue;
                            //}



                           // Line* lines;

                            /*for (const auto& bone : bones) {
                                if (auto bonePoint = II::Camera::GetMain()->WorldToScreenPoint(bone->GetTransform()->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono); bonePoint.z > 0) {
                                    bonePoint.y = init_space::Info::h - bonePoint.y;


                                    // Calculate distance
                                    //float dx = point.x - bonePoint.x;
                                    //float dy = point.y - bonePoint.y;
                                    float distance = std::sqrt(bonePoint.x * bonePoint.x + bonePoint.y * bonePoint.y);

                                    // Check distance and add line if close
                                    if (distance < 10.0f) { // Adjust the threshold distance as needed
                                        boneScreenPoints.push_back({ bonePoint.x, bonePoint.y });
                                    }
                                }
                            }*/
                            //}

                            // Draw lines
                            //auto drawList = ImGui::GetBackgroundDrawList();
                            //for (int i = boneScreenPoints.size() - 1; i != 0; i -= 2) {
                               // drawList->AddLine(boneScreenPoints[i - 1], boneScreenPoints[i], ImColor(0, 255, 0), 2.0f); // Adjust color and thickness as needed
                            //}



          //auto i = 0;
                      //for (const auto pCollider : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::Renderer*>(I::Get("UnityEngine.CoreModule.dll")->Get("Renderer"))) {
                          //if (i == 0 || i == 2 || i == 3 || i == 8) drawMath::DrawBox(pCollider, ImColor{ 255, 0, 0 });
                          //i++;
                      //}