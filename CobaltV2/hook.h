#pragma once

#include <Windows.h>
#include <vector>
#include "MinHook.h"
#include "enums.h"
#include "kiero.h"
#include "codes.h"
#include "util.h"

struct HookInfo
{
	void* Dest;
	void* detour;

	HookInfo(void* Dest, void* detour) : Dest(Dest), detour(detour) {}
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
		for (HookInfo& Hook : Hooks)if (Hook.Dest == (void*)Exception->ContextRecord->Rip) Exception->ContextRecord->Rip = (uintptr_t)Hook.detour;
		Exception->ContextRecord->EFlags |= 0x100;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	else if (Exception->ExceptionRecord->ExceptionCode == STATUS_SINGLE_STEP)
	{
		for (HookInfo& Hook : Hooks)
		{
			DWORD dwOldProtect;
			VirtualProtect(Hook.Dest, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);
		}

		return EXCEPTION_CONTINUE_EXECUTION;
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

class Hooking
{
private:
	static bool Init(int type) // Sets up veh and mh
	{
		switch (type)
		{
		case 0:
			AddVectoredExceptionHandler(true, (PVECTORED_EXCEPTION_HANDLER)VectoredExceptionHandler);
			InitVEH = true;
			return InitVEH;
			break;
		case 1:
			if (InitMH) return true;
			MH_Initialize();
			InitMH = true;
			return InitMH;
			break;
		case 2:
			bool init_hook = false;
			do
			{
				if (kiero::init(kiero::RenderType::Auto) == kiero::Status::Success)
				{
					init_hook = true;
				}
			} while (!init_hook);
			InitKiero = true;
			return InitKiero;
			break;
		}
		return false;
	}
public:
	static bool Hook(void* Dest, void* detour, void* og, int type)
	{
		if (!Init(type)) Init(type);

		switch (type) {
		case 0:
			if (AreSamePage(Dest, detour)) return false;
			DWORD dwOldProtect;
			VirtualProtect(Dest, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &dwOldProtect);
			Hooks.emplace_back(Dest, detour);
			return true;
		case 1:
			MH_CreateHook(Dest, detour, (void**)og);
			if (!MH_EnableHook(Dest) != MH_OK) return false;
			return true;
		case 2:
			kiero::Status::Enum a = kiero::bind(Dest, (void**)og, detour);
			if (a != kiero::Status::Success)
			{
				if (a == kiero::Status::NotInitializedError)
				{
					Log(KIERO_NOT_INITIALIZED, Colors::defaultGray, true, true);
				}
				return false;
			}
		}
		return false;
	}
	bool Unhook(void* Dest) // TODO
	{
		return NULL;
	}
	
	/* static void* ProcessEventHook(UObject* Object, UObject* Function, void* Params)
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
	} */
}; 