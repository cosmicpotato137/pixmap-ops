#pragma once
#include <string>
#include <functional>
#include "glm/glm.hpp"
#include "glm/gtx/scalar_multiplication.hpp"
// code from: https://solarianprogrammer.com/2019/06/10/c-programming-reading-writing-images-stb_image-libraries/
enum AllocationType
{
	NO_ALLOCATION, SELF_ALLOCATED, STB_ALLOCATED
};

struct ppm_pixel
{
	uint32_t r, g, b;

	bool operator==(const ppm_pixel& other)
	{
		return r == other.r && g == other.g && b == other.b;
	}
};


class ppm_image
{
public:
private:
	ppm_pixel** m_Data;
	int m_Width, m_Height, m_Max;
	std::string m_Spec;

public:
	ppm_image() : m_Data(NULL), m_Width(0), m_Height(0) {}
	ppm_image(int width, int height);
	ppm_image(const ppm_image& other);
	~ppm_image();

	ppm_pixel** data() const { return m_Data; }
	const int width() const { return m_Width; }
	const int height() const { return m_Height; }
	const int size() const { return m_Height * m_Width; }
	const std::string spec() const { return m_Spec; }
	const int max() const { return m_Max; }
	ppm_pixel& get(int x, int y) const { return m_Data[x][y]; }
	void set(int x, int y, const ppm_pixel& pixel) { m_Data[x][y] = pixel; }

	bool load(const char* path);
	bool save(const char* path);
	void clear();

	ppm_image& operator=(const ppm_image& other);
	bool operator==(const ppm_image& other);

	ppm_image flip_horizontal();
	ppm_image flip_vertical();
	ppm_image gamma_correct(float gamma);
	ppm_image resize(int width, int height);
	ppm_image grayscale();
	ppm_image blend(const ppm_image& other, float alpha);
	ppm_image subimage(int row, int column, int width, int height);
	ppm_image replace(const ppm_image& image, int row, int column);
	
	ppm_image edge_detect(float cutoff);
	ppm_image threshold(float cutoff);
	ppm_image apply_kernel(const float** kernel, int width, int height, std::function<ppm_pixel(float[3])> post);
	ppm_image channel_red();
	ppm_image channel_green();
	ppm_image channel_blue();

};

ppm_pixel sig_post(float* kv);
ppm_pixel clamp_post(float* kv);
ppm_pixel max_rgb_post(float* kv);