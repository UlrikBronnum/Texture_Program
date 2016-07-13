#pragma once

#include "ofMain.h"

enum LuminanceType {
	BRIGHTNESS = 0,
	LIGHTNESS,
	DARKNESS
};

enum ColorComponent {
	GREY = 0,
	RED = GREY,
	GREEN = 1,
	BLUE = 2
};

enum HsComponent {
	HUE = 0,
	SATURATION
};

enum WrapMode {
	CLAMP = 0,
	WRAP
};

class Kernel;

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class ImageProcessing
{
public:
	ImageProcessing();
	~ImageProcessing();

	static void rgbFromGreys		(unsigned char* redChannel, unsigned char* greenChannel, unsigned char* blueChannel, unsigned char* output, unsigned width, unsigned height);
	static void rgbaFromGreys		(unsigned char* redChannel, unsigned char* greenChannel, unsigned char* blueChannel, unsigned char* alphaChannel, unsigned char* output, unsigned width, unsigned height);
	static void rgbaFromRgbGrey		(unsigned char* rgb, unsigned char* alphaChannel, unsigned char* output, unsigned width, unsigned height);
	static void rgbFromRgba			(unsigned char* rgba, unsigned char* rgbOutput, unsigned width, unsigned height);

protected:

	static int getMin(int value, int control);
	static int getMax(int value, int control);
	static int getSum(int value, int control);

	static int clamp	(int low, int high, int value, int modifier);
	static float clamp	(float low, float high, float value, float modifier);
	static int wrap		(int low, int high, int value, int modifier);
	static float wrap	(float low, float high, float value, float modifier);


	static bool setGroups(unsigned char* input, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k);
	static short calculateDifference(float* kmeans, float pixel, int k);
	static void setNewMeans(unsigned char* newValues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k);

	static bool setGroupsHue(short* newHues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k);
	static short calculateDifferenceDegrees(float* kmeans, float pixel, int k);
	static void setNewMeans(short*  newValues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k);
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GreyProcessing : public ImageProcessing
{
public:
	GreyProcessing();
	~GreyProcessing();

	// input is rgb, output is greyscale
	static void greyFromChannel	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, ColorComponent index);
	static void greyFromLuminance(unsigned char* input, unsigned char* output, unsigned width, unsigned height, LuminanceType type);
	static void greyFromHS		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, HsComponent component);

	// input is greyscale, output is greyscale
	static void greyInvert		(unsigned char* input, unsigned char* output, unsigned width, unsigned height);
	static void greyBrightness	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value);
	static void greyContrast	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value);
	static void greyGamma		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, float value);
	static void greyStretch		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int newLow, int newHigh);
	static void greyThreshold	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int threshold, bool greater, bool makeBinary = true);

	static void greyBlur		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, Kernel& kernel, WrapMode mode);
	static void greyEdge		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, const int kernel[], int size, WrapMode mode, bool makeBinary = false, int threshold = 0);

	static void greyKMeans		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int k, float* kMeans, int maxIterations = 10);

	static void greyDiamondSquare(unsigned char* pixels, int baseSize, int size, float roughness);

	static int  greyGrassFire	(unsigned char * input, int width, int height, int newMark = 1);

protected:

	static void diamond	(unsigned char* pixels, int baseSize, int size, int half, float scale);
	static void square	(unsigned char* pixels, int baseSize, int size, int half, float scale);

	static void grassfire(unsigned char* input, int width, int height, int ypos, int xpos, int marker, int session = 0);
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class ColorProcessing : public ImageProcessing
{
public:
	ColorProcessing();
	~ColorProcessing();

	static void colorInvert		(unsigned char* input, unsigned char* output, unsigned width, unsigned height);
	static void colorInvertComponent(unsigned char* input, unsigned char* output, unsigned width, unsigned height, ColorComponent component);
	static void colorBrightness	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value);
	static void colorContrast	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value);
	static void colorGamma		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, float value);
	static void colorNormalize	(unsigned char* input, unsigned char* output, unsigned width, unsigned height);
	static void colorStretch	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int newLow, int newHigh);

	static void colorThreshold	(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int threshold, bool greater, bool makeBinary = true);
	
	static void colorBlur		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, Kernel& kernel, WrapMode mode);
	static void colorEdge		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, const int kernel[], int diameter, WrapMode mode, bool makeBinary = false, int threshold = 0);

	static void colorKMeans		(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int k, float* means, int maxIterations = 10);
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class GPUProcessing
{
public:
	GPUProcessing();
	~GPUProcessing();

	static void invert		(ofTexture& tex, ofFbo& out_fbo, ofVec3f channels = ofVec3f(1.0));
	static void blur		(ofTexture& tex, ofFbo& out_fbo, Kernel& kernel, ofVec3f channel = ofVec3f(1, 1, 1));
	static void edge		(ofTexture& tex, ofFbo& out_fbo, const int  hKernel[], const int  vKernel[], ofVec3f greyConvertion = ofVec3f(0.33, 0.33, 0.33));
	static void normalmap	(ofTexture& tex, ofFbo& out_fbo, const int  hKernel[], const int  vKernel[], float scale, ofVec3f greyConvertion = ofVec3f(0.33, 0.33, 0.33));

};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class Kernel
{
public:
	Kernel(int kernelDiameter = 3);
	~Kernel();
	void gaussianKernel(float sigma);
	void boxKernel();
	void crossKernel();
	void discKernel();

	float* getKernel();
	int	   getDiameter();

private:

	float*	kernel;
	int		diameter;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------

class Histogram {
public:
	Histogram();
	~Histogram();

	void update(unsigned char* data, unsigned int width, unsigned int height, ColorComponent channel = GREY);

	float getMean();
	int getMax();
	int getMin();

private:

	void reset();

	unsigned int*   bins;
	unsigned int    levels;
	unsigned int	channels;
};

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------------------------------------------
static const int prewitt_vertical[9] =
{
	 -1 , 0 , 1 ,
	 -1 , 0 , 1 ,
	 -1 , 0 , 1 
};

static const int prewitt_horizontal[9] =
{
	 1 , 1 , 1 ,
	 0 , 0 , 0 ,
	-1 ,-1 ,-1
};

static const int prewitt_diagonal[9] =
{
	 0 , 1 , 1 ,
    -1 , 0 , 1 ,
	-1 ,-1 , 0  
};
///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int sobel_vertical[9] =
{
    -1 , 0 , 1 ,
	-2 , 0 , 2 ,
	-1 , 0 , 1  
};

static const int sobel_horizontal[9] =
{
	 1 , 2 , 1 ,
	 0 , 0 , 0 ,
	-1 ,-2 ,-1  
};

static const int sobel_diagonal[9] =
{
	 0 , 1 , 2 ,
	-1 , 0 , 1 ,
	-2 ,-1 , 0  
};
///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int robinson_vertical[9] =
{
	 -1 , 1 , 1 ,
	 -1 ,-2 , 1 ,
	 -1 , 1 , 1  
};

static const int robinson_horizontal[9] =
{
	 1 , 1 , 1 ,
	 1 ,-2 , 1 ,
    -1 ,-1 ,-1  
};

static const int robinson_diagonal[9] =
{
	 1 , 1 , 1 ,
	-1 ,-2 , 1 ,
	-1 ,-1 , 1  
};

///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int kirsch_vertical[9] =
{
    -5 , 3 , 3 ,
	-5 , 0 , 3 ,
	-5 , 3 , 3  
};

static const int kirsch_horizontal[9] =
{
	 3 , 3 , 3 ,
	 3 , 0 , 3 ,
	-5 ,-5 ,-5  
};

static const int kirsch_diagonal[9] =
{
	 3 , 3 , 3 ,
	-5 , 0 , 3 ,
	-5 ,-5 , 3  
};

///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int scharr_vertical[9] =
{
	-3 , 0 , 3 ,
   -10 , 0 , 10 ,
    -3 , 0 , 3  
};

static const int scharr_horizontal[9] =
{
	 3 , 10, 3 ,
	 0 , 0 , 0 ,
    -3 ,-10,-3  
};

///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int normal_vertical[9] =
{
	 0 , 0 , 0 ,
	-1 , 0 , 1 ,
	 0 , 0 , 0  
};

static const int normal_horizontal[9] =
{
	 0 , 1 , 0 ,
	 0 , 0 , 0 ,
	 0 ,-1 , 0 
};

///----------------------------------------------
///----------------------------------------------
///----------------------------------------------

static const int laplacian2D[9] =
{
	 0 ,-1 , 0 ,
	-1 , 4 ,-1 ,
	 0 ,-1 , 0  
};

static const int laplacian3D[9] =
{
	-1 ,-1 ,-1 ,
	-1 , 8 ,-1 ,
	-1 ,-1 ,-1  
};

///----------------------------------------------
///----------------------------------------------
///----------------------------------------------
static const int emboss[9] =
{
	-2 ,-1 , 0 ,
	-1 , 2 , 1 ,
	 0 , 1 , 2  
};


