#include <thread>
#include "driver.h"
#include "offsets.h"
#include "overlay/overlay.h"
#include "memory.h"

int main() {

	pid = get_process_id(L"project8.exe");

	if (pid == 0) {
		std::cout << "failed to find process \n";
		return 1;
	}

	driver_handle = CreateFileW(L"\\\\.\\adrenochrome", GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, nullptr);

	if (driver_handle == INVALID_HANDLE_VALUE) {
		std::cout << "failed to create our driver handle \n";	
		return 1;
	}

	if (driver::attach_to_process(driver_handle, pid) == true)
	{
		std::cout << "[+] attachment successful \n";

		client = get_module_address(pid, L"client.dll");

		if (!client) {
			std::cout << "can't find client.dll module" << std::endl;
		}

		overlay::render(); //Render overlay
	}
		
	CloseHandle(driver_handle);
	return 0;
}