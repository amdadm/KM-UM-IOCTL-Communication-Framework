#pragma once

/*===============================================================================================
  Project         : KM-UM-IOCTL-Communication-Framework

  Original Author : amdadm
  GitHub          : https://github.com/amdadm
  Created         : 2025-04-28

  Description     : Framework for establishing communication between Kernel Mode (KM) and User Mode (UM) components through IOCTL.

  License         : MIT License
===============================================================================================*/

#include "Communication/Ioctl.h"

int main()
{
    bool DriverInits = InitializeCommunication();
    if (!DriverInits) {
        std::cerr << "\n [-] Failed initializing communication, driver not loaded." << std::endl;
        std::cin.get();
        return -1;
    }

    std::cerr << "\n [+] Connection initialized succesfully!" << std::endl;

    TargetPid = GetProcessID("Notepad.exe");

    std::cout << "\n [+] Process Id: " << TargetPid << "\n";

    BaseAddress = GetBaseAddress();

    std::cout << "\n [+] Base Address: " << BaseAddress << "\n";

    std::cin.get();
    return 0;
}