// SystemInformation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "sysinfo_util.h"
#include "sysinfo_util.cxx"

void PrintMySystemInformation()
{
    tpf::PrintProcessorType();
    tpf::PrintProcessorArchitecture();
    std::cout << std::endl;

    tpf::PrintSystemInformation();
    tpf::PrintProcessorFeatures();
}

void HowToUseGetFunctions()
{
    std::cout << "Allocation Granualarity: " << tpf::GetAllocationGranularity() << std::endl;
    std::cout << "Maximum Application Address: " << tpf::GetMaximumApplicationAddress() << std::endl;
    std::cout << "Minimum Application Address: " << tpf::GetMinimumApplicationAddress() << std::endl;
    std::cout << "Number of Processors: " << tpf::GetNumberOfProcessors() << std::endl;
    std::cout << "Page Size: " << tpf::GetPageSize() << std::endl;
    std::cout << "Processor Architecture: " << tpf::GetProcessorArchitecture() << std::endl;

    using pf=tpf::ProcessorFeature;

    std::cout << "3DNow Instructions Available: " <<
        tpf::IsProcessorFeaturePresent(pf::D3Now_Instructions_Available) << std::endl;

    std::cout << "SSE3 Instructions Available: " <<
        tpf::IsProcessorFeaturePresent(pf::SSE3_Instructions_Available) << std::endl;

    std::cout << "XMMI64 Instructions Available: " <<
        tpf::IsProcessorFeaturePresent(pf::XMMI64_Instructions_Available) << std::endl;
}

int main()
{
    //PrintMySystemInformation();
    HowToUseGetFunctions();
    return 0;
}

