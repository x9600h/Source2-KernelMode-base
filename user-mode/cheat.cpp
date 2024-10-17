#include "cheat.h"
#include "driver.h"
#include "offsets.h"
#include "math.h"
#include "entity.h"
#include "settings.h"
#include "bone_manager.h"
#include <thread>
#include <unordered_set>
#include <unordered_map>
#include <array>
#include <imgui.h>

view_matrix_t get_view_matrix() {
	view_matrix_t view_matrix;
	driver::read_virtual_memory(driver_handle, (client + offsets::dwViewMatrix), &view_matrix, sizeof(view_matrix));
	return view_matrix;
}

void cheat::draw_penis() {
	if (!settings::draw_penis_f)
		return;
	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2 - 20, 
		GetSystemMetrics(SM_CYSCREEN) / 2 - 10), 10.0f, ImColor(255, 255, 255, 255));

	ImGui::GetForegroundDrawList()->AddCircle(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2, 
		GetSystemMetrics(SM_CYSCREEN) / 2 - 10), 10.0f, ImColor(255, 255, 255, 255));

	ImGui::GetForegroundDrawList()->AddEllipse(ImVec2(GetSystemMetrics(SM_CXSCREEN) / 2 - 10, 
		GetSystemMetrics(SM_CYSCREEN) / 2 - 45), ImVec2(10, 30), ImColor(255, 255, 255, 255));
}

void cheat::enable() {

	cheat::draw_penis();

	std::unordered_set<ULONG_PTR> processed_ents;
	const int max_ents = 5000;
	auto view_matrix = get_view_matrix();
	for (size_t i = 1; i <= static_cast<size_t>(max_ents); ++i)
	{
		auto entity = get_base_entity_from_index(i);

		if (!entity || !processed_ents.insert(entity).second)
			continue;

		if (get_designer_name(entity) != "player")
			continue;

		if (!settings::write_entities_info)
			return;

		int health = driver::read_memory<uintptr_t>(driver_handle, entity + offsets::m_iHealth);

		printf("[>] %s | %s -> %p | health -> %d\n", get_schema_name(entity).c_str(), 
													 get_designer_name(entity).c_str(), 
													 reinterpret_cast<void*>(entity), health);
	}
	
}