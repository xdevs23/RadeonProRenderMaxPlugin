#pragma once

#include "FireRenderMtlBase.h"

FIRERENDER_NAMESPACE_BEGIN;

enum FREmissiveIntMode {
	FREmissiveMtl_IntMode_Watts,
	FREmissiveMtl_IntMode_Lumens
};

enum FREmissiveColorMode {
	FREmissiveMtl_ColorMode_RGB,
	FREmissiveMtl_ColorMode_Kelvin
};

enum FREmissiveMtl_TexmapId {
	FREmissiveMtl_TEXMAP_COLOR = 0
};

enum FREmissiveMtl_ParamID : ParamID {
	FREmissiveMtl_COLOR = 1000,
	FREmissiveMtl_WATTS = 1001,
	FREmissiveMtl_LUMENS = 1003,
	FREmissiveMtl_INTENSITY_MODE = 1004,
	FREmissiveMtl_KELVIN = 1005,
	FREmissiveMtl_COLOR_MODE = 1006,
	FREmissiveMtl_KELVINSWATCH = 1007,
	FREmissiveMtl_COLOR_TEXMAP = 1008,
};

BEGIN_DECLARE_FRMTLCLASSDESC(EmissiveMtl, L"RPR Emissive Material", FIRERENDER_EMISSIVEMTL_CID)
END_DECLARE_FRMTLCLASSDESC()

BEGIN_DECLARE_FRMTL(EmissiveMtl)

virtual Color GetDiffuse(int mtlNum, BOOL backFace) override {
	return GetFromPb<Color>(pblock, FREmissiveMtl_COLOR);
}

virtual float GetSelfIllum(int mtlNum, BOOL backFace) override {
	return 1.f;
}

virtual Color GetSelfIllumColor(int mtlNum, BOOL backFace) override {
	return GetFromPb<Color>(pblock, FREmissiveMtl_COLOR);
}

END_DECLARE_FRMTL(EmissiveMtl)

FIRERENDER_NAMESPACE_END;
