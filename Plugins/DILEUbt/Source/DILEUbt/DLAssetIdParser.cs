using System.Diagnostics.CodeAnalysis;
using System.Text;
using EpicGames.UHT.Tables;
using EpicGames.UHT.Types;
using EpicGames.UHT.Utils;

namespace DILE;

[UnrealHeaderTool]
public static class DLAssetIdParser
{
	class DLAssetIdProperty : UhtStructProperty
	{
		public string AssetType { get; init; }

		public DLAssetIdProperty(UhtPropertySettings propertySettings, UhtScriptStruct scriptStruct, string assetType)
			: base(propertySettings, scriptStruct)
		{
			AssetType = assetType;
		}

		public override StringBuilder AppendText(StringBuilder builder, UhtPropertyTextType textType, bool isTemplateArgument)
		{
			return builder.Append("TDLAssetId<").Append(AssetType).Append('>');
		}
	}

	static UhtClass? PrimaryDataAssetClass;
	static UhtScriptStruct? PrimaryAssetIdStruct;

	[UhtPropertyType(Keyword = "TDLAssetId")]
	[SuppressMessage("CodeQuality", "IDE0051:Remove unused private members", Justification = "Attribute accessed method")]
	[SuppressMessage("Style", "IDE0060:Remove unused parameter", Justification = "Attribute accessed method")]
	private static UhtProperty? ParseDLAssetIdProperty(UhtPropertyResolveArgs args)
	{
		PrimaryDataAssetClass ??= args.Session.FindType(null, UhtFindOptions.Class | UhtFindOptions.EngineName, "PrimaryDataAsset") as UhtClass;
		PrimaryAssetIdStruct ??= args.Session.FindType(null, UhtFindOptions.ScriptStruct | UhtFindOptions.EngineName, "PrimaryAssetId") as UhtScriptStruct;

		var assetClass = args.ParseTemplateClass();
		if (assetClass == null || PrimaryDataAssetClass == null || PrimaryAssetIdStruct == null)
		{
			return null;
		}

		if (!assetClass.IsChildOf(PrimaryDataAssetClass))
		{
			args.TokenReader.LogError($"Template parameter of TDLAssetId<{assetClass.SourceName}> must be subclass of {PrimaryDataAssetClass.SourceName}");
			return null;
		}

		return new DLAssetIdProperty(args.PropertySettings, PrimaryAssetIdStruct, assetClass.SourceName);
	}
}
