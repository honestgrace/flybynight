#pragma once

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

#ifndef _SYSINFO_UTIL_H
#define _SYSINFO_UTIL_H

#include <Windows.h>
#include <iostream>
#include <string>

namespace tpf
{
	enum class ProcessorType : DWORD
	{
		Intel_386 = PROCESSOR_INTEL_386,		 // 386
		Intel_486 = PROCESSOR_INTEL_486,		 // 486
		Intel_Pentium = PROCESSOR_INTEL_PENTIUM, // 586
		Intel_IA64 = PROCESSOR_INTEL_IA64,		 // 2200
		Intel_AMD_x86_64 = PROCESSOR_AMD_X8664,	 // 8664
		MIPS_R4000 = PROCESSOR_MIPS_R4000,		 // 4000, incl R4101 & R3910 for Windows CE
		Alpha_21064 = PROCESSOR_ALPHA_21064,	 // 21064
		PPC_601 = PROCESSOR_PPC_601,			 // 601
		PPC_603 = PROCESSOR_PPC_603,			 // 603
		PPC_604 = PROCESSOR_PPC_604,			 // 604
		PPC_620 = PROCESSOR_PPC_620,			 // 620
		Hitachi_SH3 = PROCESSOR_HITACHI_SH3,	 // 10003, Windows CE
		Hitachi_SH3E = PROCESSOR_HITACHI_SH3E,	 // 10004, Windows CE
		Hitachi_SH4 = PROCESSOR_HITACHI_SH4,	 // 10005, Windows CE
		Motorola_821 = PROCESSOR_MOTOROLA_821,	 // 821, Windows CE
		SHx_SH3 = PROCESSOR_SHx_SH3,			 // 103, Windows CE
		SHx_SH4 = PROCESSOR_SHx_SH4,			 // 104, Windows CE
		StrongArm = PROCESSOR_STRONGARM,		 // 2577, Windows CE - 0xA11
		ARM720 = PROCESSOR_ARM720,				 // 1824, Windows CE - 0x720
		ARM820 = PROCESSOR_ARM820,				 // 2080, Windows CE - 0x820
		ARM920 = PROCESSOR_ARM920,				 // 2336, Windows CE - 0x920
		ARM_7TDMI = PROCESSOR_ARM_7TDMI,		 // 70001, Windows CE
		OPTIL = PROCESSOR_OPTIL					 // 0x494f, MSIL
	};
	
	enum class ProcessorArchitecture : WORD
	{
		Intel = PROCESSOR_ARCHITECTURE_INTEL,					// 0
		Mips = PROCESSOR_ARCHITECTURE_MIPS,						// 1
		Alpha = PROCESSOR_ARCHITECTURE_ALPHA,					// 2
		Ppc = PROCESSOR_ARCHITECTURE_PPC,						// 3
		Shx = PROCESSOR_ARCHITECTURE_SHX,						// 4
		Arm = PROCESSOR_ARCHITECTURE_ARM,						// 5
		IA64 = PROCESSOR_ARCHITECTURE_IA64,						// 6
		Alpha64 = PROCESSOR_ARCHITECTURE_ALPHA64,				// 7
		Msil = PROCESSOR_ARCHITECTURE_MSIL,						// 8
		Amd64 = PROCESSOR_ARCHITECTURE_AMD64,					// 9
		IA32_On_Win64 = PROCESSOR_ARCHITECTURE_IA32_ON_WIN64,	// 10
		Neutral = PROCESSOR_ARCHITECTURE_NEUTRAL,				// 11
		Unknown = PROCESSOR_ARCHITECTURE_UNKNOWN				// 0xFFFF 
	};
	
	enum class ProcessorFeature : DWORD
	{
		// IsProcessorFeaturePresent function
		// https://goo.gl/2sF9Ls

		Floating_Point_Precision_Errata = PF_FLOATING_POINT_PRECISION_ERRATA,	// 0
		Floating_Point_Emulated = PF_FLOATING_POINT_EMULATED,					// 1
		Compare_Exchange_Double = PF_COMPARE_EXCHANGE_DOUBLE,					// 2   
		MMX_Instructions_Available = PF_MMX_INSTRUCTIONS_AVAILABLE,				// 3   
		PPC_MoveMem_64Bit_OK = PF_PPC_MOVEMEM_64BIT_OK,							// 4   
		Alpha_Byte_Instructions = PF_ALPHA_BYTE_INSTRUCTIONS,					// 5   
		XMMI_Instructions_Available = PF_XMMI_INSTRUCTIONS_AVAILABLE,			// 6   
		D3Now_Instructions_Available = PF_3DNOW_INSTRUCTIONS_AVAILABLE,			// 7   
		RDTSC_Instruction_Available = PF_RDTSC_INSTRUCTION_AVAILABLE,			// 8   
		PAE_Enabled = PF_PAE_ENABLED,											// 9   
		XMMI64_Instructions_Available = PF_XMMI64_INSTRUCTIONS_AVAILABLE,		// 10   
		SSE_DAZ_Mode_Available = PF_SSE_DAZ_MODE_AVAILABLE,						// 11   
		NX_Enabled = PF_NX_ENABLED,												// 12   
		SSE3_Instructions_Available = PF_SSE3_INSTRUCTIONS_AVAILABLE,			// 13   
		Compare_Exchange128 = PF_COMPARE_EXCHANGE128,							// 14   
		Compare64_Exchange128 = PF_COMPARE64_EXCHANGE128,						// 15   
		Channels_Enabled = PF_CHANNELS_ENABLED,									// 16   
		XSAVE_Enabled = PF_XSAVE_ENABLED,										// 17   
		ARM_VFP_32_Registers_Available = PF_ARM_VFP_32_REGISTERS_AVAILABLE,		// 18   
		ARM_Neon_Instructions_Available = PF_ARM_NEON_INSTRUCTIONS_AVAILABLE,	// 19   
		Second_Level_Address_Translation = PF_SECOND_LEVEL_ADDRESS_TRANSLATION, // 20   
		VIRT_Firmware_Enabled = PF_VIRT_FIRMWARE_ENABLED,						// 21   
		RDWR_FSGS_BASE_Available = PF_RDWRFSGSBASE_AVAILABLE,					// 22   
		Fast_Fail_Available = PF_FASTFAIL_AVAILABLE,							// 23   
		ARM_Divide_Instruction_Available = PF_ARM_DIVIDE_INSTRUCTION_AVAILABLE, // 24   
		ARM_64bit_Load_Store_Atomic = PF_ARM_64BIT_LOADSTORE_ATOMIC,			// 25   
		ARM_External_Cache_Available = PF_ARM_EXTERNAL_CACHE_AVAILABLE,			// 26   
		ARM_FMAC_Instructions_Available = PF_ARM_FMAC_INSTRUCTIONS_AVAILABLE,	// 27   
		RdRand_Instruction_Available = PF_RDRAND_INSTRUCTION_AVAILABLE			// 28   
	};

	extern std::ostream& operator<<(std::ostream& os, tpf::ProcessorType pt);
	extern std::ostream& operator<<(std::ostream& os, tpf::ProcessorArchitecture pro_arch);

	extern std::string ProcessorTypeToString(tpf::ProcessorType type);
	extern std::string ProcessorArchitectureToString(tpf::ProcessorArchitecture proarch);
	
	extern BOOL IsProcessorFeaturePresent(ProcessorFeature pf);
	extern ProcessorArchitecture GetProcessorArchitecture();
	extern ProcessorType GetProcessorType();
	extern DWORD GetNumberOfProcessors();
	extern DWORD GetPageSize();
	extern DWORD GetAllocationGranularity();
	extern LPVOID GetMinimumApplicationAddress();
	extern LPVOID GetMaximumApplicationAddress();
	extern WORD GetProcessorLevel();
	extern WORD GetProcessorRevision();

	extern void PrintProcessorType();
	extern void PrintProcessorFeatures();
	extern void PrintProcessorArchitecture();
	extern void PrintSystemInformation();
}

#endif // end of file
