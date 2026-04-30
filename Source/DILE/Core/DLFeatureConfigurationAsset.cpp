// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLFeatureConfigurationAsset.h"
#include "Utils/DLEnsure.h"
#include "Utils/DLLog.h"
#include "Utils/DLValidationUtils.h"

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

#if WITH_EDITOR
EDataValidationResult UDLFeatureConfigurationAsset::IsDataValid(FDataValidationContext& Context) const
{
    for (int32 Index = 0; Index < RequiredFeatures.Num(); ++Index)
    {
        DL_VALIDATE_DATA(RequiredFeatures[Index] != nullptr, "Invalid Required Feature reference at index {0}", Index);
    }

    int32 ClassIndex = 0;
    for(const auto& Pair : Classes)
    {
        DL_VALIDATE_DATA(Pair.Key != nullptr, "Invalid Key in Classes at index {0}", ClassIndex);

        for (int32 Index = 0; Index < Pair.Value.Entries.Num(); ++Index)
        {
            const FDLContainerConfigurationClassEntry& Entry = Pair.Value.Entries[Index];

            DL_VALIDATE_DATA(Entry.ServiceClass != nullptr, "Invalid ServiceClass for actor '{0}' at index {1}", Pair.Key->GetName(), Index);
        }

        ++ClassIndex;
    }

    return DL_VALIDATION_RESULT();
}
#endif

void UDLFeatureConfigurationAsset::RegisterEntry(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context, const FDLContainerConfigurationClassEntry& Entry) const
{
    DL_ENSURE_RETURN(Entry.ServiceClass != nullptr);

    if (!MatchesCondition(Context, Entry))
        return;

    auto& Registration = Builder.RegisterType(Entry.ServiceClass);
    DL_LOG(Verbose, TEXT("Registered class '%s'"), *GetNameSafe(Entry.ServiceClass.Get()));

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
        if (!ProhibitedClasses.Contains(AdditionalClass) && ensure(Entry.ServiceClass->IsChildOf(AdditionalClass)))
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
