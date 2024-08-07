// Copyright Andrei Sudarikov. All Rights Reserved.

#include "Modules/ModuleManager.h"

#include "Systems/NamedStruct/NamedStructRegistry.h"
#include "Systems/NamedStruct/DataStructNamePropertyCustomization.h"
#include "Systems/NamedStruct/DataStructNameGraphPanelPinFactory.h"

class FDILEEditorModule : public IModuleInterface
{
public:
    using ThisClass = FDILEEditorModule;

    void StartupModule() override
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        for (auto StructType : FNamedStructRegistry::Get().GetAllNameStructs())
        {
            PropertyModule.RegisterCustomPropertyTypeLayout(StructType->GetFName(), FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FDataStructNamePropertyCustomization::Make, StructType));
            PropertyTypeNames.Add(StructType->GetFName());
        }

        DataStructNameGraphPanelPinFactory = MakeShared<FDataStructNameGraphPanelPinFactory>();
        FEdGraphUtilities::RegisterVisualPinFactory(DataStructNameGraphPanelPinFactory);
    }

    void ShutdownModule() override
    {
        FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");

        for (const FName& StructName : PropertyTypeNames)
        {
            PropertyModule.UnregisterCustomPropertyTypeLayout(StructName);
        }
        PropertyTypeNames.Reset();

        FEdGraphUtilities::UnregisterVisualPinFactory(DataStructNameGraphPanelPinFactory);
        DataStructNameGraphPanelPinFactory.Reset();
    }

private:
    TArray<FName> PropertyTypeNames;
    TSharedPtr<FDataStructNameGraphPanelPinFactory> DataStructNameGraphPanelPinFactory;
};

IMPLEMENT_MODULE( FDILEEditorModule, DILEEditor );
