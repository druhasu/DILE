// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLContainerConfigurationAsset.h"

#include "DI/ObjectContainerBuilder.h"

void UDLFeatureConfigurationAsset::RegisterClassesForActor(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context) const
{
    UClass* ActorClass = Context.GetActor()->GetClass();

    for (auto& [Class, List] : Classes)
    {
        if (ActorClass->IsChildOf(Class))
        {
            for (const FDLContainerConfigurationClassEntry& Entry : List.Entries)
            {
                RegisterEntry(Builder, Context, Entry);
            }

            return;
        }
    }
}

void UDLFeatureConfigurationAsset::RegisterEntry(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context, const FDLContainerConfigurationClassEntry& Entry) const
{
    if (!MatchesCondition(Context, Entry))
        return;

    auto& Registration = Builder.RegisterType(Entry.ServiceClass);

    // register all implemented interfaces
    Registration.ByInterfaces(EInterfaceSearchOptions::CurrentAndSuper);

    // find native class for Self registration
    UClass* EntryNativeClass = Entry.ServiceClass;
    while (EntryNativeClass != nullptr && !EntryNativeClass->HasAnyClassFlags(EClassFlags::CLASS_Native))
    {
        EntryNativeClass = EntryNativeClass->GetSuperClass();
    }

    // register as Native class if possible and if it makes sense
    static TSet<UClass*> ProhibitedClasses = { nullptr, UObject::StaticClass(), UActorComponent::StaticClass(), AActor::StaticClass() };
    if (!ProhibitedClasses.Contains(EntryNativeClass))
    {
        Registration.As(EntryNativeClass);
    }

    // register as all additional classes
    for (TSubclassOf<UObject> AdditionalClass : Entry.RegisterAs)
    {
        // TODO: add compatibility check here
        if(AdditionalClass != nullptr)
            Registration.As(AdditionalClass);
    }

    // apply desired lifetime
    switch (Entry.Lifetime)
    {
        case EDLContainerConfigurationClassLifetime::Initial:
            Registration.SingleInstance(true);
            break;

        case EDLContainerConfigurationClassLifetime::OnDemandWeak:
            Registration.WeakSingleInstance();
            break;

        default:
            Registration.SingleInstance();
            break;
    }
}

bool UDLFeatureConfigurationAsset::MatchesCondition(const FDLActorContainerConfiguratorContext& Context, const FDLContainerConfigurationClassEntry& Entry) const
{
    // if condition is not set, assume it matches any context
    if (Entry.Condition.IsEmpty())
        return true;

    return Entry.Condition.Matches(Context.GetTags());
}

void UDLGameModeConfigurationAsset::RegisterClassesForActor(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context) const
{
    // TODO: flatten list of all features
    for (const UDLFeatureConfigurationAsset* FeatureAsset : Features)
    {
        FeatureAsset->RegisterClassesForActor(Builder, Context);
    }
}
