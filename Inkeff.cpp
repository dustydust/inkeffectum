
/*
 
 Downsamples
 Follow Map
 Independent particle size // Random size // Noise size
 + Global size
 
 Independent particle size can only with another algorhitm of build particles.
 
 */


#include "Inkeff.h"

struct emitterPoint
{
    A_long      x, y, stepsMax;
    A_FpShort   stepMulti;
};

class WorldParams
{
    int set;
    static WorldParams *s_instance;
    
    PF_ParamDef *wParams;
    A_long  layerWidth = 0,
            layerHeight = 0,
            currentFrame = 0;
    
    A_short downSampleX = 0,
            downSampleY = 0;
    
    WorldParams(int v = 0) { set = v; }
    
public:
    
    static WorldParams *instance()
    {
        if (!s_instance)
            s_instance = new WorldParams;
        return s_instance;
    }
    
    void setParams          (PF_ParamDef *param[])      { wParams = *param; }
    void setCurrentFrame    (A_long cFrame)             { currentFrame = cFrame; }
    void setDownSamples     (A_short dX, A_short dY)    { downSampleX = dX; downSampleY = dY; }
    
    A_short getDownSampleX() { return downSampleX; }
    A_short getDownSampleY() { return downSampleY; }
    
    PF_FpLong   getParticleSize()       { return wParams[INKEFF_PARTICLESIZE].u.fs_d.value; }
    A_long      getParticleSteps()      { return wParams[INKEFF_PARTICLESTEPS].u.fs_d.value; }
    A_long      getParticleLifetime()   { return wParams[INKEFF_PARTICLELIFETIME].u.fs_d.value; }
    A_long      getParticleForm()       { return wParams[INKEFF_PARTICLEFORM].u.pd.value; }
    A_FpLong    getParticleCXiFactor()  { return wParams[INKEFF_PARTICLECURVEXIFACTOR].u.fs_d.value; }
    A_FpLong    getParticleCYiFactor()  { return wParams[INKEFF_PARTICLECURVEYIFACTOR].u.fs_d.value; }
    A_FpLong    getParticleCXjFactor()  { return wParams[INKEFF_PARTICLECURVEXJFACTOR].u.fs_d.value; }
    A_FpLong    getParticleCYjFactor()  { return wParams[INKEFF_PARTICLECURVEYJFACTOR].u.fs_d.value; }
    
    A_long      getVectorSteps()        { return wParams[INKEFF_VECTORSTEPS].u.fs_d.value; }
    A_FpShort   getSmoothness()         { return wParams[INKEFF_VECTORSMOOTH].u.fs_d.value; }
    A_FpShort   getStepMupliplicator()  { return wParams[INKEFF_PARTICLESTEPMULTI].u.fs_d.value; }
//    A_long      getStepAddition()       { return wParams[INKEFF_PARTICLESTEPADDITION].u.fs_d.value; }
    A_FpShort   getParticleOpacity()    { return wParams[INKEFF_PARTICLEOPACITY].u.fs_d.value; }
    
    A_long      getEmitterSize()        { return wParams[INKEFF_EMITTERSIZE].u.fs_d.value; }
    A_long      getEmitterPressure()    { return wParams[INKEFF_EMITTERPRESSURE].u.fs_d.value; }
    A_long      getEmitterX()           { return FIX_2_FLOAT(wParams[INKEFF_EMITTERCENTER].u.td.x_value); }
    A_long      getEmitterY()           { return FIX_2_FLOAT(wParams[INKEFF_EMITTERCENTER].u.td.y_value); }
    A_long      getEmitterForm()        { return wParams[INKEFF_EMITTERFORM].u.pd.value; }
    A_long      getCurrentFrame()       { return currentFrame; }
    
    
    void setLayerSize(A_long width, A_long height)
    {
        layerWidth  = width;
        layerHeight = height;
    }
    
    A_long getLayerWidth() { return layerWidth; }
    A_long getLayerHeight() { return layerHeight; }
};

// Allocating and initializing GlobalClass's
// static data member.  The pointer is being
// allocated - not the object inself.
WorldParams *WorldParams::s_instance = 0;


static PF_Err
ParamsSetup (
             PF_InData		*in_data,
             PF_OutData		*out_data,
             PF_ParamDef	*params[],
             PF_LayerDef	*output )
{
	PF_Err		err		= PF_Err_NONE;
	PF_ParamDef	def;
    
    /* PARTICLE */
    
	AEFX_CLR_STRUCT(def);
    
	PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleSize_Name),
                         INKEFF_PARTICLESIZE_MIN,
                         INKEFF_PARTICLESIZE_MAX,
                         INKEFF_PARTICLESIZE_MIN,
                         INKEFF_PARTICLESIZE_MAX,
                         INKEFF_PARTICLESIZE_DFLT,
                         PF_Precision_HUNDREDTHS,
                         0,
                         0,
                         PARTICLESIZE_DISK_ID);
    
	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleOpacity_Name),
                         INKEFF_PARTICLEOPACITY_MIN,
                         INKEFF_PARTICLEOPACITY_MAX,
                         INKEFF_PARTICLEOPACITY_MIN,
                         INKEFF_PARTICLEOPACITY_MAX,
                         INKEFF_PARTICLEOPACITY_DFLT,
                         PF_Precision_THOUSANDTHS,
                         0,
                         0,
                         PARTICLEOPACITY_DISK_ID);
    
	AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleSteps_Name),
                         INKEFF_PARTICLESTEPS_MIN,
                         INKEFF_PARTICLESTEPS_MAX,
                         INKEFF_PARTICLESTEPS_MIN,
                         INKEFF_PARTICLESTEPS_MAX,
                         INKEFF_PARTICLESTEPS_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         PARTICLESTEPS_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_PatricleStepMulti_Name),
                         INKEFF_PARTICLESTEPMULTI_MIN,
                         INKEFF_PARTICLESTEPMULTI_MAX,
                         INKEFF_PARTICLESTEPMULTI_MIN,
                         INKEFF_PARTICLESTEPMULTI_MAX,
                         INKEFF_PARTICLESTEPMULTI_DFLT,
                         PF_Precision_HUNDREDTHS,
                         0,
                         0,
                         PARTICLESTEPMULTI_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
//    PF_ADD_FLOAT_SLIDERX(STR(StrID_PatricleStepAddition_Name),
//                         INKEFF_PARTICLESTEPADDITION_MIN,
//                         INKEFF_PARTICLESTEPADDITION_MAX,
//                         INKEFF_PARTICLESTEPADDITION_MIN,
//                         INKEFF_PARTICLESTEPADDITION_MAX,
//                         INKEFF_PARTICLESTEPADDITION_DFLT,
//                         PF_Precision_INTEGER,
//                         0,
//                         0,
//                         PARTICLESTEPADDITION_DISK_ID);
//    
//    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleLifetime),
                         INKEFF_PARTICLELIFETIME_MIN,
                         INKEFF_PARTICLELIFETIME_MAX,
                         INKEFF_PARTICLELIFETIME_MIN,
                         INKEFF_PARTICLELIFETIME_MAX,
                         INKEFF_PARTICLELIFETIME_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         PARTICLELIFETIME_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_POPUP(STR(StrID_ParticleType_Name),
                 2,
                 1,
                 STR(StrID_ParticleType_Choises),
                 PARTICLEFORM_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
        // Curve factors
        PF_ADD_TOPIC(STR(StrID_ParticleCurves_TopicName), PARTICLECURVERTOPIC_START_DISK_ID);
    
            AEFX_CLR_STRUCT(def);
        
            PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleCurveXi_Name),
                                 INKEFF_PARTICLECURVEXIFACTOR_MIN,
                                 INKEFF_PARTICLECURVEXIFACTOR_MAX,
                                 INKEFF_PARTICLECURVEXIFACTOR_MIN,
                                 INKEFF_PARTICLECURVEXIFACTOR_MAX,
                                 INKEFF_PARTICLECURVEXIFACTOR_DFLT,
                                 PF_Precision_THOUSANDTHS,
                                 0,
                                 0,
                                 PARTICLECURVEXIFACTOR_DISK_ID);
            
            AEFX_CLR_STRUCT(def);
            
            PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleCurveYi_Name),
                                 INKEFF_PARTICLECURVEYIFACTOR_MIN,
                                 INKEFF_PARTICLECURVEYIFACTOR_MAX,
                                 INKEFF_PARTICLECURVEYIFACTOR_MIN,
                                 INKEFF_PARTICLECURVEYIFACTOR_MAX,
                                 INKEFF_PARTICLECURVEYIFACTOR_DFLT,
                                 PF_Precision_THOUSANDTHS,
                                 0,
                                 0,
                                 PARTICLECURVEYIFACTOR_DISK_ID);
            
            AEFX_CLR_STRUCT(def);
            
            PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleCurveXj_Name),
                                 INKEFF_PARTICLECURVEXJFACTOR_MIN,
                                 INKEFF_PARTICLECURVEXJFACTOR_MAX,
                                 INKEFF_PARTICLECURVEXJFACTOR_MIN,
                                 INKEFF_PARTICLECURVEXJFACTOR_MAX,
                                 INKEFF_PARTICLECURVEXJFACTOR_DFLT,
                                 PF_Precision_THOUSANDTHS,
                                 0,
                                 0,
                                 PARTICLECURVEXJFACTOR_DISK_ID);
            
            AEFX_CLR_STRUCT(def);
            
            PF_ADD_FLOAT_SLIDERX(STR(StrID_ParticleCurveYj_Name),
                                 INKEFF_PARTICLECURVEYJFACTOR_MIN,
                                 INKEFF_PARTICLECURVEYJFACTOR_MAX,
                                 INKEFF_PARTICLECURVEYJFACTOR_MIN,
                                 INKEFF_PARTICLECURVEYJFACTOR_MAX,
                                 INKEFF_PARTICLECURVEYJFACTOR_DFLT,
                                 PF_Precision_THOUSANDTHS,
                                 0,
                                 0,
                                 PARTICLECURVEYJFACTOR_DISK_ID);
            
            AEFX_CLR_STRUCT(def);
    
        PF_END_TOPIC(PARTICLECURVERTOPIC_END_DISK_ID);
    
        AEFX_CLR_STRUCT(def);
        
        // Curve factors : end

    /* PARTICLE : END */
    
    
    /* EMITTER */
    
	PF_ADD_POINT(STR(StrID_EmitterCenter_Name),
                 INKEFF_EMITTERX_DFLT,
                 INKEFF_EMITTERY_DFLT,
                 INKEFF_EMITTERRESTRICT_BOUNDS,
                 EMITTERCENTER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_EmitterSize_Name),
                         INKEFF_EMITTERSIZE_MIN,
                         INKEFF_EMITTERSIZE_MAX,
                         INKEFF_EMITTERSIZE_MIN,
                         INKEFF_EMITTERSIZE_MAX,
                         INKEFF_EMITTERSIZE_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         EMITTER_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_EmitterPressure_Name),
                         INKEFF_EMITTERPRESSURE_MIN,
                         INKEFF_EMITTERPRESSURE_MAX,
                         INKEFF_EMITTERPRESSURE_MIN,
                         INKEFF_EMITTERPRESSURE_MAX,
                         INKEFF_EMITTERPRESSURE_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         EMITTERPRESSURE_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_POPUP(STR(StrID_EmitterForm_Name),
                 2,
                 1,
                 STR(StrID_EmitterForm_Choises),
                 EMITTERFORM_DISK_ID);
    
    /* EMITTER : END */
    
    AEFX_CLR_STRUCT(def);

    PF_ADD_FLOAT_SLIDERX(STR(StrID_VectorSteps_Name),
                         INKEFF_VECTORSTEPS_MIN,
                         INKEFF_VECTORSTEPS_MAX,
                         INKEFF_VECTORSTEPS_MIN,
                         INKEFF_VECTORSTEPS_MAX,
                         INKEFF_VECTORSTEPS_DFLT,
                         PF_Precision_INTEGER,
                         0,
                         0,
                         VECTORSTEPS_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    PF_ADD_FLOAT_SLIDERX(STR(StrID_VectorSmooth_Name),
                         INKEFF_VECTORSMOOTH_MIN,
                         INKEFF_VECTORSMOOTH_MAX,
                         INKEFF_VECTORSMOOTH_MIN,
                         INKEFF_VECTORSMOOTH_MAX,
                         INKEFF_VECTORSMOOTH_DFLT,
                         PF_Precision_TENTHS,
                         0,
                         0,
                         VECTORSMOOTH_DISK_ID);
    
    AEFX_CLR_STRUCT(def);
    
    
	out_data->num_params = INKEFF_NUM_PARAMS;
    
	return err;
}


class PVector
{
    float x = 0, y = 0;
public:
    
    PVector(float x_, float y_) { x = x_; y = y_; }
    PVector() = default;
    
    float getX() { return x; }
    float getY() { return y; }
    
    void add(PVector v)     { x = x + v.getX(); y = y + v.getY(); }
    void sub(PVector v)     { x = x - v.x; y = y - v.y; }
    void mult(float n)      { x = x * n; y = y * n; }
    void div(float n)       { x = x / n; y = y / n; }
    float mag()             { return sqrt(x*x + y*y); }
    
    void normalize(PVector v)
    {
        float m = mag();
        if (m != 0) {
            div(m);
        }
    }
    
};

std::vector<PVector> vecrotFieldArr;

class NoiseMap
{
    std::vector<std::vector<A_long> > noiseMapArr;
    
    A_long      layerWidth        = WorldParams::instance()->getLayerWidth();
    A_long      layerHeight       = WorldParams::instance()->getLayerHeight();
    A_long      vectorSteps;
    A_FpShort   fieldSmoothness;
    
public:
    
    void initNoiseMap()
    {
        layerWidth      = WorldParams::instance()->getLayerWidth();
        layerHeight     = WorldParams::instance()->getLayerHeight();
        fieldSmoothness = WorldParams::instance()->getSmoothness();
        vectorSteps     = WorldParams::instance()->getVectorSteps();
        
        noiseMapArr.resize(layerHeight);
        
        for (int y = 0; y < layerHeight; y++)
        {
            noiseMapArr[y].resize(layerWidth);
            for (int x = 0; x < layerWidth; x++)
            {
                int noiseValue = SimplexNoise::noise(x * fieldSmoothness, y * fieldSmoothness) * vectorSteps;
                if (noiseValue < 0)
                {
                    noiseValue = noiseValue * -1;
                }
                
                noiseMapArr[y][x] = noiseValue;
            }
        }
    }
    
    A_long getNoisemapValue(A_long x, A_long y)
    {
        return noiseMapArr[y][x];
    }
    
};

NoiseMap noiseMap;

class Particle
{
    
    PVector *position, *velocity;
    std::vector<PVector> steps;
    A_long  x, y, sX, sY, stepsMax;
    float   stepMulti;
    
    PF_Err  err = PF_Err_NONE;
    
public:
    
    
    Particle(int _x, int _y, int _stepsMax, A_FpShort _stepsMulti)
    {
        sX          = _x;
        sY          = _y;
        x           = _x;
        y           = _y;
        stepsMax    = _stepsMax;
        stepMulti   = _stepsMulti;
        
        position = new PVector(sX, sY);
        velocity = new PVector();
        steps.insert(steps.begin(), PVector(sX, sY));
    }
    
    void move()
    {
        if ((x < WorldParams::instance()->getLayerWidth() && x > 0) && (y < WorldParams::instance()->getLayerHeight() && y > 0))
        {
            A_long noiseVal = noiseMap.getNoisemapValue(x, y);
            
            PVector vel = vecrotFieldArr[noiseVal]; // ERROR !!!!!!
            vel.mult(stepMulti);
            velocity->add(vel);
            position->add(*velocity);
            
            x = (A_long) position->getX();
            y = (A_long) position->getY();

            //s = brightness(imageLoaded.get(x, y)) >> 6;
            //s = 100;
            stepsMax--;
        }
        else
        {
            stepsMax = 0;
        }
        
    }
    
    void makeSteps(A_long currentFrameL)
    {
        steps.clear();
        steps.push_back(PVector(sX, sY));
        
        for (int i = 0; i <= currentFrameL; i++)
        {
            if (!isGone() == true)
            {
                move();
                steps.insert(steps.begin() + 1 + i, PVector(x, y));
            }
            else
            {
                break;
            }
        }
    }
    
    PF_Err trace(cairo_t *cairoContext, PF_InData *in_data)
    {
//        double  cairoCoordX,
//                cairoCoordY,
//                cairoSizeX,
//                cairoSizeY;
        A_long  prevX = 0,
                prevY = 0;
        
        A_long  currentFrame        = WorldParams::instance()->getCurrentFrame(),
                particleLifetime    = WorldParams::instance()->getParticleLifetime();
        
        A_long      removeFrom;
        A_long      UIParticleSize    = WorldParams::instance()->getParticleSize();
        A_FpShort   UIParticleOpacity = WorldParams::instance()->getParticleOpacity();
        A_FpLong    curveXiFactor     = WorldParams::instance()->getParticleCXiFactor(),
                    curveYiFactor     = WorldParams::instance()->getParticleCYiFactor(),
                    curveXjFactor     = WorldParams::instance()->getParticleCXjFactor(),
                    curveYjFactor     = WorldParams::instance()->getParticleCYjFactor();
        A_long      UIParticleFormId  = WorldParams::instance()->getParticleForm();
        
        cairo_set_source_rgba (cairoContext, 1, 1, 1, UIParticleOpacity);
        
        for (int i = 0; i < steps.size(); i++)
        {
            err = PF_PROGRESS(in_data, i, steps.size());
            removeFrom = (currentFrame - particleLifetime) + 1;
            
            if (removeFrom < i)
            {
                // Convert to another Coordinate System
                //cairoCoordX = ( steps[i].getX() - LAYERMINWIDTH ) / ( WorldParams::instance()->getLayerWidth()      - LAYERMINWIDTH ) * ( CAIROMAXX - CAIROMINX ) + CAIROMINX;
                //cairoCoordY = ( steps[i].getY() - LAYERMINHEIGHT ) / ( WorldParams::instance()->getLayerHeight()    - LAYERMINHEIGHT ) * ( CAIROMAXY - CAIROMINY ) + CAIROMINY;
                //cairoSizeX  = ( WorldParams::instance()->getParticleSize() - LAYERMINWIDTH ) / ( WorldParams::instance()->getLayerWidth()      - LAYERMINWIDTH ) * ( CAIROMAXX - CAIROMINX ) + CAIROMINX;
                //cairoSizeY  = ( WorldParams::instance()->getParticleSize() - LAYERMINHEIGHT ) / ( WorldParams::instance()->getLayerHeight()    - LAYERMINHEIGHT ) * ( CAIROMAXY - CAIROMINY ) + CAIROMINY;
            
                if (UIParticleFormId == 1) { // Draw Circle
                    cairo_arc (cairoContext, steps[i].getX(), steps[i].getY(), UIParticleSize, 0, 2 * M_PI);
                    cairo_fill (cairoContext);
                }
                else if (UIParticleFormId == 2) { // Draw Rectangle
                    cairo_rectangle (cairoContext, steps[i].getX() - (UIParticleSize / 2), steps[i].getY() - (UIParticleSize / 2), UIParticleSize, UIParticleSize);
                    cairo_fill (cairoContext);
                }
                else if (UIParticleFormId == 3) { // Draw Lines
                    if (prevX == 0 || prevY == 0) {
                        prevX = steps[i].getX(),
                        prevY = steps[i].getY();
                    } else {
                        cairo_move_to           (cairoContext, prevX, prevY);
                        cairo_line_to           (cairoContext, steps[i].getX(), steps[i].getY());
                        cairo_set_line_width    (cairoContext, UIParticleSize);
                        cairo_stroke            (cairoContext);
                        
                        prevX = steps[i].getX(),
                        prevY = steps[i].getY();
                    }
                }
                else if(UIParticleFormId == 4) { // Draw Curves
                    if (prevX == 0 || prevY == 0) {
                        prevX = steps[i].getX(),
                        prevY = steps[i].getY();
                    } else {
                        cairo_move_to           (cairoContext, prevX, prevY);
                        cairo_curve_to          (
                                                 cairoContext,
                                                 steps[i].getX() + (steps[i].getX() * curveXiFactor),
                                                 steps[i].getY() + (steps[i].getX() * curveYiFactor),
                                                 steps[i].getX() + (steps[i].getX() * curveXjFactor),
                                                 steps[i].getY() + (steps[i].getX() * curveYjFactor),
                                                 steps[i].getX(),
                                                 steps[i].getY()
                                                 );
                        cairo_set_line_width    (cairoContext, UIParticleSize);
                        cairo_stroke            (cairoContext);
                        
                        prevX = steps[i].getX(),
                        prevY = steps[i].getY();
                    }
                }
            }
        }
        
        return err;
    }
    
    void    draw()      { /* glColor3d(1, 1, 1); glVertex2d(position->getX(), position->getY());*/ }
    int     getX()      { return x; }
    int     getY()      { return y; }
    bool    isGone()    { return stepsMax == 0; }

};

class PWorld
{
    
    std::vector<Particle>       particles;
    std::vector<emitterPoint>   emitterArr;
    
    A_long                  vectorSteps,
                            emitterSize,
                            emitterPressure,
                            emitterPosX,
                            emitterPosY,
                            particleMultiply,
                            emitterForm,
                            particleSteps;
    
    A_FpShort               smoothness;

    
    A_long layerWidth = 0;
    A_long layerHeight = 0;
    
public:
    
    void buildWorld()
    {
        A_long currentFrameL = WorldParams::instance()->getCurrentFrame();
//        A_short dSampleFactorX = WorldParams::instance()->getDownSampleX(),
//                dSampleFactorY = WorldParams::instance()->getDownSampleY();
        for (int i = 0; i < emitterArr.size(); i++) {
            addParticle(
                        emitterArr[i].x,
                        emitterArr[i].y,
                        emitterArr[i].stepsMax,
                        emitterArr[i].stepMulti
                        );
        }
        for (int i = 0; i < particles.size(); i++)  {
            particles[i].makeSteps(currentFrameL);
        }
    }
    
    void drawWorld(cairo_t *cairoContext, PF_InData *in_data)
    {
        for (int i = 0; i < particles.size(); ++i) {
            particles[i].trace(cairoContext, in_data);
        }
    }
    
    void addParticle(A_long startX, A_long startY, A_long stepsMax, A_FpShort stepMulti)
    {
        particles.push_back(Particle(startX, startY, stepsMax, stepMulti));
    }
    
    void clearParticles()
    {
        particles.clear();
    }
    
    void reField()
    {
        A_long      UIVectorSteps       = WorldParams::instance()->getVectorSteps();
        A_FpShort   UISmoothness        = WorldParams::instance()->getSmoothness();
        A_long      UIEmitterPosX       = WorldParams::instance()->getEmitterX(),
                    UIEmitterPosY       = WorldParams::instance()->getEmitterY(),
                    UIEmitterSize       = WorldParams::instance()->getEmitterSize(),
                    UIEmitterPressure   = WorldParams::instance()->getEmitterPressure(),
                    UIParticleMulti     = WorldParams::instance()->getStepMupliplicator(),
                    UIEmitterForm       = WorldParams::instance()->getEmitterForm(),
                    UIParticleSteps     = WorldParams::instance()->getParticleSteps();
        
        if (
            vectorSteps      != UIVectorSteps        ||
            smoothness       != UISmoothness         ||
            emitterPosX      != UIEmitterPosX        ||
            emitterPosY      != UIEmitterPosY        ||
            emitterSize      != UIEmitterSize        ||
            emitterPressure  != UIEmitterPressure    ||
            particleMultiply != UIParticleMulti      ||
            emitterForm      != UIEmitterForm        ||
            particleSteps    != UIParticleSteps // ?
            )
        {
            if (
                vectorSteps     != UIVectorSteps        ||
                smoothness      != UISmoothness
                )
            {
                vectorSteps     = UIVectorSteps,
                smoothness      = UISmoothness;
                
                noiseMap.initNoiseMap();
                createVectorField();
            }
            
            emitterPosX      = UIEmitterPosX,
            emitterPosY      = UIEmitterPosY,
            emitterSize      = UIEmitterSize,
            emitterPressure  = UIEmitterPressure,
            particleMultiply = UIParticleMulti,
            emitterForm      = UIEmitterForm,
            particleSteps    = UIParticleSteps; // ?
            
            //createNoiseMap();
            
            makeEmitter();
        }
    }
    
    // Think about refresh noisemap and vectorfields separatly with Smoothness and Steps changes.
    // I mean, by logical, noisemap needs to refresh if each of this parametrs changed. 
    
    void createVectorField()
    {
        vecrotFieldArr.clear();
        for (int i = 0; i < vectorSteps; i++)
        {
            vecrotFieldArr.push_back(PVector(cos(i*.5)*.05, sin(i*.5)*.1)); // Use Insert Iterator?
        }
    }
    
    void makeEmitter()
    {

        int shapeX, shapeY, distanceSquared, rndIndex;
        
        layerWidth  = WorldParams::instance()->getLayerWidth();
        layerHeight = WorldParams::instance()->getLayerHeight();
        
        A_long UIEmitterSize        = WorldParams::instance()->getEmitterSize();
        A_long UIEmitterPressure    = WorldParams::instance()->getEmitterPressure();
        A_long UIEmitterPosX        = WorldParams::instance()->getEmitterX();
        A_long UIEmitterPosY        = WorldParams::instance()->getEmitterY();
        A_long UIEmitterFormId      = WorldParams::instance()->getEmitterForm();
        
        emitterSize         = UIEmitterSize;
        emitterPressure     = UIEmitterPressure;
        emitterPosX         = UIEmitterPosX;
        emitterPosY         = UIEmitterPosY;
    
        emitterArr.clear();
        
        for (int x = 0; x < layerWidth; x++)
        {
            for (int y = 0; y < layerHeight; y++)
            {
                if (UIEmitterFormId == 1) // If Circle
                {
                    // If the center is at (x0,y0) and the radius is r, then
                    // if (x-x0)^2 + (y-y0)^2 < r^2,     the point (x,y) is inside the circle,
                    // if (x-x0)^2 + (y-y0)^2 == r^2,    the point (x,y) is on the circle, and
                    // if (x-x0)^2 + (y-y0)^2 > r^2,     the point (x,y) is outside the circle.
                    
                    shapeX = x - emitterPosX;
                    shapeY = y - emitterPosY;
                    
                    distanceSquared = shapeX * shapeX + shapeY * shapeY;
                    
                    if ((distanceSquared <= (emitterSize * emitterSize)))
                    {
                        emitterPoint emitterPixel;
                        emitterPixel.x            = x;
                        emitterPixel.y            = y;
                        emitterPixel.stepsMax     = WorldParams::instance()->getParticleSteps();
                        emitterPixel.stepMulti    = WorldParams::instance()->getStepMupliplicator();
                        emitterArr.push_back(emitterPixel);
                    }
                    
                }
                if (UIEmitterFormId == 2) // If Square
                {
                    if (
                        (x >= (emitterPosX - (emitterSize / 2)) && x <= (emitterPosX + (emitterSize / 2))) &&
                        (y >= (emitterPosY - (emitterSize / 2)) && y <= (emitterPosY + (emitterSize / 2)))
                        )
                    {
                        emitterPoint emitterPixel;
                        emitterPixel.x            = x;
                        emitterPixel.y            = y;
                        emitterPixel.stepsMax     = WorldParams::instance()->getParticleSteps();
                        emitterPixel.stepMulti    = WorldParams::instance()->getStepMupliplicator();
                        emitterArr.push_back(emitterPixel);
                    }
                }
            }
        }
    
        // Emitter Depressure
    
        A_long  min = 0,
                max = emitterArr.size();
    
        for (int i = 0; i < emitterArr.size(); i++)
        {
            rndIndex = min + (rand() % (A_long)(max - min + 1));
            if (rndIndex < (emitterPressure * 100) )
            {
                emitterArr.erase(emitterArr.begin() + i);
                emitterArr.erase(emitterArr.begin() + rndIndex); /// ???
            }
        }
    }
    
};

PWorld pworld;


static PF_Err
About (
       PF_InData		*in_data,
       PF_OutData		*out_data,
       PF_ParamDef		*params[],
       PF_LayerDef		*output )
{
	AEGP_SuiteHandler suites(in_data->pica_basicP);
	
	suites.ANSICallbacksSuite1()->sprintf(	out_data->return_msg,
                                          "%s v%d.%d\r%s",
                                          STR(StrID_Name),
                                          MAJOR_VERSION,
                                          MINOR_VERSION,
                                          STR(StrID_Description));
	return PF_Err_NONE;
}

static PF_Err
GlobalSetup (
             PF_InData		*in_data,
             PF_OutData		*out_data,
             PF_ParamDef	*params[],
             PF_LayerDef	*output )
{
	out_data->my_version = PF_VERSION(MAJOR_VERSION,
                                      MINOR_VERSION,
                                      BUG_VERSION,
                                      STAGE_VERSION,
                                      BUILD_VERSION);
    
	out_data->out_flags = PF_OutFlag_NON_PARAM_VARY;
    
	PF_Err err = PF_Err_NONE;
    
    return err;
}


static PF_Err
GlobalSetdown (
               PF_InData		*in_data,
               PF_OutData		*out_data,
               PF_ParamDef		*params[],
               PF_LayerDef		*output )
{
	PF_Err  err = PF_Err_NONE;
    
	return err;
}


static PF_Err
Render (
        PF_InData		*in_data,
        PF_OutData		*out_data,
        PF_ParamDef		*params[],
        PF_LayerDef		*output )
{
	PF_Err				err		= PF_Err_NONE;
	AEGP_SuiteHandler	suites(in_data->pica_basicP);
    
    PF_EffectWorld	cairoWorld;
	A_long			layerWidthL     =	output->width,
                    layerHeightL	=	output->height;
    
    A_long motionStepL = in_data->current_time / in_data->time_step;
	A_long linesL      = 0;
    
    A_short downSampleX = static_cast<A_short>(in_data->downsample_x.den);
    A_short downSampleY = static_cast<A_short>(in_data->downsample_y.den);
    
	linesL = output->extent_hint.bottom - output->extent_hint.top;
    
    WorldParams::instance()->setParams(params);
    WorldParams::instance()->setCurrentFrame(motionStepL);
    WorldParams::instance()->setDownSamples(downSampleX, downSampleY);
    WorldParams::instance()->setLayerSize(layerWidthL, layerHeightL);
    
    //    std::cout << "Render";
    //    std::cout << "Current time: " << in_data->current_time << " \n";
    //    std::cout << "Time step: " << in_data->time_step << " \n";
    //    std::cout << "Local time step: " << in_data->local_time_step << " \n";
    //    std::cout << "Total time: " << in_data->total_time << " \n";
    //    std::cout << "Time scale: " << in_data->time_scale << " \n";
    //      std::cout << WorldParams::instance()->getEmitterX() << " ";
    
    err = PF_PROGRESS(in_data, 0, 100);

    AEFX_CLR_STRUCT(cairoWorld);
    ERR(suites.WorldSuite1()->new_world(in_data->effect_ref,
                                        layerWidthL,
                                        layerHeightL,
                                        PF_NewWorldFlag_CLEAR_PIXELS,
                                        &cairoWorld));
    
    PF_Handle bufferH = NULL;
    bufferH = suites.HandleSuite1()->host_new_handle(((layerWidthL * layerHeightL) * sizeof(PF_Pixel8)));
    if (bufferH)
    {
        
        PF_Handle	pixelInsert   =	suites.HandleSuite1()->host_new_handle(sizeof(PF_Pixel8));
        PF_Pixel8	*pixelInsertP = reinterpret_cast<PF_Pixel8*>(suites.HandleSuite1()->host_lock_handle(pixelInsert));
        
        cairo_surface_t *surface;
        cairo_t *cairoContext;
        
        surface = cairo_image_surface_create (CAIRO_FORMAT_A8, layerWidthL, layerHeightL);
        cairoContext = cairo_create (surface);
        
        /* Examples are in 1.0 x 1.0 coordinate space */
        // cairo_scale (cairoContext, layerWidthL, layerHeightL);
        // cairo_scale(cairoContext, 0, 0);
        /* Drawing code goes here */
        
        pworld.reField();
        pworld.clearParticles();

        //pworld.makeEmitter();

        pworld.buildWorld();
        pworld.drawWorld(cairoContext, in_data);
        
        unsigned char *data = cairo_image_surface_get_data(surface);
        int stride = cairo_image_surface_get_stride(surface);
        
        for (int y = 0; y < cairoWorld.height; y+=downSampleY)        // row. Cairo AE World and Cairo surface must have the same size!
        {
            uint8_t *rowP;
            rowP = (uint8_t *)(data + y * stride);          // row y
            
            for (int x = 0; x < cairoWorld.width; x+=downSampleX)     // column
            {
                
                A_u_char pixel      = rowP[x];
                //                    A_u_char pixel_     = rowP[x];
                //                    A_u_char pixel__    = rowP[x];
                
                //A_u_char red = (pixel >> 4) & 0xFF;
                
                pixelInsertP->alpha     = 255;
                pixelInsertP->red       = pixel;
                pixelInsertP->green     = pixel;
                pixelInsertP->blue      = pixel;
                
                PF_Pixel8 *pixelDataStart = NULL;
                
                PF_GET_PIXEL_DATA8(&cairoWorld , NULL, &pixelDataStart);
                ::memcpy(
                         pixelDataStart + (x + (y * cairoWorld.rowbytes/sizeof(PF_Pixel8))),
                         pixelInsertP,
                         sizeof(PF_Pixel8)
                         );
            }
        }
        
        cairo_destroy (cairoContext);
        cairo_surface_destroy (surface);
        
        //clean the data after being copied
        suites.HandleSuite1()->host_unlock_handle(bufferH);
        suites.HandleSuite1()->host_dispose_handle(bufferH);
        
        suites.HandleSuite1()->host_unlock_handle(pixelInsert);
        suites.HandleSuite1()->host_dispose_handle(pixelInsert);
    }
    else
    {
        CHECK(PF_Err_OUT_OF_MEMORY);
    }
    
    if (PF_Quality_HI == in_data->quality) {
        ERR(suites.WorldTransformSuite1()->copy_hq(in_data->effect_ref,
                                                   &cairoWorld,
                                                   output,
                                                   NULL,
                                                   NULL));
        
    }
    else
    {
        ERR(suites.WorldTransformSuite1()->copy(in_data->effect_ref,
                                                &cairoWorld,
                                                output,
                                                NULL,
                                                NULL));
    }
    
    err = PF_PROGRESS(in_data, 100, 100);
    
    ERR(PF_ABORT(in_data));

    
	return err;
}


DllExport
PF_Err
EntryPointFunc (
                PF_Cmd			cmd,
                PF_InData		*in_data,
                PF_OutData		*out_data,
                PF_ParamDef		*params[],
                PF_LayerDef		*output,
                void			*extra)
{
	PF_Err		err = PF_Err_NONE;
    
    

	
	try {
		switch (cmd) {
			case PF_Cmd_ABOUT:
                
				err = About(in_data,
							out_data,
							params,
							output);
				break;
				
			case PF_Cmd_GLOBAL_SETUP:
                
				err = GlobalSetup(in_data,
                                  out_data,
                                  params,
                                  output);
				break;
				
			case PF_Cmd_PARAMS_SETUP:
                
				err = ParamsSetup(in_data,
                                  out_data,
                                  params,
                                  output);
				break;
				
			case PF_Cmd_RENDER:
                
				err = Render(in_data,
                             out_data,
                             params,
                             output);
				break;
                
            case PF_Cmd_GLOBAL_SETDOWN:
				err = GlobalSetdown(in_data,
                                    out_data,
                                    params,
                                    output);
				break;
		}
	}
	catch(PF_Err &thrown_err){
		err = thrown_err;
	}
	return err;
}





