#include "ImageProcessing.h"

//------------------------------------------------------------------------
//-----------------------ImageProcessing----------------------------------
//------------------------------------------------------------------------


ImageProcessing::ImageProcessing()
{
}


ImageProcessing::~ImageProcessing()
{
}

void ImageProcessing::rgbFromGreys(	unsigned char* redChannel, unsigned char* greenChannel, unsigned char* blueChannel,
									unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output[(y*width + x) * 3 + 0] = redChannel[y*width + x];
			output[(y*width + x) * 3 + 1] = greenChannel[y*width + x];
			output[(y*width + x) * 3 + 2] = blueChannel[y*width + x];
		}
	}
}

void ImageProcessing::rgbaFromGreys(unsigned char* redChannel, unsigned char* greenChannel,
									unsigned char* blueChannel, unsigned char* alphaChannel,
									unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output[(y*width + x) * 4 + 0] = redChannel[y*width + x];
			output[(y*width + x) * 4 + 1] = greenChannel[y*width + x];
			output[(y*width + x) * 4 + 2] = blueChannel[y*width + x];
			output[(y*width + x) * 4 + 3] = alphaChannel[y*width + x];
		}
	}
}
void ImageProcessing::rgbaFromRgbGrey(unsigned char* rgb, unsigned char* alphaChannel,
									unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output[(y*width + x) * 4 + 0] = rgb[(y * width + x) * 3 + 0];
			output[(y*width + x) * 4 + 1] = rgb[(y * width + x) * 3 + 1];
			output[(y*width + x) * 4 + 2] = rgb[(y * width + x) * 3 + 2];
			output[(y*width + x) * 4 + 3] = alphaChannel[y*width + x];
		}
	}
}
void ImageProcessing::rgbFromRgba(unsigned char* rgba, unsigned char* rgbOutput, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			rgbOutput[(y*width + x) * 3 + 0] = rgba[(y * width + x) * 4 + 0];
			rgbOutput[(y*width + x) * 3 + 1] = rgba[(y * width + x) * 4 + 1];
			rgbOutput[(y*width + x) * 3 + 2] = rgba[(y * width + x) * 4 + 2];
		}
	}
}

int ImageProcessing::getMin(int value, int control) {
	if (value < control) {
		return value;
	}
	else {
		return control;
	}
}

int ImageProcessing::getMax(int value, int control) {
	if (value > control) {
		return value;
	}
	else {
		return control;
	}
}

int ImageProcessing::getSum(int value, int control) {
	return value + control;
}
///
///                 WrapModes
///
int ImageProcessing::clamp(int low, int high, int value, int modifier)
{
	int out = value + modifier;
	if (out > high) {
		out = high;
	}
	else if (out < low) {
		out = low;
	}
	return out;
}

float ImageProcessing::clamp(float low, float high, float value, float modifier)
{
	float out = value + modifier;
	if (out > high) {
		out = high;
	}
	else if (out < low) {
		out = low;
	}
	return out;
}

int ImageProcessing::wrap(int low, int high, int value, int modifier)
{
	int out = value + modifier;
	if (out > high) {
		out = low + modifier - 1;
	}
	else if (out < low) {
		out = high - abs(modifier) + 1;
	}
	return out;
}

float ImageProcessing::wrap(float low, float high, float value, float modifier)
{
	float out = value + modifier;
	if (out > high) {
		out = low + modifier - 1;
	}
	else if (out < low) {
		out = high - abs(modifier) + 1;
	}
	return out;
}

bool ImageProcessing::setGroups(unsigned char* input, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k)
{
	bool anyChanges = false;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			short group = calculateDifference(kmeans, input[y * width + x], k);
			if (groupContainer[y * width + x] != group)
			{
				groupContainer[y * width + x] = group;
				anyChanges = true;
			}
		}
	}
	return anyChanges;
}
short ImageProcessing::calculateDifference(float* kmeans, float pixel, int k)
{

	float* diff = new float[k];
	for (int i = 0; i < k; i++)
	{
		diff[i] = sqrt(pow(float(kmeans[i]) - float(pixel), 2));
	}

	short group = 0;
	float minDist = diff[0];
	for (int i = 1; i < k; i++)
	{
		if (minDist > diff[i]) {
			minDist = diff[i];
			group = i;
		}
	}
	delete[] diff;
	return group;
}
void ImageProcessing::setNewMeans(unsigned char* newValues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k)
{
	int* nMeans = new int[k];
	int* nCount = new int[k];
	for (int i = 0; i < k; i++)
	{
		nMeans[i] = 0;
		nCount[i] = 0;
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			nMeans[groupContainer[y * width + x]] += newValues[y * width + x];
			nCount[groupContainer[y * width + x]] ++;
		}
	}
	for (int i = 0; i < k; i++)
	{
		if (nCount[i] == 0)
			kmeans[i] = 0;
		else
			kmeans[i] = float(nMeans[i]) / float(nCount[i]);
	}

	delete[] nMeans;
	delete[] nCount;
}


bool ImageProcessing::setGroupsHue(short* newHues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k)
{
	bool anyChanges = false;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			short group = calculateDifferenceDegrees(kmeans, newHues[y * width + x], k);
			if (groupContainer[y * width + x] != group)
			{
				groupContainer[y * width + x] = group;
				anyChanges = true;
			}
		}
	}
	return anyChanges;
}
short ImageProcessing::calculateDifferenceDegrees(float* kmeans, float pixel, int k)
{
	float* diff = new float[k];

	for (int i = 0; i < k; i++)
	{
		diff[i] = abs(ofAngleDifferenceDegrees(kmeans[i], pixel));
	}

	short group = 0;
	float minDist = diff[0];
	for (int i = 1; i < k; i++)
	{
		if (minDist > diff[i]) {
			minDist = diff[i];
			group = i;
		}
	}
	delete[] diff;
	return group;
}
void ImageProcessing::setNewMeans(short* newValues, short* groupContainer, unsigned width, unsigned height, float* kmeans, int k)
{
	int* nMeans = new int[k];
	int* nCount = new int[k];

	for (int i = 0; i < k; i++)
	{
		nMeans[i] = 0;
		nCount[i] = 0;
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			nMeans[groupContainer[y * width + x]] += newValues[y * width + x];
			nCount[groupContainer[y * width + x]] ++;
		}
	}
	for (int i = 0; i < k; i++)
	{
		kmeans[i] = float(nMeans[i]) / nCount[i];
	}
	delete[] nMeans;
	delete[] nCount;
}
//------------------------------------------------------------------------
//------------------------GreyProcessing----------------------------------
//------------------------------------------------------------------------

GreyProcessing::GreyProcessing()
{
}


GreyProcessing::~GreyProcessing()
{
}

void GreyProcessing::greyFromLuminance(unsigned char* input, unsigned char* output, unsigned width, unsigned height, LuminanceType type)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			int value = (type == DARKNESS) ? 255 : 0;
			for (int c = 0; c < 3; c++)
			{
				unsigned char channel = input[(y * width + x)* 3 + c];
				switch (type) {
				case BRIGHTNESS:
					value = getMax(channel, value);
					break;
				case LIGHTNESS:
					value = getSum(channel, value);
					break;
				case DARKNESS:
					value = getMin(channel, value);
					break;
				default:
					break;
				}
			}
			if (type == LIGHTNESS) {
				output[y * width + x] = value / (int)3;
			}
			else {
				output[y * width + x] = value;
			}
		}
	}
}

void GreyProcessing::greyFromHS(unsigned char* input, unsigned char* output, unsigned width, unsigned height, HsComponent component)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			unsigned char r, g, b;
			r = input[(y * width + x)* 3 + 0];
			g = input[(y * width + x)* 3 + 1];
			b = input[(y * width + x)* 3 + 2];
			ofColor color;
			color.set(r, g, b);
			if (component == HUE) {
				output[y * width + x] = (unsigned char)((color.getHueAngle() / 360) * 255);
			}
			else if (component == SATURATION) {
				output[y * width + x] = (unsigned char)(color.getSaturation() < 0.05f) ? 0 : color.getSaturation();
			}
		}
	}
}

void GreyProcessing::greyFromChannel(unsigned char* input, unsigned char* output, unsigned width, unsigned height, ColorComponent index)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			output[y * width + x] = input[(y * width + x)* 3 + index];
		}
	}
}

///
///                 Point Processing
///

void GreyProcessing::greyInvert(unsigned char* input, unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			output[y * width + x] = 255 - input[y * width + x];
		}
	}
}
void GreyProcessing::greyBrightness(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			output[y*width + x] = clamp(0, 255, input[y*width + x], value);
		}
	}
}
void GreyProcessing::greyContrast(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value)
{
	float factor = (259.0f * ((float)value + 255.0f)) / (255.0f * (259.0f - (float)value));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			output[y*width + x] = (unsigned char)clamp(0.0f, 255.0f, factor * float(input[y*width + x] - 128.0f) + 128.0f, 0.0f);
		}
	}
}
void GreyProcessing::greyGamma(unsigned char* input, unsigned char* output, unsigned width, unsigned height, float value)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			output[y*width + x] = (unsigned char)clamp(0.0f, 255.0f, pow(255.0f * ((float)input[y*width + x] / 255.0f), value), 0.0f);
		}
	}
}
void GreyProcessing::greyStretch(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int newLow, int newHigh)
{
	Histogram hist;
	hist.update(input, width, height, GREY);

	int min = int(hist.getMin());
	int max = int(hist.getMax());

	int fullLength = max - min;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			float   diff = 1.0f - (float(fullLength) - ((float)input[y*width + x] - min)) / (float)fullLength;
			output[y * width + x] = int(float(newHigh - newLow) * diff + 0.5f) + newLow;
		}
	}
}
///
///                 Binary
///
void GreyProcessing::greyThreshold(	unsigned char* input, unsigned char* output, unsigned width, unsigned height, 
									int threshold, bool greater, bool makeBinary)
{
	if (greater) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
			{
				if (input[y*width + x] > threshold) {
					if (makeBinary) {
						output[y*width + x] = 255;
					}
					else {
						output[y*width + x] = input[y*width + x];
					}
				}
				else {
					output[y*width + x] = 0;
				}
			}
		}
	}
	else 
	{
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
			{
				if (input[y*width + x] < threshold) {
					if (makeBinary) {
						output[y*width + x] = 255;
					}
					else {
						output[y*width + x] = input[y*width + x];
					}
				}
				else {
					output[y*width + x] = 0;
				}
			}
		}
	}
}
///
///                 Blur
///

void GreyProcessing::greyBlur(	unsigned char* input, unsigned char* output, unsigned width, unsigned height, 
								Kernel& kernel, WrapMode mode)
{
	int size = kernel.getDiameter();
	int n = (size - 1) / 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			float sum = 0;
			for (int fy = -n; fy <= n; fy++)
			{
				for (int fx = -n; fx <= n; fx++)
				{
					int pos_x = 0;
					int pos_y = 0;

					if (mode == CLAMP) {
						pos_x = clamp(0, width - 1, x, fx);
						pos_y = clamp(0, height - 1, y, fy);
					}
					else if (mode == WRAP) {
						pos_x = wrap(0, width - 1, x, fx);
						pos_y = wrap(0, height - 1, y, fy);
					}
					sum += (float)input[(pos_y)*width + (pos_x)] * kernel.getKernel()[(fy + n) * size + (fx + n)];
				}
			}
			output[y*width + x] = (unsigned char)sum;
		}
	}
}
///
///                 Edge
///
void GreyProcessing::greyEdge(	unsigned char* input, unsigned char* output,
								unsigned width, unsigned height,
								const int kernel[], int size,
								WrapMode mode, bool makeBinary, int threshold)
{
	int n = (size - 1) / 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			float sum = 0;
			for (int fy = -n; fy <= n; fy++)
			{
				for (int fx = -n; fx <= n; fx++)
				{
					int pos_x = 0;
					int pos_y = 0;

					if (mode == CLAMP) {
						pos_x = clamp(0, width - 1, x, fx);
						pos_y = clamp(0, height - 1, y, fy);
					}
					else if (mode == WRAP) {
						pos_x = wrap(0, width - 1, x, fx);
						pos_y = wrap(0, height - 1, y, fy);
					}
					sum += (float)input[(pos_y)*width + (pos_x)] * kernel[(fy + n) * size + (fx + n)];
				}
			}

			if (makeBinary) {
				if (abs(sum) > threshold) {
					output[y*width + x] = 255;
				}
				else {
					output[y*width + x] = 0;
				}
			}
			else {
				output[y*width + x] = (unsigned char)(128 + clamp(-127, 127, (int)sum, 0));
			}
		}
	}
}
///
///                 k-means
///
void GreyProcessing::greyKMeans(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int k, float* means, int maxIterations)
{
	short*   groups = new short[width * height];
	int    max = 0;
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
		{
			if (max < input[y * width + x])
				max = input[y * width + x];
		}
	}

	float  subsec = float(max) / float(k);
	for (int i = 0; i < k; i++)
	{
		means[i] = subsec * (i + 1);
	}
	bool pixelChangeGroup = true;
	int  iteration = 0;

	setGroups(input, groups, width, height, means, k);

	while (iteration < maxIterations)
	{
		setNewMeans(input, groups, width, height, means, k);
		pixelChangeGroup = setGroups(input, groups, width, height, means, k);
		iteration++;
		if (!pixelChangeGroup)
			iteration = maxIterations;
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			output[y * width + x] = means[groups[y * width + x]];
		}
	}
	delete[] groups;
}
///
///                 Noise
///
void GreyProcessing::greyDiamondSquare(unsigned char* pixels, int baseSize, int size, float roughness) {

	int half = size / 2;
	float scale = roughness * size;

	if (half < 1) return;

	square(pixels, baseSize, size, half, scale);
	diamond(pixels, baseSize, size, half, scale);

	greyDiamondSquare(pixels, baseSize, size / 2, roughness);
}
void GreyProcessing::square(unsigned char* pixels, int baseSize, int size, int half, float scale) {
	for (int y = half; y < baseSize; y += size) {
		for (int x = half; x < baseSize; x += size) {

			int c1, c2, c3, c4;
			int left_x, right_x, top_y, but_y;

			left_x = (x - half);
			top_y = (y - half);
			right_x = (x + half);
			but_y = (y + half);

			if (left_x < 0) { left_x = baseSize + left_x; }
			if (top_y < 0) { top_y = baseSize + top_y; }
			if (right_x > baseSize) { right_x = right_x - baseSize; }
			if (but_y > baseSize) { but_y = but_y - baseSize; }

			c1 = pixels[top_y * baseSize + left_x];
			c2 = pixels[top_y * baseSize + right_x];
			c3 = pixels[but_y * baseSize + left_x];
			c4 = pixels[but_y * baseSize + right_x];

			int sum = ((c1 + c2 + c3 + c4) / 4) + ofRandom(0, 1) * scale * 2 - scale;

			if (sum > 255) {
				sum = 255;
			}
			else if (sum < 0) {
				sum = 0;
			}

			pixels[y * baseSize + x] = sum;
		}
	}
}
void GreyProcessing::diamond(unsigned char* pixels, int baseSize, int size, int half, float scale) {

	for (int y = 0; y < baseSize; y += half) {
		for (int x = (y + half) % size; x < baseSize; x += size) {

			int c1, c2, c3, c4;
			int left_x, right_x, top_y, but_y;

			left_x = (x - half);
			top_y = (y - half);
			right_x = (x + half);
			but_y = (y + half);

			if (left_x < 0) { left_x = baseSize + left_x; }
			if (top_y < 0) { top_y = baseSize + top_y; }
			if (right_x > baseSize) { right_x = right_x - baseSize; }
			if (but_y > baseSize) { but_y = but_y - baseSize; }

			c1 = pixels[top_y * baseSize + x];
			c2 = pixels[but_y * baseSize + x];
			c3 = pixels[y * baseSize + left_x];
			c4 = pixels[y * baseSize + right_x];

			int sum = ((c1 + c2 + c3 + c4) / 4) + ofRandom(0, 1) * scale * 2 - scale;

			if (sum > 255) {
				sum = 255;
			}
			else if (sum < 0) {
				sum = 0;
			}

			pixels[y * baseSize + x] = sum;
		}
	}
}
///
///                 Object detection
///
int GreyProcessing::greyGrassFire(unsigned char * input, int width, int height, int newMark) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (input[y*width + x] == 255 && newMark < 255) {
				newMark++;
				grassfire(input, width, height, y, x, newMark, 0);
			}
		}
	}
	return newMark;
}

void GreyProcessing::grassfire(unsigned char* input, int width, int height, int ypos, int xpos, int marker, int session) {

	input[ypos*width + xpos] = marker;
	if (xpos + 1 < width && input[ypos*width + (xpos + 1)] == 255 && session < 20000) {
		session++;
		grassfire(input, width, height, ypos, xpos + 1, marker, session);
	}
	if (ypos + 1 < height && input[(ypos + 1)*width + xpos] == 255 && session < 20000) {
		session++;
		grassfire(input, width, height, ypos + 1, xpos, marker, session);
	}
	if (xpos - 1 >= 0 && input[ypos*width + (xpos - 1)] == 255 && session < 20000) {
		session++;
		grassfire(input, width, height, ypos, xpos - 1, marker, session);
	}
	if (ypos - 1 >= 0 && input[(ypos - 1)*width + xpos] == 255 && session < 20000) {
		session++;
		grassfire(input, width, height, ypos - 1, xpos, marker, session);
	}
}
//------------------------------------------------------------------------
//-----------------------ColorProcessing----------------------------------
//------------------------------------------------------------------------

ColorProcessing::ColorProcessing()
{
}


ColorProcessing::~ColorProcessing()
{
}
void ColorProcessing::colorInvert(unsigned char* input, unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char r, g, b;
			r = input[(y * width + x)* 3 + 0];
			g = input[(y * width + x)* 3 + 1];
			b = input[(y * width + x)* 3 + 2];

			output[(y * width + x)* 3 + 0] = 255 - r;
			output[(y * width + x)* 3 + 1] = 255 - g;
			output[(y * width + x)* 3 + 2] = 255 - b;
		}
	}
}
void ColorProcessing::colorInvertComponent(unsigned char* input, unsigned char* output, unsigned width, unsigned height, ColorComponent component)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			output[(y * width + x) * 3 + component] = 255 - output[(y * width + x) * 3 + component];
		}
	}
}
void ColorProcessing::colorBrightness(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++)
			{
				output[(y*width + x)*3 + c] = clamp(0, 255, input[(y*width + x)*3 + c], value);
			}
		}
	}
}
void ColorProcessing::colorContrast(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int value)
{
	float factor = (259.0f * ((float)value + 255.0f)) / (255.0f * (259.0f - (float)value));
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++)
			{
				output[(y*width + x)*3 + c] = (unsigned char)clamp(0.0f, 255.0f, factor * float(input[(y*width + x)*3 + c] - 128.0f) + 128.0f, 0.0f);
			}
		}
	}
}
void ColorProcessing::colorGamma(unsigned char* input, unsigned char* output, unsigned width, unsigned height, float value)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++)
			{
				output[(y*width + x)*3 + c] = (unsigned char)clamp(0.0f, 255.0f, pow(255.0f * ((float)input[(y*width + x)*3 + c] / 255.0f), value), 0.0f);
			}
		}
	}
}
void ColorProcessing::colorNormalize(unsigned char* input, unsigned char* output, unsigned width, unsigned height)
{
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char r, g, b;
			r = input[(y * width + x) * 3 + 0];
			g = input[(y * width + x) * 3 + 1];
			b = input[(y * width + x) * 3 + 2];

			float sum = float(r + g + b);

			if (sum == 0) {
				output[(y * width + x) * 3 + 0] = 0;
				output[(y * width + x) * 3 + 1] = 0;
				output[(y * width + x) * 3 + 2] = 0;
			}
			else {
				output[(y * width + x) * 3 + 0] = (float)r / sum * 255;
				output[(y * width + x) * 3 + 1] = (float)g / sum * 255;
				output[(y * width + x) * 3 + 2] = (float)b / sum * 255;
			}
		}
	}
}
void ColorProcessing::colorStretch(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int newLow, int newHigh)
{
	for (int c = 0; c < 3; c++) {
		Histogram hist;

		ColorComponent color = RED;
		if (c == 1) {
			color = GREEN;
		}else if (c == 2) {
			color = BLUE;
		}
		hist.update(input, width, height, color);

		int min = int(hist.getMin());
		int max = int(hist.getMax());

		int fullLength = max - min;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++)
			{
				float   diff = 1.0f - (float(fullLength) - ((float)input[(y*width + x) * 3 + c] - min)) / (float)fullLength;
				output[(y*width + x) * 3 + c] = int(float(newHigh - newLow) * diff + 0.5f) + newLow;
			}
		}
	}
}
///
///                 Binary
///
void ColorProcessing::colorThreshold(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int threshold, bool greater, bool makeBinary)
{
	
	if (greater) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				for (int c = 0; c < 3; c++)
				{
					if (input[(y*width + x)*3 + c] > threshold) {
						if (makeBinary) {
							output[(y*width + x)*3 + c] = 255;
						}
						else {
							output[(y*width + x)*3 + c] = input[(y*width + x)*3 + c];
						}
					}
					else {
						output[(y*width + x)*3 + c] = 0;
					}
				}
			}
		}
	}
	else{
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				for (int c = 0; c < 3; c++)
				{
					if (input[(y*width + x)*3 + c] < threshold) {
						if (makeBinary) {
							output[(y*width + x)*3 + c] = 255;
						}
						else {
							output[(y*width + x)*3 + c] = input[(y*width + x)*3 + c];
						}
					}
					else {
						output[(y*width + x)*3 + c] = 0;
					}
				}
			}
		}
	}
}

///
///                 Blur
///

void ColorProcessing::colorBlur(unsigned char* input, unsigned char* output, unsigned width, unsigned height, 
								Kernel& kernel, WrapMode mode)
{
	int size = kernel.getDiameter();
	int n = (size - 1) / 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++)
			{
				float sum = 0;
				for (int fy = -n; fy <= n; fy++)
				{
					for (int fx = -n; fx <= n; fx++)
					{
						int pos_x = 0;
						int pos_y = 0;

						if (mode == CLAMP) {
							pos_x = clamp(0, width - 1, x, fx);
							pos_y = clamp(0, height - 1, y, fy);
						}
						else if (mode == WRAP) {
							pos_x = wrap(0, width - 1, x, fx);
							pos_y = wrap(0, height - 1, y, fy);
						}

						sum += (float)input[((pos_y)*width + (pos_x)) * 3 + c] * kernel.getKernel()[(fy + n) * size + (fx + n)];
					}
				}
				output[(y*width + x) * 3 + c] = (unsigned char)sum;
			}
		}
	}
}
///
///                 Edge
///
void ColorProcessing::colorEdge(unsigned char* input, unsigned char* output,
								unsigned width, unsigned height,
								const int kernel[], int diameter,
								WrapMode mode, bool makeBinary, int threshold)
{
	int n = (diameter - 1) / 2;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			for (int c = 0; c < 3; c++)
			{
				float sum = 0;
				for (int fy = -n; fy <= n; fy++)
				{
					for (int fx = -n; fx <= n; fx++)
					{
						int pos_x = 0;
						int pos_y = 0;

						if (mode == CLAMP) {
							pos_x = clamp(0, width - 1, x, fx);
							pos_y = clamp(0, height - 1, y, fy);
						}
						else if (mode == WRAP) {
							pos_x = wrap(0, width - 1, x, fx);
							pos_y = wrap(0, height - 1, y, fy);
						}
						sum += (float)input[((pos_y)*width + (pos_x)) * 3 + c] * kernel[(fy + n) * diameter + (fx + n)];
					}
				}

				if (makeBinary) {
					if (abs(sum) > threshold) {
						output[(y*width + x) * 3 + c] = 255;
					}
					else {
						output[(y*width + x) * 3 + c] = 0;
					}
				}
				else {
					output[(y*width + x) * 3 + c] = (unsigned char)(128 + clamp(-127, 127, (int)sum, 0));
				}
			}
		}
	}
}
///
///                 k-means
///
void ColorProcessing::colorKMeans(unsigned char* input, unsigned char* output, unsigned width, unsigned height, int k, float* means, int maxIterations)
{
	short*   groups = new short[width * height];
	short*   hues = new short[width * height];
	short    max = 0;

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			unsigned char r, g, b;
			r = input[(y * width + x) * 3 + 0];
			g = input[(y * width + x) * 3 + 1];
			b = input[(y * width + x) * 3 + 2];
			ofColor c;
			c.set(r, g, b);
			short hue = c.getHue();
			hues[y * width + x] = hue;
			if (max < hue)
				max = hue;
		}
	}

	for (int i = 0; i < k; i++)
	{
		means[i] = (max / k * (i + 1));
	}

	bool pixelChangeGroup = true;
	int  iteration = 0;

	setGroupsHue(hues, groups, width, height, means, k);

	while (iteration < maxIterations)
	{
		setNewMeans(hues, groups, width, height, means, k);
		pixelChangeGroup = setGroupsHue(hues, groups, width, height, means, k);
		iteration++;
		if (!pixelChangeGroup)
			iteration = maxIterations;
	}
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			ofColor c;
			c.setHsb(0, 255.0f, 255.0f);
			c.setHueAngle(means[groups[y * width + x]]);
			output[(y * width + x) * 3 + 0] = c.r;
			output[(y * width + x) * 3 + 1] = c.g;
			output[(y * width + x) * 3 + 2] = c.b;
		}
	}
	delete[]groups;
	delete[]hues;
}
//------------------------------------------------------------------------
//-------------------------CPUProcessing----------------------------------
//------------------------------------------------------------------------

GPUProcessing::GPUProcessing()
{
}


GPUProcessing::~GPUProcessing()
{
}
void GPUProcessing::invert(ofTexture& tex, ofFbo& out_fbo, ofVec3f channel)
{
	ofShader    shader;

	if (ofGetUsingArbTex()) {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/invertArb.frag");
	}
	else {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/invertNArb.frag");
	}

	if (!out_fbo.isAllocated() ||
		tex.getWidth() != out_fbo.getWidth() ||
		tex.getHeight() != out_fbo.getHeight())
	{
		out_fbo.allocate(tex.getWidth(), tex.getHeight());
	}

	if (shader.isLoaded()) {
		out_fbo.begin();
		ofClear(0, 0, 0, 255);
		shader.begin();

		shader.setUniformTexture("tex0", tex, 1);

		shader.setUniform3f("channel", channel);

		out_fbo.draw(0, 0);

		shader.end();
		out_fbo.end();
	}
}
void GPUProcessing::blur(ofTexture& tex, ofFbo& out_fbo, Kernel& kernel, ofVec3f channel)
{
	ofShader    shader;

	if (ofGetUsingArbTex()) {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/blurArb.frag");
	}
	else {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/blurNArb.frag");
	}

	if (!out_fbo.isAllocated() ||
		tex.getWidth() != out_fbo.getWidth() ||
		tex.getHeight() != out_fbo.getHeight() ) 
	{
		out_fbo.allocate(tex.getWidth(), tex.getHeight());
	}

	if (shader.isLoaded()) {
		out_fbo.begin();
		ofClear(0, 0, 0, 255);
		shader.begin();

		shader.setUniformTexture("tex0", tex, 1);
		if (!ofGetUsingArbTex())
			shader.setUniform1f("texSize", float(tex.getWidth()));

		shader.setUniform1i("diameter", kernel.getDiameter());
		shader.setUniform1fv("kernel", kernel.getKernel(), kernel.getDiameter()*kernel.getDiameter());
		shader.setUniform3f("channel", channel);
		out_fbo.draw(0, 0);

		shader.end();
		out_fbo.end();
	}
}
void GPUProcessing::edge(ofTexture& tex, ofFbo& out_fbo, const int hKernel[], const int vKernel[], ofVec3f greyConvertion)
{
	ofShader    shader;

	if (ofGetUsingArbTex()) {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/edgeArb.frag");
	}
	else {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/edgeNArb.frag");
	}

	if (!out_fbo.isAllocated() ||
		tex.getWidth() != out_fbo.getWidth() ||
		tex.getHeight() != out_fbo.getHeight() ) 
	{
		out_fbo.allocate(tex.getWidth(), tex.getHeight());
	}

	if (shader.isLoaded()) {
		out_fbo.begin();
		ofClear(0, 0, 0, 255);
		shader.begin();

		shader.setUniformTexture("tex0", tex, 1);
		if(!ofGetUsingArbTex())
			shader.setUniform1f("texSize", float(tex.getWidth()));

		shader.setUniform1iv("Hkernel", hKernel, 3 * 3);
		shader.setUniform1iv("Vkernel", vKernel, 3 * 3);
		shader.setUniform3f("greyConvertion", greyConvertion);
		out_fbo.draw(0, 0);

		shader.end();
		out_fbo.end();
	}
}
void GPUProcessing::normalmap(ofTexture& tex, ofFbo& out_fbo, const int hKernel[], const int vKernel[], float scale, ofVec3f greyConvertion)
{
	ofShader    shader;

	if (ofGetUsingArbTex()) {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/normalmapArb.frag");
	}
	else {
		shader.load("res/shaders/defaultVertex.vert", "res/shaders/normalmapNArb.frag");
	}

	if (!out_fbo.isAllocated() ||
		tex.getWidth() != out_fbo.getWidth() ||
		tex.getHeight() != out_fbo.getHeight())
	{
		out_fbo.allocate(tex.getWidth(), tex.getHeight());
	}

	if (shader.isLoaded()) {
		out_fbo.begin();
		ofClear(0, 0, 0, 255);
		shader.begin();

		// Pass the video texture
		shader.setUniformTexture("tex0", tex, 1);
		if (!ofGetUsingArbTex())
			shader.setUniform1f("texSize", float(tex.getWidth()));

		shader.setUniform1iv("Hkernel", hKernel, 3 * 3);
		shader.setUniform1iv("Vkernel", vKernel, 3 * 3);
		shader.setUniform3f("greyConvertion", greyConvertion.x, greyConvertion.y, greyConvertion.z);
		shader.setUniform1f("scale", scale);
		out_fbo.draw(0, 0);

		shader.end();
		out_fbo.end();
	}
}
//------------------------------------------------------------------------
//----------------------------Kernel--------------------------------------
//------------------------------------------------------------------------

Kernel::Kernel(int kernelDiameter)
{
	diameter = kernelDiameter;
	kernel = new float[diameter*diameter];
}
Kernel::~Kernel()
{
	delete[] kernel;
}
void Kernel::gaussianKernel(float sigma) {
	int     n = (diameter - 1) / 2;
	float   weight = 1.0f / (2 * PI * (sigma * sigma));
	float   kernelSum = 0.0f;

	for (int fy = -n; fy <= n; fy++) {
		for (int fx = -n; fx <= n; fx++)
		{
			kernel[(fy + n) * diameter + (fx + n)] = exp(-1 * ((pow(fx, 2) + pow(fy, 2)) / (2 * pow(sigma, 2))));
			kernelSum += kernel[(fy + n) * diameter + (fx + n)];
		}
	}
	for (int fy = 0; fy < diameter; fy++) {
		for (int fx = 0; fx < diameter; fx++)
		{
			kernel[fy * diameter + fx] = kernel[fy * diameter + fx] * (1.0f / kernelSum);
			
		}
	}
}
void Kernel::boxKernel() {
	for (int i = 0; i < diameter*diameter; i++) {
		kernel[i] = 1.0f / (diameter * diameter);
	}
}
void Kernel::crossKernel() {
	int n = (diameter - 1) / 2;

	for (int fy = -n; fy <= n; fy++) {
		for (int fx = -n; fx <= n; fx++)
		{
			kernel[(fy + n) * diameter + (fx + n)] = (fy == 0 || fx == 0) ? 1.0f / float(diameter + diameter - 1) : 0.0f;
		}
	}
}
void Kernel::discKernel() {
	int     n = (diameter - 1) / 2;
	float   kernelSum = 0.0f;

	for (int fy = -n; fy <= n; fy++) {
		for (int fx = -n; fx <= n; fx++)
		{
			float value = sqrt(pow(fx, 2) + pow(fy, 2));
			kernel[(fy + n) * diameter + (fx + n)] = (value <= n) ? 1.0f : 0.0f;
			kernelSum += kernel[(fy + n) * diameter + (fx + n)];
		}
	}
	for (int fy = 0; fy < diameter; fy++) {
		for (int fx = 0; fx < diameter; fx++)
		{
			kernel[fy * diameter + fx] = kernel[fy * diameter + fx] * (1.0f / kernelSum);
		}
	}
}
float* Kernel::getKernel() {
	return kernel;
}
int Kernel::getDiameter() {
	return diameter;
}

//------------------------------------------------------------------------
//---------------------------Histogram------------------------------------
//------------------------------------------------------------------------

Histogram::Histogram()
{
	levels = 256;
	bins = new unsigned int[levels];
}
Histogram::~Histogram()
{
	delete[] bins;
}
void Histogram::update(unsigned char* data, unsigned int width, unsigned int height, ColorComponent channel) {
	reset();
	if (channel == GREY) {
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				bins[data[y*width + x]] ++;
			}
		}
	}
	else 
	{
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				bins[data[(y*width + x) * 3 + channel]] ++;
			}
		}
	}
}
float Histogram::getMean() {
	int sum = 0;
	int count = 0;
	for (int i = 0; i < levels; i++) {
		sum += bins[i] * i;
		count += bins[i];
	}
	return (float)sum / (float)count;
}
int Histogram::getMax() {
	bool done = false;
	unsigned index = levels - 1;
	while (!done) {
		if (bins[index] != 0) {
			done = true;
		}
		else {
			index--;
		}
		if (index < 0) {
			done = true;
		}
	}
	return index;
}
int Histogram::getMin() {
	bool done = false;
	unsigned index = 0;
	while (!done) {
		if (bins[index] != 0) {
			done = true;
		}
		else {
			index++;
		}

		if (index >= levels) {
			done = true;
		}
	}
	return index;
}
void Histogram::reset() {
	for (int i = 0; i < levels; i++) {
		bins[i] = 0;
	}
}

//------------------------------------------------------------------------
//----------------------------END-----------------------------------------
//------------------------------------------------------------------------
