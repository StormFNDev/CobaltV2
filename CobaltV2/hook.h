#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include "MinHook.h"
#include "structs.h"
#include "enums.h"
#include "kiero.h"

namespace InitTypes {
	bool InitVEH = false;
	bool InitMH = false;
}

struct HookInfo
{
	void* Dest;
	void* detour;

	HookInfo(void* Dest, void* destour) : Dest(Dest), detour(detour) {}
};

static std::vector<HookInfo> Hooks;

bool AreSamePage(void* Addr1, void* Addr2)
{
	MEMORY_BASIC_INFORMATION memInfo1;
	MEMORY_BASIC_INFORMATION memInfo2;
	if (!VirtualQuery(Addr1, &memInfo1, sizeof(memInfo1))) return true;
	if (!VirtualQuery(Addr2, &memInfo2, sizeof(memInfo2))) return true;

	return memInfo1.BaseAddress == memInfo2.BaseAddress;
}

LONG WINAPI VectoredExceptionHandler(EXCEPTION_POINTERS* Exception)
{

	if (Exception->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
	{
		for (HookInfo& Hook : Hooks)if (Hook.Target == (void*)Exception->ContextRecord->Rip) Exception->ContextRecord->Rip = (uintptr_t)Hook.Detour;
		Exception->ContextRecord->EFlags |= 0x100;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (Exception->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
	{
		for (HookInfo& Hook : Hooks)
		{
			DWORD dwOldProtect;
			VirtualProtect(Hook.Target, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

class Hooking
{
	bool Init(HookingMethod type) // Sets up veh and mh
	{
		switch (type)
		{
		case HookingMethod::VEH:
			AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)VectoredExceptionHandler);
			InitTypes::InitVEH = true;
			return InitTypes::InitVEH;
		case HookingMethod::MH:
			if (InitTypes::InitMH) return true;
			MH_Initialize();
			InitTypes::InitMH = true;
			return InitTypes::InitMH;
		case HookingMethod::KIERO:
				kiero::init(kiero::RenderType::Auto);
		default:
			return false;
		}
	}
	bool Hook(void* Dest, void* detour, HookingMethod type, void* og)
	{
		if (!Init(type)) return false;

		switch (type) {
		case HookingMethod::VEH:
			if (AreSamePage(Dest, detour)) return false;
			DWORD dwOldProtect;
			VirtualProtect(Dest, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);
			Hooks.emplace_back(Dest, detour);
			return true;
		case HookingMethod::MH:
			MH_CreateHook(Dest, detour, (void**)og);
			if (!MH_EnableHook(Dest) != MH_OK) return false;
			return true;
		default:
			return false;
		}
	}
	bool Unhook(void* Dest) // TODO
	{
		return NULL;
	}
	
	static void* ProcessEventHook(UObject* Object, UObject* Function, void* Params)
	{
		if (Object && Function)
		{
			std::wstring sObjName = GetObjectFirstName(Object);
			std::wstring sFuncName = GetObjectFirstName(Function);

			if (sFuncName.find(L"SendClientHello") != std::string::npos ||
				sFuncName.find(L"SendPacketToServer") != std::string::npos ||
				sFuncName.find(L"SendPacketToClient") != std::string::npos)
			{
				return NULL;
			}

			// TODO(Cyuubi): Re-add ProcessEvent logging, after it's been cleaned up and more refined.
		}

		return ProcessEvent(Object, Function, Params);
	}
}; 