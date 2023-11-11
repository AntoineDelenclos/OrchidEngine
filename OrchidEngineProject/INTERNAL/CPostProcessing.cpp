#include "CPostProcessing.h"

CPostProcessing::CPostProcessing() {
	this->glfPOPBrightness = 0.0f;
	this->glfPOPContrast = 1.0f;
	this->glfPOPSaturation = 1.0f;
	this->glfPOPGamma = 1.0f;
	this->pglfPOPBrightnessLimits = new GLfloat[2];
	this->pglfPOPContrastLimits = new GLfloat[2];
	this->pglfPOPSaturationLimits = new GLfloat[2];
	this->pglfPOPGammaLimits = new GLfloat[2];
	this->bPOPNormeRec709 = false;
}

CPostProcessing::~CPostProcessing() {
	delete(this->pglfPOPBrightnessLimits);
	delete(this->pglfPOPContrastLimits);
	delete(this->pglfPOPSaturationLimits);
	delete(this->pglfPOPGammaLimits);
}

void CPostProcessing::POPSetBrightness(GLfloat brightness) {
	this->glfPOPBrightness = brightness;
}

void CPostProcessing::POPSetContrast(GLfloat contrast) {
	this->glfPOPContrast = contrast;
}

void CPostProcessing::POPSetSaturation(GLfloat saturation) {
	this->glfPOPSaturation = saturation;
}

void CPostProcessing::POPSetGamma(GLfloat gamma) {
	this->glfPOPGamma = gamma;
}

GLfloat CPostProcessing::glfPOPGetBrightness() {
	return this->glfPOPBrightness;
}

GLfloat CPostProcessing::glfPOPGetContrast() {
	return this->glfPOPContrast;
}

GLfloat CPostProcessing::glfPOPGetSaturation() {
	return this->glfPOPSaturation;
}

GLfloat CPostProcessing::glfPOPGetGamma() {
	return this->glfPOPGamma;
}

void CPostProcessing::POPSetBrightnessLimits(GLfloat below, GLfloat above) {
	if (below > above) {
		CException exception(INCOHERENCE_LIMITES);
		throw(exception);
	}
	if (!this->pglfPOPBrightnessLimits) {
		CException exception(MODIF_SUR_OBJET_NON_INIT);
		throw(exception);
	}
	this->pglfPOPBrightnessLimits[0] = below;
	this->pglfPOPBrightnessLimits[1] = above;
}

void CPostProcessing::POPSetContrastLimits(GLfloat below, GLfloat above) {
	if (below > above) {
		CException exception(INCOHERENCE_LIMITES);
		throw(exception);
	}
	if (!this->pglfPOPContrastLimits) {
		CException exception(MODIF_SUR_OBJET_NON_INIT);
		throw(exception);
	}
	this->pglfPOPContrastLimits[0] = below;
	this->pglfPOPContrastLimits[1] = above;
}

void CPostProcessing::POPSetSaturationLimits(GLfloat below, GLfloat above) {
	if (below > above) {
		CException exception(INCOHERENCE_LIMITES);
		throw(exception);
	}
	if (!this->pglfPOPSaturationLimits) {
		CException exception(MODIF_SUR_OBJET_NON_INIT);
		throw(exception);
	}
	this->pglfPOPSaturationLimits[0] = below;
	this->pglfPOPSaturationLimits[1] = above;
}

void CPostProcessing::POPSetGammaLimits(GLfloat below, GLfloat above) {
	if (below > above) {
		CException exception(INCOHERENCE_LIMITES);
		throw(exception);
	}
	if (!this->pglfPOPGammaLimits) {
		CException exception(MODIF_SUR_OBJET_NON_INIT);
		throw(exception);
	}
	this->pglfPOPGammaLimits[0] = below;
	this->pglfPOPGammaLimits[1] = above;
}

GLfloat* CPostProcessing::pglfPOPGetBrightnessLimits() {
	return this->pglfPOPBrightnessLimits;
}

GLfloat* CPostProcessing::pglfPOPGetContrastLimits() {
	return this->pglfPOPContrastLimits;
}

GLfloat* CPostProcessing::pglfPOPGetSaturationLimits() {
	return this->pglfPOPSaturationLimits;
}

GLfloat* CPostProcessing::pglfPOPGetGammaLimits() {
	return this->pglfPOPGammaLimits;
}

void CPostProcessing::POPSetNormeRec709(bool bNorme) {
	this->bPOPNormeRec709 = bNorme;
}