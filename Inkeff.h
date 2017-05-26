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

/*
	Inkeff.h
*/

#pragma once

#ifndef INKEFF_H
#define INKEFF_H

typedef unsigned char		u_char;
typedef unsigned short		u_short;
typedef unsigned short		u_int16;
typedef unsigned long		u_long;
typedef short int			int16;
typedef float				fpshort;
#define PF_TABLE_BITS	12
#define PF_TABLE_SZ_16	4096

#define PF_DEEP_COLOR_AWARE 1	// make sure we get 16bpc pixels; 
								// AE_Effect.h checks for this.

#include "AEConfig.h"

#ifdef AE_OS_WIN
	typedef unsigned short PixelType;
	#include <Windows.h>
#endif

#include "entry.h"
#include "AE_Effect.h"
#include "AE_EffectCB.h"
#include "AE_Macros.h"
#include "Param_Utils.h"
#include "AE_EffectCBSuites.h"
#include "String_Utils.h"
#include "AE_GeneralPlug.h"
#include "AEFX_ChannelDepthTpl.h"
#include "AEGP_SuiteHandler.h"

#include "Inkeff_Strings.h"

#include <iostream>
#include <vector>
#include <cairo.h>
#include "SimplexNoise.h"


/* Versioning information */

#define	MAJOR_VERSION	1
#define	MINOR_VERSION	0
#define	BUG_VERSION		0
#define	STAGE_VERSION	PF_Stage_DEVELOP
#define	BUILD_VERSION	1


/* Parameter defaults */

#define LAYERMINWIDTH       0
#define LAYERMINHEIGHT      0

#define CAIROMINX           0
#define CAIROMAXX           1.0
#define CAIROMINY           0
#define CAIROMAXY           1.0

#define	INKEFF_PARTICLESIZE_MIN         0
#define	INKEFF_PARTICLESIZE_MAX         25
#define	INKEFF_PARTICLESIZE_DFLT		5

#define	INKEFF_PARTICLEOPACITY_MIN		0
#define	INKEFF_PARTICLEOPACITY_MAX		1.0
#define	INKEFF_PARTICLEOPACITY_DFLT		.5

#define INKEFF_PARTICLESTEPS_MIN   	1
#define INKEFF_PARTICLESTEPS_MAX   	10000
#define INKEFF_PARTICLESTEPS_DFLT     500

#define INKEFF_PARTICLELIFETIME_MIN      1
#define INKEFF_PARTICLELIFETIME_MAX      500
#define INKEFF_PARTICLELIFETIME_DFLT     250

#define INKEFF_PARTICLESTEPMULTI_MIN          -150
#define INKEFF_PARTICLESTEPMULTI_MAX          150
#define INKEFF_PARTICLESTEPMULTI_DFLT         10

//#define INKEFF_PARTICLESTEPADDITION_MIN       0
//#define INKEFF_PARTICLESTEPADDITION_MAX       500
//#define INKEFF_PARTICLESTEPADDITION_DFLT      50

#define INKEFF_PARTICLECURVEXIFACTOR_MIN      -2
#define INKEFF_PARTICLECURVEXIFACTOR_MAX      2
#define INKEFF_PARTICLECURVEXIFACTOR_DFLT     0

#define INKEFF_PARTICLECURVEYIFACTOR_MIN      -2
#define INKEFF_PARTICLECURVEYIFACTOR_MAX      2
#define INKEFF_PARTICLECURVEYIFACTOR_DFLT     0

#define INKEFF_PARTICLECURVEXJFACTOR_MIN      -2
#define INKEFF_PARTICLECURVEXJFACTOR_MAX      2
#define INKEFF_PARTICLECURVEXJFACTOR_DFLT     0

#define INKEFF_PARTICLECURVEYJFACTOR_MIN      -2
#define INKEFF_PARTICLECURVEYJFACTOR_MAX      2
#define INKEFF_PARTICLECURVEYJFACTOR_DFLT     0

#define INKEFF_EMITTERX_DFLT              50L
#define INKEFF_EMITTERY_DFLT              50L
#define INKEFF_EMITTERRESTRICT_BOUNDS     0

#define INKEFF_VECTORSTEPS_MIN   	5
#define INKEFF_VECTORSTEPS_MAX   	1000
#define INKEFF_VECTORSTEPS_DFLT   100

#define INKEFF_VECTORSMOOTH_MIN   	.1
#define INKEFF_VECTORSMOOTH_MAX   	25
#define INKEFF_VECTORSMOOTH_DFLT      5

#define INKEFF_EMITTERSIZE_MIN   	1
#define INKEFF_EMITTERSIZE_MAX   	100
#define INKEFF_EMITTERSIZE_DFLT   3

#define INKEFF_EMITTERPRESSURE_MIN   	1
#define INKEFF_EMITTERPRESSURE_MAX   	500
#define INKEFF_EMITTERPRESSURE_DFLT     15



enum {
	INKEFF_INPUT = 0,
	INKEFF_PARTICLESIZE,
    INKEFF_PARTICLEOPACITY,
    INKEFF_PARTICLESTEPS,
    INKEFF_PARTICLESTEPMULTI,
//    INKEFF_PARTICLESTEPADDITION,
    INKEFF_PARTICLELIFETIME,
    INKEFF_PARTICLEFORM,
    INKEFF_PARTICLECURVERTOPIC_START,
    INKEFF_PARTICLECURVEXIFACTOR,
    INKEFF_PARTICLECURVEYIFACTOR,
    INKEFF_PARTICLECURVEXJFACTOR,
    INKEFF_PARTICLECURVEYJFACTOR,
    INKEFF_PARTICLECURVERTOPIC_END,
    INKEFF_EMITTERCENTER,
    INKEFF_EMITTERSIZE,
    INKEFF_EMITTERPRESSURE,
    INKEFF_EMITTERFORM,
    INKEFF_VECTORSTEPS,
    INKEFF_VECTORSMOOTH,
	INKEFF_NUM_PARAMS
};

enum {
	PARTICLESIZE_DISK_ID = 1,
    PARTICLEOPACITY_DISK_ID,
    PARTICLESTEPS_DISK_ID,
    PARTICLESTEPMULTI_DISK_ID,
//    PARTICLESTEPADDITION_DISK_ID,
    PARTICLELIFETIME_DISK_ID,
    PARTICLEFORM_DISK_ID,
    PARTICLECURVERTOPIC_START_DISK_ID,
    PARTICLECURVEXIFACTOR_DISK_ID,
    PARTICLECURVEYIFACTOR_DISK_ID,
    PARTICLECURVEXJFACTOR_DISK_ID,
    PARTICLECURVEYJFACTOR_DISK_ID,
    PARTICLECURVERTOPIC_END_DISK_ID,
    EMITTER_DISK_ID,
    EMITTERPRESSURE_DISK_ID,
    EMITTERFORM_DISK_ID,
    VECTORSTEPS_DISK_ID,
    VECTORSMOOTH_DISK_ID,
};

enum
{
	X_SLIDER_DISK_ID = 2,
	Y_SLIDER_DISK_ID,
	EMITTERCENTER_DISK_ID
};


typedef struct GainInfo{
	PF_FpLong	gainF;
} GainInfo, *GainInfoP, **GainInfoH;

#ifdef __cplusplus
	extern "C" {
#endif
	
DllExport	PF_Err 
EntryPointFunc(	
	PF_Cmd			cmd,
	PF_InData		*in_data,
	PF_OutData		*out_data,
	PF_ParamDef		*params[],
	PF_LayerDef		*output,
	void			*extra) ;


//helper func
inline u_char AlphaLookup(u_int16 inValSu, u_int16 inMaxSu)
{
    fpshort normValFp = 1.0f - (inValSu)/static_cast<fpshort>(inMaxSu);
    return static_cast<u_char>(normValFp*normValFp*0.8f*255);
}

//error checking macro
#define CHECK(err) {PF_Err err1 = err; if (err1 != PF_Err_NONE ){ throw PF_Err(err1);}}
        
#ifdef __cplusplus
}
#endif

#endif // INKEFF_H