// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Camera/PlayerCameraManager.h"

#include "DLCameraManager.generated.h"

/**
 *  Basic First Person camera manager.
 *  Limits min/max look pitch.
 */
UCLASS()
class ADLCameraManager : public APlayerCameraManager
{
    GENERATED_BODY()
    
public:

    /** Constructor */
    ADLCameraManager();
};
