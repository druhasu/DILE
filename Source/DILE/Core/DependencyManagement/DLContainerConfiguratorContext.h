// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "GameplayTagContainer.h"

class UWorld;

class FDLWorldContainerConfiguratorContext
{
public:
    FDLWorldContainerConfiguratorContext(UWorld* World);

    UWorld* GetWorld() const;

    /* Whether game has Server logic */
    bool HasServerLogic() const;

    /* Whether game has Client logic */
    bool HasClientLogic() const;

    const FGameplayTagContainer& GetTags() const { return ContextTags; }

protected:
    UWorld* World;
    FGameplayTagContainer ContextTags;
};

class FDLActorContainerConfiguratorContext : public FDLWorldContainerConfiguratorContext
{
public:
    FDLActorContainerConfiguratorContext(AActor* Actor);

    AActor* GetActor() const { return Actor; }

    /* Whether Actor is local and controlled from current game instance */
    bool IsLocal() const;

private:
    AActor* Actor;
};
