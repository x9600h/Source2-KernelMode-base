#include "overlay.h"
#include "../settings.h"
#include <imgui.h>

void overlay::draw_gui()
{
	ImGui::SetNextWindowSize(ImVec2(600, 400));

	ImGui::Begin("gui", nullptr, ImGuiWindowFlags_NoDecoration);
	{
		ImGui::Text("monochrome | Source2 kernel base");

		ImGui::GetForegroundDrawList()->AddLine(ImVec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y + 27), 
											    ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + 27),
												ImColor(255, 255, 255, 255), 3.0f);

		ImGui::Spacing(); // }
		ImGui::Spacing(); // } --- quite dirty shit
		ImGui::Spacing(); // }

		ImGui::Checkbox("Write entity info to console", &settings::write_entities_info);
		ImGui::Checkbox("Draw penis", &settings::draw_penis_f);
	}
	ImGui::End();
}