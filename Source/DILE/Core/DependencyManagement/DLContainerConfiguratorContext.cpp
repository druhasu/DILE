// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfiguratorContext.h"

#include "GameFramework/Actor.h"
#include "NativeGameplayTags.h"

static UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tag_HasServerLogic, "ContainerContext.HasServerLogic", "Game has server logic, i.e is a host or standalone");
static UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tag_HasClientLogic, "ContainerContext.HasClientLogic", "Game has client logic, i.e is connected to a server or standalone");
static UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tag_IsLocal, "ContainerContext.IsLocallyControlled", "Current Actor is controlled locally");

FDLWorldContainerConfiguratorContext::FDLWorldContainerConfiguratorContext(UWorld* World)
    : World(World)
{
    const ENetMode NetMode = World->GetNetMode();

    if (NetMode != NM_Client)
        ContextTags.AddTagFast(Tag_HasServerLogic);

    if (NetMode != NM_DedicatedServer)
        ContextTags.AddTagFast(Tag_HasClientLogic);
}

bool FDLWorldContainerConfiguratorContext::HasServerLogic() const
{
    return ContextTags.HasTag(Tag_HasServerLogic);
}

bool FDLWorldContainerConfiguratorContext::HasClientLogic() const
{
    return ContextTags.HasTag(Tag_HasClientLogic);
}

FDLActorContainerConfiguratorContext::FDLActorContainerConfiguratorContext(AActor* Actor)
    : FDLWorldContainerConfiguratorContext(Actor->GetWorld())
    , Actor(Actor)
{
    if (Actor->HasLocalNetOwner())
        ContextTags.AddTagFast(Tag_IsLocal);
}

bool FDLActorContainerConfiguratorContext::IsLocal() const
{
    return ContextTags.HasTag(Tag_IsLocal);
}
