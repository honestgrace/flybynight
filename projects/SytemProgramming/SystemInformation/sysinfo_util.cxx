/*
	Author: Thomas Kim
	First Edit: Dec. 24, 2018

	Include this file in the pre-compiled header either stdafx.h or pch.h
	and include sysinfo_util.cxx in stdafx.cpp or pch.cpp

	in the stdafx.h or pch.h
	#include "sysinfo_util.h"

	in the stdafx.cpp or pch.cpp
	#include "sysinfo_util.cxx"

	System Information Functions - https://goo.gl/jqQuGJ
	GetSystemInfo function - https://goo.gl/zwDdLK
	IsProcessorFeaturePresent function - https://goo.gl/M6dHRz
*/

// #include "sysinfo_util.h"
namespace tpf
{
	ProcessorType GetProcessorType()
	{
		// GetSystemInfo function
		// https://goo.gl/7Rnjy9
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return static_cast<ProcessorType>(si.dwProcessorType);
	}

	DWORD GetNumberOfProcessors()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.dwNumberOfProcessors;
	}

	ProcessorArchitecture GetProcessorArchitecture()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return static_cast<ProcessorArchitecture>(si.wProcessorArchitecture);
	}

	BOOL IsProcessorFeaturePresent(ProcessorFeature pf)
	{
		return ::IsProcessorFeaturePresent((DWORD)pf);
	}

	void PrintFeaturePresent(tpf::ProcessorFeature pf, std::string msg)
	{
		BOOL bSupported = tpf::IsProcessorFeaturePresent(pf);

		std::cout << (bSupported ? "Yes - " : "No  - ")
			<< msg << std::endl;
	}

	void PrintProcessorFeatures()
	{
		using pf = tpf::ProcessorFeature;
		std::cout << "--- Is Processor Feature Present ---" << std::endl;
		PrintFeaturePresent(pf::Alpha_Byte_Instructions, "Alpha Byte Instructions");
		PrintFeaturePresent(pf::ARM_64bit_Load_Store_Atomic, "ARM 64bit Load Store Atomic");
		PrintFeaturePresent(pf::ARM_Divide_Instruction_Available, "ARM Divide Instruction Available");
		PrintFeaturePresent(pf::ARM_External_Cache_Available, "ARM External Cache Available");
		PrintFeaturePresent(pf::ARM_FMAC_Instructions_Available, "ARM FMAC Instruction Available");
		PrintFeaturePresent(pf::ARM_Neon_Instructions_Available, "ARM Neon Instructions Available");
		PrintFeaturePresent(pf::ARM_VFP_32_Registers_Available, "ARM VFP 32 Registers Available");
		PrintFeaturePresent(pf::Channels_Enabled, "Channels Enabled");
		PrintFeaturePresent(pf::Compare64_Exchange128, "Compare64 Exchange128");
		PrintFeaturePresent(pf::Compare_Exchange128, "Compare Exchange128");
		PrintFeaturePresent(pf::Compare_Exchange_Double, "Compare Exchange Double");
		PrintFeaturePresent(pf::D3Now_Instructions_Available, "3DNow Instructions Available");
		PrintFeaturePresent(pf::Fast_Fail_Available, "Fast Fail Available");
		PrintFeaturePresent(pf::Floating_Point_Emulated, "Floating Point Emulated");
		PrintFeaturePresent(pf::Floating_Point_Precision_Errata, "Floating Point Precision Errata");
		PrintFeaturePresent(pf::MMX_Instructions_Available, "MMX Instructions Available");
		PrintFeaturePresent(pf::NX_Enabled, "NX Enabled");
		PrintFeaturePresent(pf::PAE_Enabled, "PAE Enabled");
		PrintFeaturePresent(pf::PPC_MoveMem_64Bit_OK, "PPC MoveMem 64bit OK");
		PrintFeaturePresent(pf::RdRand_Instruction_Available, "RdRand Instruction Available");
		PrintFeaturePresent(pf::RDTSC_Instruction_Available, "RDTSC Instruction Available");
		PrintFeaturePresent(pf::RDWR_FSGS_BASE_Available, "RDWR FSGS Base Available");
		PrintFeaturePresent(pf::Second_Level_Address_Translation, "Second Level Address Translation");
		PrintFeaturePresent(pf::SSE3_Instructions_Available, "SSE3 Instructions Available");
		PrintFeaturePresent(pf::SSE_DAZ_Mode_Available, "SSE DAZ Mode Available");
		PrintFeaturePresent(pf::VIRT_Firmware_Enabled, "VIRT Firmware Enabled");
		PrintFeaturePresent(pf::XMMI64_Instructions_Available, "XMMI64 Instructions Available");
		PrintFeaturePresent(pf::XMMI_Instructions_Available, "XMMI Instructions Available");
		PrintFeaturePresent(pf::XSAVE_Enabled, "XSAVE Enabled");
		std::cout << std::endl;
	}
		
	std::string ProcessorArchitectureToString(tpf::ProcessorArchitecture proarch)
	{
		using pa = tpf::ProcessorArchitecture;

		std::string proc_arch("Unknown");

		switch (proarch)
		{
		case pa::Alpha:
			proc_arch = "Alpha"; break;

		case pa::Alpha64:
			proc_arch = "Alpha64"; break;

		case pa::Amd64:
			proc_arch = "Amd64"; break;

		case pa::Arm:
			proc_arch = "Arm"; break;

		case pa::IA32_On_Win64:
			proc_arch = "IA32_On_Win64"; break;

		case pa::IA64:
			proc_arch = "IA64"; break;

		case pa::Intel:
			proc_arch = "Intel"; break;

		case pa::Mips:
			proc_arch = "Mips"; break;

		case pa::Msil:
			proc_arch = "Msil"; break;

		case pa::Neutral:
			proc_arch = "Neutral"; break;

		case pa::Ppc:
			proc_arch = "Ppc"; break;

		case pa::Shx:
			proc_arch = "SHx"; break;

		case pa::Unknown:
		default:
			proc_arch = "Unknown"; break;
		}

		return proc_arch;
	}
	
	std::ostream& operator<<(std::ostream& os, tpf::ProcessorArchitecture pro_arch)
	{
		os << ProcessorArchitectureToString(pro_arch);
		return os;
	}

	void PrintProcessorArchitecture()
	{
		std::cout << "--- Your processor architecture: "
			<< ProcessorArchitectureToString(GetProcessorArchitecture()) << std::endl;
	}

	std::string ProcessorTypeToString(tpf::ProcessorType type)
	{
		std::string proc_type("Unknown");
		using pt = tpf::ProcessorType;
		switch (type)
		{
		case pt::Alpha_21064:
			proc_type = "Alpha_21064"; break;

		case pt::ARM720:
			proc_type = "ARM720"; break;

		case pt::ARM820:
			proc_type = "ARM820"; break;

		case pt::ARM920:
			proc_type = "ARM920"; break;

		case pt::ARM_7TDMI:
			proc_type = "ARM_7TDMI"; break;

		case pt::Hitachi_SH3:
			proc_type = "Hitachi_SH3"; break;

		case pt::Hitachi_SH3E:
			proc_type = "Hitachi_SH3E"; break;

		case pt::Hitachi_SH4:
			proc_type = "Hitachi_SH4"; break;

		case pt::Intel_386:
			proc_type = "Intel_386"; break;

		case pt::Intel_486:
			proc_type = "Intel_486"; break;

		case pt::Intel_AMD_x86_64:
			proc_type = "Intel_AMD_x86_64"; break;

		case pt::Intel_IA64:
			proc_type = "Intel_IA64"; break;

		case pt::Intel_Pentium:
			proc_type = "Intel_Pentium"; break;

		case pt::MIPS_R4000:
			proc_type = "MIPS_R4000"; break;

		case pt::Motorola_821:
			proc_type = "Motorola_821"; break;

		case pt::OPTIL:
			proc_type = "OPTIL"; break;

		case pt::PPC_601:
			proc_type = "PPC_601"; break;

		case pt::PPC_603:
			proc_type = "PPC_603"; break;

		case pt::PPC_604:
			proc_type = "PPC_604"; break;

		case pt::PPC_620:
			proc_type = "PPC_620"; break;

		case pt::SHx_SH3:
			proc_type = "SHx_SH3"; break;

		case pt::SHx_SH4:
			proc_type = "SHX_SH4"; break;

		case pt::StrongArm:
			proc_type = "StrongArm"; break;

		default:
			;
		}

		return proc_type;
	}

	void PrintProcessorType()
	{
		std::cout << "--- Your processor type: "
			<< ProcessorTypeToString(GetProcessorType()) << std::endl;
	}

	std::ostream& operator<<(std::ostream& os, tpf::ProcessorType pt)
	{
		os << ProcessorTypeToString(pt);
		return os;
	}

	DWORD GetPageSize()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.dwPageSize;
	}

	DWORD GetAllocationGranularity()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.dwAllocationGranularity;
	}

	LPVOID GetMinimumApplicationAddress()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.lpMinimumApplicationAddress;
	}

	LPVOID GetMaximumApplicationAddress()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.lpMaximumApplicationAddress;
	}

	WORD GetProcessorLevel()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.wProcessorLevel;
	}

	WORD GetProcessorRevision()
	{
		SYSTEM_INFO si{}; GetSystemInfo(&si);
		return si.wProcessorRevision;
	}

	void PrintSystemInformation()
	{
		std::cout << "--- System Information ---" << std::endl;

		std::cout << "Processor Architecture: " << GetProcessorArchitecture() << std::endl;
		std::cout << "Processor Type: " << GetProcessorType() << std::endl;
		std::cout << "Number of Processors: " << GetNumberOfProcessors() << std::endl;
		std::cout << "Page Size: " << GetPageSize() << std::endl;
		std::cout << "Allocation Granularity: " << GetAllocationGranularity() << std::endl;
		std::cout << "Minimum Application Address: " << GetMinimumApplicationAddress() << std::endl;
		std::cout << "Maximum Application Address: " << GetMaximumApplicationAddress() << std::endl;
		std::cout << "Processor Level: " << GetProcessorLevel() << std::endl;
		std::cout << "Processor Revision: " << GetProcessorRevision() << std::endl;
		std::cout << std::endl;
	}
}