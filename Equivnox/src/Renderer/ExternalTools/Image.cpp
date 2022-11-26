#include "eqxpch.h"

#include "Image.h"

namespace EQX {

	Color Color::White = Color(255, 255, 255, 255);
	Color Color::Black = Color(0, 0, 0, 255);

	Color::Color() : r(0), g(0), b(0), a(255) {	}

	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a) {	}

	Color::Color(Vector4& v) : r(v.x), g(v.y), b(v.z), a(v.w) {   }

	Color::Color(Vector3& v) : Color({ v.x, v.y, v.z, 255 }) {	}

	Color::Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {	}

	Color& Color::operator= (const Color& c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
		return *this;
	}


	Image::Image()
	{
		this->type = ImageType::TGA;
		this->width = 100;
		this->height = 100;
		this->canvas = new Color[100 * 100];
		this->filename = "output";
		for (unsigned int i = 0; i != 100 * 100; ++i)
			this->canvas[i] = Color::Black;
	}

	Image::Image(unsigned int w, unsigned int h, ImageType t, std::string n)
	{
		this->type = t;
		this->width = w;
		this->height = h;
		this->canvas = new Color[static_cast<size_t>(w) * static_cast<size_t>(h)];
		this->filename = n;
		for (size_t i = 0; i != static_cast<size_t>(w) * static_cast<size_t>(h); ++i)
			this->canvas[i] = Color::Black;
	}

	Image::~Image()
	{
		if (canvas)
			delete[] canvas;
	}

	void Image::set(unsigned int w, unsigned int h, Color c)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = c;
	}

	Color Image::get(unsigned int h, unsigned int w) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[h * this->width + w];
	}

	void Image::clear()
	{
		for (unsigned int i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Color::Black;
	}

	void Image::write()
	{
		cout << (int)(canvas[256 * width + 196].r) << endl;
		if (this->type == ImageType::TGA)
		{
			TGAImage image(this->width, this->height, TGAImage::RGBA);
			unsigned int w, h;
			for (unsigned int i = 0; i < width * height; ++i)
			{
				w = i % width;
				h = i / width;
				//if (canvas[i].r != 0)
					//cout << w << " " << h << " " << (int)(canvas[i].r) << endl;
				image.set(w, h, toTGAColor(canvas[i]));
			}
			image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
			image.write_tga_file((this->filename + ".tga").c_str());
		}
	}


	/**
	 * Mix TGAColors
	 *
	 * @param fore Newly added color
	 * @param back Originally existing color
	 * @param coeff The opacity of fore: 1 for completely opaque
	 * @return blended color
	 */
	Color blendColor(Color fore, Color back, float coeff)
	{
		return Color((int)(fore.r * coeff + back.r * (1 - coeff)),
			(int)(fore.g * coeff + back.g * (1 - coeff)),
			(int)(fore.b * coeff + back.b * (1 - coeff)),
			(int)(fore.a * coeff + back.a * (1 - coeff)));
	}

	inline TGAColor toTGAColor(Color c)
	{
		return TGAColor(c.r, c.g, c.b, c.a);
	}
}
