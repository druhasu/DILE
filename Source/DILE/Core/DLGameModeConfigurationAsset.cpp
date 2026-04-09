// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLGameModeConfigurationAsset.h"
#include "DLFeatureConfigurationAsset.h"
#include "Utils/DLValidationUtils.h"

void UDLGameModeConfigurationAsset::RegisterClassesForActor(FObjectContainerBuilder& Builder, const FDLActorContainerConfiguratorContext& Context) const
{
    TQueue<const UDLFeatureConfigurationAsset*, EQueueMode::SingleThreaded> FeatureQueue;
    TSet<const UDLFeatureConfigurationAsset*> AllFeatures;

    for (const UDLFeatureConfigurationAsset* FeatureAsset : Features)
    {
        FeatureQueue.Enqueue(FeatureAsset);
    }

    const UDLFeatureConfigurationAsset* Feature = nullptr;
    while (FeatureQueue.Dequeue(Feature))
    {
        if (!ensure(Feature != nullptr))
            continue;

        bool bAlreadyInSet = false;
        AllFeatures.Add(Feature, &bAlreadyInSet);

        if (!bAlreadyInSet)
        {
            for (const UDLFeatureConfigurationAsset* RequiredFeature : Feature->RequiredFeatures)
                FeatureQueue.Enqueue(RequiredFeature);
        }
    }

    for (const UDLFeatureConfigurationAsset* FeatureAsset : AllFeatures)
    {
        FeatureAsset->RegisterClassesForActor(Builder, Context);
    }
}

#if WITH_EDITOR
EDataValidationResult UDLGameModeConfigurationAsset::IsDataValid(FDataValidationContext& Context) const
{
    for (int32 Index = 0; Index < Features.Num(); ++Index)
    {
        DL_VALIDATE_DATA(Features[Index] != nullptr, "Features contain invalid reference at index {0}", Index);
    }

    return DL_VALIDATION_RESULT();
}
#endif
