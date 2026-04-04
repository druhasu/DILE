// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Templates/SharedPointer.h"

struct FDLAbstractSessionID : TSharedFromThis<FDLAbstractSessionID>
{
};

struct FDLSessionHandle
{
private:
    TSharedPtr<FDLAbstractSessionID> InternalID;
};
