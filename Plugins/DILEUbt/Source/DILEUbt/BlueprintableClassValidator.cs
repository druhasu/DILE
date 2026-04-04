using System.Linq;
using EpicGames.Core;
using EpicGames.UHT.Tables;
using EpicGames.UHT.Types;
using EpicGames.UHT.Utils;

namespace DILE;

[UnrealHeaderTool]
public static class BlueprintableClassValidator
{
    private const string ModuleName = "DILE";
    private const string ExporterName = nameof(BlueprintableClassValidator);

    private static readonly string[] IgnoredBaseClassesNames = ["DataAsset", "ActorComponent", "DeveloperSettings"];

    [UhtExporter(Name = ExporterName, ModuleName = ModuleName, Options = UhtExporterOptions.Default)]
    public static void Validate(IUhtExportFactory factory)
    {
        var ignoredBaseClasses = IgnoredBaseClassesNames.Select(n => (UhtClass)factory.Session.FindType(null, UhtFindOptions.EngineName | UhtFindOptions.Class, n)!).ToArray();

        foreach (var uhtHeader in factory.Session.HeaderFiles)
        {
            if (uhtHeader.Module.IsPartOfEngine)
                continue;

            foreach (var uhtClass in uhtHeader.Children.OfType<UhtClass>())
            {
                if (!uhtClass.EngineName.StartsWith("DL"))
                    continue;

                if (ignoredBaseClasses.Any(uhtClass.IsChildOf))
                    continue;

                var editableProperty = uhtClass.Properties.FirstOrDefault(p => p.PropertyFlags.HasFlag(EPropertyFlags.Edit) && !p.PropertyFlags.HasFlag(EPropertyFlags.EditConst));

                if (editableProperty != null && !uhtClass.MetaData.ContainsKeyHierarchical(UhtNames.IsBlueprintBase))
                {
                    uhtClass.LogError($"{uhtClass.SourceName} contains editable UProperties and must have 'Blueprintable' specifier");
                }
            }
        }
    }
}
