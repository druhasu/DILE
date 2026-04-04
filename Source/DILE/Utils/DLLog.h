// Copyright

#pragma once

#include "Logging/LogMacros.h"
#include "Engine/World.h"

/* Main log category used across the project */
DECLARE_LOG_CATEGORY_EXTERN(LogDILE, Log, All);

/* Main logging macros used across the project */
#define DL_LOG(Verbosity, Format, ...) \
    { \
        UE_LOG(LogDILE, Verbosity, TEXT("%s%hs: ") Format, DLLog_Private::GetExecutingContextPrefix(), __FUNCTION__, ##__VA_ARGS__); \
    }

namespace DLLog_Private
{
    // helper function to determine whether we are on server or client
    // copied from UKismetSystemLibrary::PrintString
    static const TCHAR* GetExecutingContextPrefix()
    {
#if WITH_EDITOR
        static constexpr const TCHAR* ClientNames[] =
        {
            TEXT("[Client 1]  "),
            TEXT("[Client 2]  "),
            TEXT("[Client 3]  "),
            TEXT("[Client 4]  "),
            TEXT("[Client N]  ")
        };

        if (GWorld != nullptr)
        {
            if (GWorld->WorldType == EWorldType::PIE)
            {
                switch (GWorld->GetNetMode())
                {
                    case NM_Client:
                        return ClientNames[FMath::Min<int32>(UE::GetPlayInEditorID() - 1, UE_ARRAY_COUNT(ClientNames) - 1)];
                    case NM_DedicatedServer:
                    case NM_ListenServer:
                        return TEXT("[Server]    ");
                }
            }
        }

        return TEXT("[Standalone]");
#else
        return TEXT("");
#endif
    }
}
