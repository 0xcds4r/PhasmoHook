#include "GhostEsp.hpp"
#include "../Ghost.hpp"
#include "../../../GUI.h"

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

auto GhostEsp::DrawGhostStateInfo(float distance) -> void {

    auto screenWidth = init_space::Info::w;
    auto screenHeight = init_space::Info::h;

    ImVec2 textPos = { screenWidth * 0.5f, screenHeight - 30.0f };
    ImVec2 textPosAtStart = { screenWidth * 0.005f, screenHeight - 30.0f };
    
    static bool bHunting = false;

    if (GhostAI::ghost->isHunting && !bHunting)
    {
        Gui::AddNotify("[!] HUNTING START", "The ghost has started the hunt", 3.5f, ImColor(255, 255, 255, 255), ImColor(249, 36, 14, 150));
        bHunting = true;
    }

    if (!GhostAI::ghost->isHunting && bHunting)
    {
        Gui::AddNotify("[!] HUNTING END", "The ghost stopped the hunt", 3.5f, ImColor(255, 255, 255, 255), ImColor(0, 185, 204, 150));
        bHunting = false;
    }
    
    {
        if (GhostAI::ghost->ghostInfo) {
            const char* strInfo = std::format("Ghost State: {} | Ghost Type: {} | Ghost Distance: {:.2f}" /* | States: 1 : {} 2 : {} 3 : {} 4 : {} 5 : {} 6 : {}"*/,
                !GhostAI::ghost->isHunting ? GhostAI::ghost->ghostState : GhostAI::GhostState::Hunting,
                static_cast<GhostAI::GhostType>(static_cast<int>(GhostAI::ghost->ghostInfo->ghostType) + 1),
                distance/*,
                GhostAI::ghost->canEnterHuntingMode ? "Y" : "N",
                GhostAI::ghost->someBool ? "Y" : "N",
                GhostAI::ghost->isHunting ? "Y" : "N",
                GhostAI::ghost->canAttack ? "Y" : "N",
                GhostAI::ghost->smudgeSticksUsed ? "Y" : "N",
                GhostAI::ghost->canWander ? "Y" : "N"*/
            ).c_str();

            ImVec2 textSize = ImGui::CalcTextSize(strInfo);
            textPos.x -= textSize.x * 0.5f;

            DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
                textPos, strInfo, ImColor(255, 255, 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));

            /*if (LevelController::gLevelController) {
                auto pRoom = GhostAI::ghost->ghostInfo->room;

                if (pRoom && pRoom->name)
                {
                    textPos.y -= 30.0f;
                    std::string formatMessage = std::format("Room: {}", pRoom->name->ToString().c_str());
                    DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
                        textPos, formatMessage.c_str(), ImColor(255, 255, 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));
                }
            }*/
        }

        std::string strCheatsInfo("");
        if (bEsp) {
            strCheatsInfo += "\t[ESP]";
        }

        if (bBoneWallHack) {
            strCheatsInfo += "\t[WallHack]";
        }

        //ImVec2 textSize = ImGui::CalcTextSize(strCheatsInfo.c_str());
        //textPosAtStart.x -= textSize.x * 0.5f;

        DrawTextWithOutline(ImGui::GetBackgroundDrawList(),
            textPosAtStart, strCheatsInfo.c_str(), ImColor(255, 255, 255), 1.0f, ImGuiEx::OutlineSide::All, ImColor(0, 0, 0));
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

auto DrawBoneConnections(II::Animator* pAnimator, float distance) -> void {
    // really ghost bones?
    static float firstDistance = distance;

    for (const auto& [boneStart, boneEnd, label] : bonePairs) 
    {
        auto pStartTransform = pAnimator->GetBoneTransformInternal(boneStart);
        auto pEndTransform = pAnimator->GetBoneTransformInternal(boneEnd);

        if (pStartTransform && pEndTransform) 
        {
            auto pointStart = II::Camera::GetMain()->WorldToScreenPoint(pStartTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
            auto pointEnd = II::Camera::GetMain()->WorldToScreenPoint(pEndTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);

            if (pointStart.z > 0 && pointEnd.z > 0) 
            {
                constexpr float minDist = 1.0f;
                float maxDist = firstDistance;

                float normalizedDistance = std::clamp((distance - minDist) / (maxDist - minDist), 0.0f, 1.0f);

                ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f - normalizedDistance, normalizedDistance, 0.0f, 1.0f));

                pointStart.y = init_space::Info::h - pointStart.y;
                pointEnd.y = init_space::Info::h - pointEnd.y;

                if (pointStart.x >= init_space::Info::w || pointEnd.x >= init_space::Info::w) {
                    continue;
                }

                if (pointStart.y >= init_space::Info::h || pointEnd.y >= init_space::Info::h) {
                    continue;
                }

                if (pointStart.x <= 0.0f|| pointEnd.x <= 0.0f) {
                    continue;
                }

                if (pointStart.y <= 0.0f || pointEnd.y <= 0.0f) {
                    continue;
                }

                ImGui::GetBackgroundDrawList()->AddLine(
                    { pointStart.x, pointStart.y },  
                    { pointEnd.x, pointEnd.y },     
                    color,
                    2.0f                           
                );
            }
        }
    }
}

auto DrawCameraLinesAndCircles(II::Animator* pAnimator, float distance) -> void {
    auto pStartTransform = pAnimator->GetBoneTransformInternal(HumanBodyBones::Spine);

    static float firstDistance = distance;

    if (pStartTransform) 
    {
        auto pointStart = II::Camera::GetMain()->WorldToScreenPoint(pStartTransform->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
        if (pointStart.z > 0) 
        {
            pointStart.y = init_space::Info::h - pointStart.y;

            if (pointStart.x >= init_space::Info::w) {
                return;
            }

            if (pointStart.y >= init_space::Info::h) {
                return;
            }

            if (pointStart.x <= 0.0f) {
                return;
            }

            if (pointStart.y <= 0.0f) {
                return;
            }

            constexpr float minDist = 1.0f; 
            float maxDist = firstDistance;

            float normalizedDistance = std::clamp((distance - minDist) / (maxDist - minDist), 0.0f, 1.0f);

            ImU32 color = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f - normalizedDistance, normalizedDistance, 0.0f, 1.0f));

            ImGui::GetBackgroundDrawList()->AddLine({ init_space::Info::w * 0.5f, init_space::Info::h * 0.5f }, { pointStart.x, pointStart.y }, color, 2.0f);

            ImVec2 textPosition = { init_space::Info::w * 0.5125f, init_space::Info::h * 0.5125f };
            ImGui::GetBackgroundDrawList()->AddText(textPosition, color, std::format("{:.2f}", distance).c_str());
        }
    }

    /*if (auto cameraPoint = II::Camera::GetMain()->WorldToScreenPoint(II::Camera::GetMain()->GetTransform()->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono); point.z > 0) {
        if (distance >= 1.00f && distance <= 2000.0f) {
            ImGui::GetBackgroundDrawList()->AddLine({ init_space::Info::w * 0.5f, init_space::Info::h * 0.5f }, { point.x, point.y - distance }, ImColor(rand() % 255, rand() % 255, rand() % 255), 2.0f);
            ImGui::GetBackgroundDrawList()->AddCircle(
                { point.x, point.y - distance }, 
                5.0f, 
                ImColor(rand() % 255, rand() % 255, rand() % 255),  
                0,  
                2.0f 
            );
        }
    }*/
}

auto GhostEsp::Draw() -> void {
    if (GhostAI::ghost)
    {
        try {
            auto point = II::Camera::GetMain()->WorldToScreenPoint(GhostAI::ghost->GetTransform()->GetPosition(), UnityResolve::UnityType::Camera::Eye::Mono);
            bool isInView = point.z > 0;
            if (isInView) {
                point.y = init_space::Info::h - point.y;

                auto ghostPos = GhostAI::ghost->GetTransform()->GetPosition();
                auto cameraPos = II::Camera::GetMain()->GetTransform()->GetPosition();

                auto ghostScale = GhostAI::ghost->GetTransform()->GetLocalScale();
                auto result = ghostPos - cameraPos;

                float distance = std::sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
                float ghostScaleMagnitude = std::sqrt(ghostScale.x * ghostScale.x + ghostScale.y * ghostScale.y + ghostScale.z * ghostScale.z);
                float realGhostScale = ghostScaleMagnitude * 100.0f;

                distance = std::abs(distance - ghostScaleMagnitude);
                float boxSizeMax = ghostScaleMagnitude * 100.0f;

                // Draw ghost box and info if needed
                // DrawGhostBoxAndInfo({ point.x, point.y }, distance, boxSizeMax, 
                //     std::format("[{}]\n{} | {:.2f}", GhostAI::ghost->ghostState, static_cast<GhostAI::GhostType>(static_cast<int>(GhostAI::ghost->ghostInfo->ghostType) + 1), distance).c_str());

                if (bStateInfo) {
                    DrawGhostStateInfo(distance);
                }

                if (bBoneWallHack || bEsp) {
                    for (const auto pAnimator : GhostAI::ghost->GetGameObject()->GetComponentsInChildren<II::Animator*>(I::Get("UnityEngine.AnimationModule.dll")->Get("Animator"))) {
                        if (bBoneWallHack) {
                            DrawBoneConnections(pAnimator, distance);
                        }

                        if (bEsp) {
                            DrawCameraLinesAndCircles(pAnimator, distance);
                        }
                    }
                }
            }
            else {
                auto ghostPos = GhostAI::ghost->GetTransform()->GetPosition();
                auto cameraPos = II::Camera::GetMain()->GetTransform()->GetPosition();

                auto ghostScale = GhostAI::ghost->GetTransform()->GetLocalScale();
                auto result = ghostPos - cameraPos;

                float distance = std::sqrt(result.x * result.x + result.y * result.y + result.z * result.z);
                float ghostScaleMagnitude = std::sqrt(ghostScale.x * ghostScale.x + ghostScale.y * ghostScale.y + ghostScale.z * ghostScale.z);
                float realGhostScale = ghostScaleMagnitude * 100.0f;

                distance = std::abs(distance - ghostScaleMagnitude);

                if (bStateInfo) {
                    DrawGhostStateInfo(distance);
                }
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
        catch (...) {
            std::cerr << "Unknown exception occurred" << std::endl;
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