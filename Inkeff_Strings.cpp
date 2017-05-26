/*******************************************************************/
/*                                                                 */
/*                      ADOBE CONFIDENTIAL                         */
/*                   _ _ _ _ _ _ _ _ _ _ _ _ _                     */
/*                                                                 */
/* Copyright 2007 Adobe Systems Incorporated                       */
/* All Rights Reserved.                                            */
/*                                                                 */
/* NOTICE:  All information contained herein is, and remains the   */
/* property of Adobe Systems Incorporated and its suppliers, if    */
/* any.  The intellectual and technical concepts contained         */
/* herein are proprietary to Adobe Systems Incorporated and its    */
/* suppliers and may be covered by U.S. and Foreign Patents,       */
/* patents in process, and are protected by trade secret or        */
/* copyright law.  Dissemination of this information or            */
/* reproduction of this material is strictly forbidden unless      */
/* prior written permission is obtained from Adobe Systems         */
/* Incorporated.                                                   */
/*                                                                 */
/*******************************************************************/

#include "Inkeff.h"

typedef struct {
	A_u_long	index;
	A_char		str[256];
} TableString;

TableString		g_strs[StrID_NUMTYPES] = {
	StrID_NONE,						"",
	StrID_Name,						"Inkeffectum",
	StrID_Description,				"Particle and line droplets generator.\rThis is free unstable version of plugin.\rUse at your own risk.\rMike Tune, 2016\rhttp://onemore.site",
	StrID_ParticleSize_Name,        "Particle size",
    StrID_ParticleOpacity_Name,     "Particle opacity",
    StrID_ParticleSteps_Name,       "Particle steps",
    StrID_PatricleStepMulti_Name,   "Step multiplicator",
//    StrID_PatricleStepAddition_Name,"Step addition",
    StrID_ParticleLifetime,         "Particle lifetime",
    StrID_ParticleType_Name,        "Particle type",
    StrID_ParticleType_Choises,     "Circle|"
                                    "Square|"
                                    "Line|"
                                    "Curve",
    StrID_ParticleCurves_TopicName, "Curve Beziers",
    StrID_ParticleCurveXi_Name,     "Bezier Xi",
    StrID_ParticleCurveYi_Name,     "Bezier Yi",
    StrID_ParticleCurveXj_Name,     "Bezier Xj",
    StrID_ParticleCurveYj_Name,     "Bezier Yj",
    StrID_EmitterCenter_Name,		"Emitter position",
    StrID_EmitterSize_Name,         "Emitter size",
    StrID_EmitterPressure_Name,     "Emitter depressure",
    StrID_EmitterForm_Name,         "Emitter form",
    StrID_EmitterForm_Choises,		"Circle|"
                                    "Square"
                                    //"(-|"
                                    //"Plan A|"
                                    //"Plan B"
                                    ,
    StrID_VectorSteps_Name,         "Vector steps",
    StrID_VectorSmooth_Name,        "Vector smoothness",};


char	*GetStringPtr(int strNum)
{
	return g_strs[strNum].str;
}
	