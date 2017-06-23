/*********************************************************************************************************************************
* Radeon ProRender for 3ds Max plugin
* Copyright (c) 2017 AMD
* All Rights Reserved
*
* Radeon ProRender dedicated custom material plugin
*********************************************************************************************************************************/
#include "FireRenderDiffuseRefractionMtl.h"
#include "FireRenderNormalMtl.h"
#include "Resource.h"
#include "parser\MaterialParser.h"
#include "maxscript\mxsplugin\mxsPlugin.h"

FIRERENDER_NAMESPACE_BEGIN;

IMPLEMENT_FRMTLCLASSDESC(DiffuseRefractionMtl)

FRMTLCLASSDESCNAME(DiffuseRefractionMtl) FRMTLCLASSNAME(DiffuseRefractionMtl)::ClassDescInstance;

// All parameters of the material plugin. See FIRE_MAX_PBDESC definition for notes on backwards compatibility
static ParamBlockDesc2 pbDesc(
	0, _T("DiffuseRefractionMtlPbdesc"), 0, &FRMTLCLASSNAME(DiffuseRefractionMtl)::ClassDescInstance, P_AUTO_CONSTRUCT + P_AUTO_UI + P_VERSION, FIRERENDERMTLVER_LATEST, 0,
    //rollout
	IDD_FIRERENDER_DIFFUSEREFRACTIONMTL, IDS_FR_MTL_DIFFUSEREFRACTION, 0, 0, NULL,

	FRDiffuseRefractionMtl_COLOR, _T("diffuseColor"), TYPE_RGBA, P_ANIMATABLE, 0,
    p_default, Color(0.5f, 0.5f, 0.5f), p_ui, TYPE_COLORSWATCH, IDC_DIFFUSE_COLOR, PB_END,

	FRDiffuseRefractionMtl_ROUGHNESS, _T("diffuseRougness"), TYPE_FLOAT, P_ANIMATABLE, 0,
    p_default, 1.f, p_range, 0.f, 1.f, p_ui, TYPE_SPINNER, EDITTYPE_FLOAT, IDC_DIFFUSE_ROUGHNESS, IDC_DIFFUSE_ROUGHNESS_S, SPIN_AUTOSCALE, PB_END,

	FRDiffuseRefractionMtl_COLOR_TEXMAP, _T("diffuseTexmap"), TYPE_TEXMAP, 0, 0,
	p_subtexno, FRDiffuseRefractionMtl_TEXMAP_DIFFUSE, p_ui, TYPE_TEXMAPBUTTON, IDC_DIFFUSE_TEXMAP, PB_END,

	FRDiffuseRefractionMtl_ROUGHNESS_TEXMAP, _T("roughnessTexmap"), TYPE_TEXMAP, 0, 0,
	p_subtexno, FRDiffuseRefractionMtl_TEXMAP_ROUGHNESS, p_ui, TYPE_TEXMAPBUTTON, IDC_DIFFUSE_ROUGHNESS_TEXMAP, PB_END,

	FRDiffuseRefractionMtl_NORMALMAP, _T("normalTexmap"), TYPE_TEXMAP, 0, 0,
	p_subtexno, FRDiffuseRefractionMtl_TEXMAP_NORMAL, p_ui, TYPE_TEXMAPBUTTON, IDC_NORMAL_TEXMAP, PB_END,

    PB_END
    );

std::map<int, std::pair<ParamID, MCHAR*>> FRMTLCLASSNAME(DiffuseRefractionMtl)::TEXMAP_MAPPING = {
	{ FRDiffuseRefractionMtl_TEXMAP_DIFFUSE, { FRDiffuseRefractionMtl_COLOR_TEXMAP, _T("Color Map") } },
	{ FRDiffuseRefractionMtl_TEXMAP_ROUGHNESS, { FRDiffuseRefractionMtl_ROUGHNESS_TEXMAP, _T("Roughness Map") } },
	{ FRDiffuseRefractionMtl_TEXMAP_NORMAL, { FRDiffuseRefractionMtl_NORMALMAP, _T("Normal map") } }
};

FRMTLCLASSNAME(DiffuseRefractionMtl)::~FRMTLCLASSNAME(DiffuseRefractionMtl)()
{
}


frw::Shader FRMTLCLASSNAME(DiffuseRefractionMtl)::getShader(const TimeValue t, MaterialParser& mtlParser, INode* node)
{
	auto ms = mtlParser.materialSystem;

	frw::Shader material(ms, frw::ShaderTypeDiffuseRefraction);
		
	Texmap* normalTexmap = GetFromPb<Texmap*>(pblock, FRDiffuseRefractionMtl_NORMALMAP);
	const Color diffuseColor = GetFromPb<Color>(pblock, FRDiffuseRefractionMtl_COLOR);
	const float roughness = GetFromPb<float>(pblock, FRDiffuseRefractionMtl_ROUGHNESS);
	Texmap* diffuseTexmap = GetFromPb<Texmap*>(pblock, FRDiffuseRefractionMtl_COLOR_TEXMAP);
	Texmap* roughnessTexmap = GetFromPb<Texmap*>(pblock, FRDiffuseRefractionMtl_ROUGHNESS_TEXMAP);
		
	frw::Value color(diffuseColor.r, diffuseColor.g, diffuseColor.b);
	if (diffuseTexmap)
		color = mtlParser.createMap(diffuseTexmap, 0);

	frw::Value roughnessv(roughness, roughness, roughness);
	if (roughnessTexmap)
		roughnessv = mtlParser.createMap(roughnessTexmap, MAP_FLAG_NOGAMMA);

	material.SetValue("color", color);
	material.SetValue("roughness", roughnessv);
	
	if (normalTexmap)
		material.SetValue("normal", FRMTLCLASSNAME(NormalMtl)::translateGenericBump(t, normalTexmap, 1.f, mtlParser));
	
    return material;
}

FIRERENDER_NAMESPACE_END;
