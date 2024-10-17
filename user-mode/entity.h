#pragma once

ULONG_PTR get_entity_list()
{
	uintptr_t tf = driver::read_memory<uintptr_t>(driver_handle, client + offsets::dwEntityList);
	return static_cast<ULONG_PTR>(tf);
}

ULONG_PTR get_base_entity_from_index(int index, ULONG_PTR entity_list = 0)
{
	if (entity_list == 0)
		entity_list = get_entity_list();

	ULONG_PTR entity_base = entity_list + 8LL * ((index & 0x7FFF) >> 9) + 16;
	ULONG_PTR entity = driver::read_memory<uint64_t>(driver_handle, entity_base);
	if (!entity)
		return 0;

	entity += 120LL * (index & 0x1FF);
	entity = driver::read_memory<ULONG_PTR>(driver_handle, entity);
	return entity;
}

std::string get_schema_name(const uintptr_t& entity)
{
	const uintptr_t entity_identity = driver::read_memory<uintptr_t>(driver_handle, entity + 0x10); // entity2::CEntityIdentity* m_pEntity; // 0x10   
	if (!entity_identity)
		return "";

	const uintptr_t entity_class_info = driver::read_memory<uintptr_t>(driver_handle, entity_identity + 0x8);
	const uintptr_t schema_class_info_data = driver::read_memory<uintptr_t>(driver_handle, entity_class_info + 0x30);
	const uintptr_t class_name = driver::read_memory<uintptr_t>(driver_handle, schema_class_info_data + 0x8);

	return read_str(class_name);
}

std::string get_designer_name(const uintptr_t& entity)
{
	const uintptr_t entity_identity = driver::read_memory<uintptr_t>(driver_handle, entity + 0x10);
	if (!entity_identity)
		return "";

	const uintptr_t designer_name = driver::read_memory<uintptr_t>(driver_handle, entity_identity + 0x20); //CUtlSymbolLarge m_designerName; // 0x20 
	return read_str(designer_name);
}

uint8_t get_team_num(uintptr_t entity) {
	return driver::read_memory<std::uintptr_t>(driver_handle, entity + offsets::m_iTeamNum);
}

ULONG_PTR get_player_pawn(uintptr_t entity) {
	auto pawn_handle = driver::read_memory<uint32_t>(driver_handle, entity + 0x520);
	return get_base_entity_from_index(pawn_handle & 0x7fff);
}

vector3 get_player_pos(const ULONG_PTR& entity) {
	ULONG_PTR entity_cGameNode = driver::read_memory<ULONG_PTR>(driver_handle, entity + 0x270);
	vector3 absPos = driver::read_memory<vector3>(driver_handle, entity_cGameNode + 0xd0);

	return absPos;
}
