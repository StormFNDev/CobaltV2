#pragma once
#include <Windows.h>
#include <string>

struct UObject
{
public:
    void** VTable;         // 0x0000
    int         ObjectFlags;    // 64 = 0x0008 | 32 = 0x0004
    int         InternalIndex;  // 64 = 0x000C | 32 = 0x0008
    void* ClassPrivate;   // 64 = 0x0010 | 32 = 0x000C (Missing: UClass)
    char        Unk00[0x8];     // 64 = 0x0018 | 32 = 0x0010
    UObject* OuterPrivate;   // 64 = 0x0020 | 32 = 0x0018
};

typedef UObject* (__fastcall* fStaticConstructObject_Internal)
(
    void* Class   // (Missing: UClass)
    , UObject* InOuter
    , void* Name
    , int             SetFlags
    , unsigned int    InternalSetFlags
    , UObject* Template
    , bool            bCopyTransientsFromClassDefaults
    , void* InstanceGraph
    , bool            bAssumeTemplateIsArchetype
    );

static fStaticConstructObject_Internal StaticConstructObject_Internal;

typedef void(__fastcall* fFree_Internal)
(
    void* Buffer
    );

static fFree_Internal Free_Internal;

typedef FString(__fastcall* fGetObjectName_Internal)
(
    UObject* Object
    );

static fGetObjectName_Internal GetObjectName_Internal;

static std::wstring GetObjectFirstName(UObject* Object)
{
    std::wstring sName(L"");

    FString objName = GetObjectName_Internal(Object);

    if (objName.IsValid())
    {
        sName = objName.c_str();

        Free_Internal(objName.c_str());
    }

    return sName;
}

static std::wstring GetObjectName(UObject* Object)
{
    std::wstring sName(L"");

    for (auto i = 0; Object; Object = Object->OuterPrivate, ++i)
    {
        FString objName = GetObjectName_Internal(Object);

        if (objName.IsValid())
        {
            sName = objName.c_str() + std::wstring(i > 0 ? L"." : L"") + sName;

            Free_Internal(objName.c_str());
        }
    }

    return sName;
}

static PBYTE pProcessEventAddress;

typedef void* (__fastcall* fProcessEvent)
(
    UObject* Object
    , UObject* Function
    , void* Params
    );

static fProcessEvent ProcessEvent;
