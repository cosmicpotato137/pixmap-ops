#include "ppm_image.h"
#include "test.h"
#include <iostream>
using namespace std;

void test_method()
{
	ppm_image image;
	image.load("res/feep.ppm");
	image.save("res/feep-test-save.ppm"); // should match original

	// should print 4 4
	cout << "loaded feep: " << image.width() << " " << image.height() << endl;

	// test: copy constructor
	ppm_image copy = image;
	copy.save("res/feep-test-copy.ppm"); // should match original and load into gimp

	// test: assignment operator
	copy = image;
	copy.save("res/feep-test-assignment.ppm"); // should match original and load into gimp

	// should print r,g,b
	ppm_pixel pixel = image.get(1, 1);
	cout << (int)pixel.r << " " << (int)pixel.g << " " << (int)pixel.b << endl;

	// test: setting a color
	pixel.r = 255;
	image.set(1, 1, pixel);
	image.save("res/feep-test-newcolor.ppm");

	// test a non-trivial image
	image.load("res/earth-ascii.ppm"); // a real image

	// should print 400 400
	cout << "loaded earth: " << image.width() << " " << image.height() << endl;

	// resize
	ppm_image resize = image.resize(200, 300);
	resize.save("res/earth-200-300.ppm");

	// grayscale
	ppm_image grayscale = image.grayscale();
	grayscale.save("res/earth-grayscale.ppm");

	// flip horizontal
	ppm_image flip = image.flip_horizontal();
	flip.save("res/earth-flip.ppm");

	// sub image
	ppm_image sub = image.subimage(200, 200, 100, 100);
	sub.save("res/earth-subimage.ppm");

	// gamma correction
	ppm_image gamma = image.gamma_correct(0.6f);
	gamma.save("res/earth-gamma-0.6.ppm");

	gamma = image.gamma_correct(2.2f);
	gamma.save("res/earth-gamma-2.2.ppm");

	// alpha blend
	ppm_image soup;
	soup.load("res/soup-ascii.ppm");

	int y = (int)(0.5f * (image.width() - soup.width()));
	int x = (int)(0.5f * (image.height() - soup.height()));
	ppm_image background = image.subimage(x, y, soup.width(), soup.height());
	background.save("res/background-test.ppm");
	ppm_image blend = background.alpha_blend(soup, 0.5f);
	image = image.replace(blend, x, y);
	image.save("res/earth-blend-0.5.ppm");
}