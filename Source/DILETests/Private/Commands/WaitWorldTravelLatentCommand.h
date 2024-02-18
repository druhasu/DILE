// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/Engine.h"
#include "Tests/AutomationCommon.h"

/*
 * Waits for completion of WorldTravel of given World
 */
class FWaitWorldTravelCommand : public IAutomationLatentCommand
{
public:
    FWaitWorldTravelCommand(UWorld* SourceWorld)
    {
        // remember WorldContext of given World
        WorldContext = &GEngine->GetWorldContextFromWorldChecked(SourceWorld);

        // listen to PostLoad events
        Handle = FCoreUObjectDelegates::PostLoadMapWithWorld.AddRaw(this, &FWaitWorldTravelCommand::OnPostLoad);
    }

    ~FWaitWorldTravelCommand()
    {
        // unsubscribe from event manually, because we cannot use AddSP in the constructor
        FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(Handle);
    }

    bool Update() override
    {
        // remember current state of GWorld, because TickWorldTravel may change it
        auto CurrentGWorld = GWorld;

        // manually tick world travel because EditorEngine does it only for PIE Worlds
        GEngine->TickWorldTravel(*WorldContext, 1 / 60.f);

        // restore previous value of GWorld
        GWorld = CurrentGWorld;

        return bLoadComplete;
    }

private:
    void OnPostLoad(UWorld* NewWorld)
    {
        // check that loaded world belongs to our WorldContext
        if (WorldContext->World() == NewWorld)
        {
            bLoadComplete = true;
        }
    }

    FWorldContext* WorldContext;
    FDelegateHandle Handle;
    bool bLoadComplete = false;
};
