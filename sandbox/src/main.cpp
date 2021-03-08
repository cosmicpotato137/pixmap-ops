#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

#include "Log.h"
#include "ppm_image.h"

int main(int argc, char** argv)
{
	Log::Init("HW1");

	// vertical gradiant
	const float** gx = { new const float* [3]{ 
		new float[3]{ -1, -2, -1 },
		new float[3]{ 0, 0, 0 },
		new float[3]{ 1, 2, 1 }
	}};

	// gaussian blur
	const float** blur = { new const float* [3]{ 
		new float[3]{ 1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f },
		new float[3]{ 2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f },
		new float[3]{ 1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f }
	}};

	// sharpen
	const float** sharpen = { new const float* [3]{
		new float[3]{ 0.0f, -1.0f, 0.0f },
		new float[3]{ -1.0f, 5.0f, -1.0f },
		new float[3]{ 0.0f, -1.0f, 0.0f }
	} };

	const float** emboss = { new const float* [3]{
		new float[3]{ -2.0f, -1.0f, 0.0f },
		new float[3]{ -1.0f, 1.0f, 1.0f },
		new float[3]{ 0.0f, 1.0f, 2.0f }
	} };
	
	const float** outline = { new const float* [3]{
		new float[3]{ -1.0f, -1.0f, -1.0f },
		new float[3]{ -1.0f, 8.0f, -1.0f },
		new float[3]{ -1.0f, -1.0f, -1.0f }
	} };


	ppm_image pixels;
	pixels.load("res/building.ppm");

	pixels = pixels.apply_kernel(blur, 3, 3, clamp_post);

	//pixels = pixels.apply_kernel(outline, 3, 3, sig_post);

	pixels = pixels.edge_detect(.7);
	//pixels = pixels.threshold(.7);
	pixels.save("res/building(1).ppm");
	
	return 0;
}