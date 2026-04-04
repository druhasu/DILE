// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "Engine/DataTable.h"
#include "Systems/NamedStruct/DataStructName.h"

#include "ColorStyle.generated.h"

struct FColorStyleTableRow;

/*
  Name of a ColorStyle
 */
USTRUCT()
struct FColorStyleName : public FDataStructName
#if CPP
    , public TDataStructName<FColorStyleName, FColorStyleTableRow>
#endif
{
    GENERATED_BODY()
    IMPLEMENT_DATA_STRUCT_NAME(FColorStyleName)
};

/*
  Predefined color value
 */
USTRUCT()
struct FColorStyleTableRow : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditDefaultsOnly)
    FLinearColor Color;
};
