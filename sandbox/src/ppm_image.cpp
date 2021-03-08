#include <filesystem>
#include <fstream>
#include <functional>
#include "Log.h"
#include "ppm_image.h"
#include "image_math.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "STB/stb_image_write.h"

namespace fs = std::filesystem;

ppm_image::ppm_image(int width, int height)
	: m_Data(NULL), m_Width(width), m_Height(height),
	m_Spec("P3"), m_Max(255)
{
	m_Data = (ppm_pixel**)malloc(m_Height * sizeof(ppm_pixel*));
	//m_Data = { new ppm_pixel*[m_Height] };
	for (int i = 0; i < m_Height; i++)
	{
		m_Data[i] = (ppm_pixel*)malloc(m_Width * sizeof(ppm_pixel));
		//m_Data[i] = { new ppm_pixel[m_Width] };
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = { 0, 0, 0 };
		}
	}
}

ppm_image::ppm_image(const ppm_image& other)
	: m_Data(NULL), m_Width(other.width()), m_Height(other.height()),
	m_Spec(other.spec()), m_Max(other.max())
{
	m_Data = (ppm_pixel**)malloc(m_Height * sizeof(ppm_pixel*));
	//m_Data = { new ppm_pixel*[m_Height] };
	for (int i = 0; i < m_Height; i++)
	{
		m_Data[i] = (ppm_pixel*)malloc(m_Width * sizeof(ppm_pixel));
		//m_Data[i] = { new ppm_pixel[m_Width] };
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p = other.get(i, j);
			m_Data[i][j] = { p.r, p.g, p.b };
		}
	}
}

ppm_image::~ppm_image()
{
	clear();
}

bool ppm_image::load(const char* path)
{
	FILE* img;
	int is_eof = 0;
	fopen_s(&img, path, "rb");
	CORE_ASSERT(img, "Filepath doesn't exist!");

	char spec[16];
	is_eof = fscanf(img, "%s", spec);
	m_Spec = spec;
	is_eof = fscanf(img, "%i %i", &m_Width, &m_Height);
	is_eof = fscanf(img, "%i", &m_Max);
	CORE_VALIDATE(is_eof != EOF, return false, "Image contains no data!");

	m_Data = (ppm_pixel**)malloc(m_Height * sizeof(ppm_pixel*));
	//m_Data = { new ppm_pixel*[m_Height] };
	for (int i = 0; i < m_Height; i++)
	{
		m_Data[i] = (ppm_pixel*)malloc(m_Width * sizeof(ppm_pixel));
		//m_Data[i] = { new ppm_pixel[m_Width] };
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p;
			is_eof = fscanf(img, "%u", &p.r);
			is_eof = fscanf(img, "%u", &p.g);
			is_eof = fscanf(img, "%u", &p.b);
			CORE_VALIDATE(is_eof != EOF, return false, "Image data corrupted, can't read file!");
			m_Data[i][j] = p;
		}
	}
	fclose(img);
}

bool ppm_image::save(const char* path)
{
	std::ofstream newImg(path);
	newImg << m_Spec << std::endl;
	newImg << m_Width << " " << m_Height << std::endl;
	newImg << m_Max << std::endl;
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel pixel = m_Data[i][j];
			newImg << pixel.r << std::endl << pixel.g << std::endl << pixel.b << std::endl;
		}
	}
	newImg.close();
	return false;
}

void ppm_image::clear()
{
	for (int i = 0; i < m_Height; i++)
		free(m_Data[i]);
	free(m_Data);
}

ppm_image& ppm_image::operator=(const ppm_image& other)
{
	if (*this == other)
		return *this;
	if (m_Data)
		clear();
	m_Height = other.height();
	m_Width = other.width();
	m_Spec = other.spec();
	m_Max = other.max();
	m_Data = (ppm_pixel**)malloc(m_Height * sizeof(ppm_pixel*));
	//m_Data = { new ppm_pixel*[m_Height] };
	for (int i = 0; i < m_Height; i++)
	{
		m_Data[i] = (ppm_pixel*)malloc(m_Width * sizeof(ppm_pixel));
		//m_Data[i] = { new ppm_pixel[m_Width] };
		for (int j = 0; j < m_Width; j++)
		{
			m_Data[i][j] = other.get(i, j);
		}
	}
}

bool ppm_image::operator==(const ppm_image& other)
{
	bool result = true;
	result *= other.m_Height == m_Height;
	result *= other.m_Width == m_Width;
	result *= other.m_Spec == m_Spec;
	result *= other.m_Max == m_Max;
	if (!result)
		return false;
	for (int i = 0; i < m_Height; i++)
		for (int j = 0; j < m_Width; j++)
			result *= other.get(i, j) == get(i, j);
	return result;
}

ppm_image ppm_image::flip_horizontal()
{
	ppm_image flipped(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			flipped.set(i, j, get(i, m_Width - j - 1));
		}
	}
	return flipped;
}

ppm_image ppm_image::flip_vertical()
{
	ppm_image flipped(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			flipped.set(i, j, get(m_Height - i - 1, j));
		}
	}
	return flipped;
}

ppm_image ppm_image::gamma_correct(float gamma)
{
	float gc = 1 / gamma;
	ppm_image new_gamma(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p = get(i, j);
			new_gamma.set(i, j, {
				uint32_t(255.0f * pow(p.r / 255.0f, gc)),
				uint32_t(255.0f * pow(p.g / 255.0f, gc)),
				uint32_t(255.0f * pow(p.b / 255.0f, gc))});
		}
	}
	return new_gamma;
}

ppm_image ppm_image::grayscale()
{
	ppm_image gray(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p = get(i, j);
			uint32_t avg = (p.r + p.g + p.b) / 3;
			gray.set(i, j, { avg, avg, avg });
		}
	}
	return gray;
}

ppm_image ppm_image::blend(const ppm_image& other, float alpha)
{
	CORE_VALIDATE(other.width() == m_Width && other.height() == m_Height,
		return ppm_image(m_Width, m_Height), "Dimensions of images must be the same!");
	ppm_image blended(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p = get(i, j);
			ppm_pixel po = other.get(i, j);
			ppm_pixel bp = {
				(float)p.r * (1 - alpha) + (float)po.r * alpha,
				(float)p.g * (1 - alpha) + (float)po.g * alpha,
				(float)p.b * (1 - alpha) + (float)po.b * alpha
			};

			blended.set(i, j, bp);
		}
	}
	return blended;
}

ppm_image ppm_image::subimage(int row, int column, int width, int height)
{
	CORE_VALIDATE(row + height <= m_Height&& column + width <= m_Width,
		return ppm_image(), "Parameters out of bounds of image!");
	ppm_image sub(width, height);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			ppm_pixel p = get(i + row, j + column);
			sub.set(i, j, p);
		}
	}
	return sub;
}

ppm_image ppm_image::replace(const ppm_image& image, int row, int column)
{
	ppm_image repl(row + image.width(), column + image.height());
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			repl.set(i, j, get(i, j));
		}
	}
	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			repl.set(i + row, j + column, image.get(i, j));
		}
	}
	return repl;
}

ppm_image ppm_image::resize(int width, int height)
{
	float jfac = (float)m_Width / (float)width;
	float ifac = (float)m_Height / (float)height;
	ppm_image resized(width, height);
	for (int i = 0; i < resized.height(); i++)
	{
		for (int j = 0; j < resized.width(); j++)
		{
			ppm_pixel p = get(ifac * (float)i, jfac * (float)j);
			resized.set(i, j, p);
		}
	}
	return resized;
}

ppm_image ppm_image::edge_detect(float cutoff)
{
	glm::mat3 gx = {
		-1, 0, 1,
		-2, 0, 2,
		-1, 0, 1
	};
	glm::mat3 gy = {
		-1, -2, -1,
		0, 0, 0,
		1, 2, 2
	};
	ppm_image input = this->grayscale();
	ppm_image result(m_Width-3, m_Height-3);
	for (int i = 0; i < m_Height - 3; i++)
	{
		for (int j = 0; j < m_Width - 3; j++)
		{
			glm::mat3 block = {
				input.get(i + 0, j + 0).r, input.get(i + 1, j + 0).r, input.get(i + 0, j + 0).r,
				input.get(i + 0, j + 1).r, input.get(i + 1, j + 1).r, input.get(i + 2, j + 1).r,
				input.get(i + 0, j + 2).r, input.get(i + 1, j + 2).r, input.get(i + 2, j + 2).r
			};
			uint32_t d = sqrt(pow(conv_mat3(block, gx), 2) + pow(conv_mat3(block, gy), 2));
			result.set(i, j, { d, d, d });
		}
	}

	return result.threshold(cutoff);
}

// cutoff is a float in range [0, 1]
ppm_image ppm_image::threshold(float cutoff)
{
	ppm_image result(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			ppm_pixel p = get(i, j);
			if ((p.r + p.g + p.b) / (3 * 255) > cutoff)
				result.set(i, j, p);
		}
	}
	return result;
}

ppm_image ppm_image::apply_kernel(const float** kernel, int kwidth, int kheight, std::function<ppm_pixel (float[3])> post)
{
	int newWidth = m_Width - kwidth;
	int newHeight = m_Height - kheight;
	CORE_VALIDATE(newWidth <= m_Width && newHeight <= m_Height, return ppm_image(kwidth, kheight), 
		"width or height too large!");
	ppm_image result(newWidth, newHeight);

	// iterate through image pixels
	for (int i = 0; i < newHeight; i++)
	{
		for (int j = 0; j < newWidth; j++)
		{
			// iterate through kernel
			float kv[3] = { 0, 0, 0 };
			for (int k = 0; k < kheight; k++)
			{
				for (int l = 0; l < kwidth; l++)
				{
					ppm_pixel p = get(i + k, j + l);
					kv[0] += kernel[kheight - k - 1][kwidth - l - 1] * p.r;
					kv[1] += kernel[kheight - k - 1][kwidth - l - 1] * p.g;
					kv[2] += kernel[kheight - k - 1][kwidth - l - 1] * p.b;
				}
			}
			result.set(i, j, post(kv));
		}
	}
	return result;
}

ppm_image ppm_image::channel_red()
{
	ppm_image result = ppm_image(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			result.set(i, j, { get(i, j).r, 0, 0 });
		}
	}
	return result;
}

ppm_image ppm_image::channel_green()
{
	ppm_image result = ppm_image(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			result.set(i, j, { 0, get(i, j).g, 0 });
		}
	}
	return result;
}

ppm_image ppm_image::channel_blue()
{
	ppm_image result = ppm_image(m_Width, m_Height);
	for (int i = 0; i < m_Height; i++)
	{
		for (int j = 0; j < m_Width; j++)
		{
			result.set(i, j, { 0, 0, get(i, j).b });
		}
	}
	return result;
}
//Gx = [-1 0 1; -2 0 2; -1 0 1]
//Gy = [-1 - 2 - 1; 0 0 0; 1 2 1]
//
//rows = size(A, 1)
//columns = size(A, 2)
//mag = zeros(A)
//
//for i = 1:rows - 2
//for j = 1 : columns - 2
//S1 = sum(sum(Gx.*A(i:i + 2, j : j + 2)))
//S2 = sum(sum(Gy.*A(i:i + 2, j : j + 2)))
//
//mag(i + 1, j + 1) = sqrt(S1. ^ 2 + S2. ^ 2)
//end for
//end for
//
//threshold = 70 % varies for application[0 255]
//output_image = max(mag == round(threshold)) = 0;
//return output_image, threshold)
//output_image(output_image

ppm_pixel sig_post(float* kv)
{
	return {
		(uint8_t)sigmoid(kv[0]),
		(uint8_t)sigmoid(kv[1]),
		(uint8_t)sigmoid(kv[2])
	};
}

ppm_pixel clamp_post(float* kv)
{
	
	return {
		(uint32_t)min(255.0f, abs(kv[0])),
		(uint32_t)min(255.0f, abs(kv[1])),
		(uint32_t)min(255.0f, abs(kv[2]))
	};
}

ppm_pixel max_rgb_post(float* kv)
{
	uint8_t mv = max(abs(kv[0]), max(abs(kv[1]), abs(kv[2])));
	return { mv, mv, mv };
}
