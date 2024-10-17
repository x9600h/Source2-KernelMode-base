#pragma once

std::string get_bone_name(uint64_t cmodel_ptr, const int bone_index)
{
	const int names_count = driver::read_memory<int>(driver_handle, cmodel_ptr + 0x178);
	auto names = driver::read_memory<const char**>(driver_handle, cmodel_ptr + 0x168);

	if (names_count < 0 || bone_index >= names_count)
		return "root";

	const uint64_t bone_name_address = driver::read_memory<uint64_t>(driver_handle, (uint64_t)names + bone_index * 8);
	return read_str(bone_name_address);
}


int get_bone_index(uintptr_t target_entity, const std::string& bone_name)
{
	uint64_t skel_instance = driver::read_memory<uint64_t>(driver_handle, (uint64_t)target_entity + 0x270); // 0x270 - scenenode (credits to source2sdk)
	const auto model_ptr = driver::read_memory<uint64_t>(driver_handle, (uint64_t)skel_instance + 0x170 + 0xd0); // 0x170 - modelstate 0xd0 - hmodel (credits to source2sdk)
	const uint64_t model = driver::read_memory<uint64_t>(driver_handle, model_ptr);

	const __int32 names_count = driver::read_memory<__int32>(driver_handle, (uint64_t)model + 0x178);
	auto names = driver::read_memory<const char**>(driver_handle, (uint64_t)model + 0x168);
	if (names_count <= 0)
		return -1;

	for (int i = 0; i < names_count; ++i)
	{
		const uint64_t bone_name_address = driver::read_memory<uint64_t>(driver_handle, (uint64_t)names + i * sizeof(uint64_t));
		const std::string current_bone_name = read_str(bone_name_address);

		if (current_bone_name == bone_name)
		{
			return i;
		}
	}

	return -1;
}