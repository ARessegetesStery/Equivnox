#include "eqxpch.h"

#include "Image.h"

// include it here as stb is header-only
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "vendor/stb/stb_image.h"
#include "vendor/stb/stb_image_write.h"

namespace EQX {

	Color Color::White = Color(255, 255, 255, 255);
	Color Color::Black = Color(0, 0, 0, 255);

	Color::Color() : r(0), g(0), b(0), a(255) {	 }

	Color::Color(float grey) : r(grey), g(grey), b(grey), a(255) {  }

	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a) {  }

	Color::Color(Vector4& v) : r(v.x), g(v.y), b(v.z), a(v.w) {  }

	Color::Color(Vector3& v) : Color({ v.x, v.y, v.z, 255 }) {	}

	Color::Color(const Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {	 }

	unsigned char& Color::operator[] (size_t index)
	{
		if (index == 0)
			return r;
		else if (index == 1)
			return g;
		else if (index == 2)
			return b;
		else if (index == 3)
			return a;
		return a;
	}

	const unsigned char& Color::operator[] (size_t index) const
	{
		if (index == 0)
			return r;
		else if (index == 1)
			return g;
		else if (index == 2)
			return b;
		else if (index == 3)
			return a;
		return a;
	}

	Color operator* (const float k, const Color v)
	{
		Color ans;
		for (size_t index = 0; index != 4; ++index)
		{
			unsigned int color = v[index];
			color *= k;
			if (color > 255)
				color = 255;
			ans[index] = color;
		}
		return ans;
	}

	Color operator* (const Color v, const float k)
	{
		return k * v;
	}

	Color operator/(const Color v, const float k)
	{
		return (1/k) * v;
	}

	Color& Color::operator= (const Color& c)
	{
		this->r = c.r;
		this->g = c.g;
		this->b = c.b;
		this->a = c.a;
		return *this;
	}

	Color LitColor(const Color c1, const Color c2)
	{
		Color ans;
		for (size_t index = 0; index != 4; ++index)
		{
			unsigned int color = static_cast<unsigned int>(c1[index]) * static_cast<unsigned int>(c2[index]) / 255;
			if (color > 255)
				color = 255;
			ans[index] = color;
		}
		return ans;
	}

	Color Color::operator+ (Color& c)
	{
		Color ans;
		for (size_t index = 0; index != 4; ++index)
		{
			unsigned int color = c[index] + (*this)[index];
			if (color > 255)
				color = 255;
			ans[index] = color;
		}
		return ans;
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

	Image& Image::operator= (const Image& image)
	{
		if (this->canvas)
			delete[] canvas;

		this->type = image.type;
		this->width = image.width;
		this->height = image.height;
		this->canvas = new Color[image.width * image.height];
		this->filename = image.filename;
		for (unsigned int i = 0; i != image.width * image.height; ++i)
			this->canvas[i] = image.canvas[i];

		return *this;
	}

	// INSP change "h * this->width + w" to "h * w + w" for interesting results
	void Image::set(unsigned int w, unsigned int h, Color c)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = c;
	}

	Color Image::get(unsigned int w, unsigned int h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return Color::Black;
	}

	void Image::clear()
	{
		for (unsigned int i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Color::Black;
	}

	void Image::write()
	{
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
		if (this->type == ImageType::PNG)
		{
			stbi_flip_vertically_on_write(true);
			
			int info;
			info = stbi_write_png((this->filename + ".png").c_str(), this->width, this->height, 4, this->canvas, 0);
			if (!info)
				cout << "Writing to " << this->filename << ".png failed." << endl;
		}
	}


	/**
	 * Mix Colors
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
