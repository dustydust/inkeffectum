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

#pragma once

typedef enum {
	StrID_NONE, 
	StrID_Name,
	StrID_Description,
	StrID_ParticleSize_Name,
    StrID_ParticleOpacity_Name,
    StrID_ParticleSteps_Name,
    StrID_PatricleStepMulti_Name,
//    StrID_PatricleStepAddition_Name,
    StrID_ParticleLifetime,
    StrID_ParticleType_Name,
    StrID_ParticleType_Choises,
    StrID_ParticleCurves_TopicName,
    StrID_ParticleCurveXi_Name,
    StrID_ParticleCurveYi_Name,
    StrID_ParticleCurveXj_Name,
    StrID_ParticleCurveYj_Name,
    StrID_EmitterCenter_Name,
    StrID_EmitterSize_Name,
    StrID_EmitterPressure_Name,
    StrID_EmitterForm_Name,
    StrID_EmitterForm_Choises,
    StrID_VectorSteps_Name,
    StrID_VectorSmooth_Name,
	StrID_NUMTYPES
} StrIDType;
