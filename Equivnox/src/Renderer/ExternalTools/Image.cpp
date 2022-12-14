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

	Color::Color(_ColorIntermediate c_temp)
	{
		for (size_t ind = 0; ind < 4; ++ind)
		{
			if (c_temp[ind] > 255)
				c_temp[ind] = 255;
			else if (c_temp[ind] < 0)
				c_temp[ind] = 0;
			(*this)[ind] = static_cast<unsigned char>(c_temp[ind]);
		}
	}

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

	_ColorIntermediate operator* (const float k, const Color v)
	{
		_ColorIntermediate ans(v);
		for (size_t index = 0; index != 4; ++index)
			ans[index] = k * ans[index];
		return ans;
	}

	_ColorIntermediate operator* (const Color v, const float k)
	{
		return k * v;
	}

	_ColorIntermediate operator/(const Color v, const float k)
	{
		if (k == 0)
			return v;
		return (1 / k) * v;
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
		_ColorIntermediate ans(c1);
		for (size_t index = 0; index != 4; ++index)
			ans[index] = ans[index] * static_cast<float>(c2[index]) / 255;
		return ans;
	}

	_ColorIntermediate::_ColorIntermediate(Color c)
	{
		for (size_t ind = 0; ind != 4; ++ind)
			(*this)[ind] = static_cast<float>(c[ind]);
	}

	_ColorIntermediate::_ColorIntermediate(const _ColorIntermediate& c)
	{
		for (size_t ind = 0; ind != 4; ++ind)
			(*this)[ind] = c[ind];
	}

	float& _ColorIntermediate::operator[](size_t index)
	{
		if (index == 0)
			return this->r;
		if (index == 1)
			return this->g;
		if (index == 2)
			return this->b;
		if (index == 3)
			return this->a;
		return this->r;
	}

	const float& _ColorIntermediate::operator[](size_t index) const
	{
		if (index == 0)
			return this->r;
		if (index == 1)
			return this->g;
		if (index == 2)
			return this->b;
		if (index == 3)
			return this->a;
		return this->r;
	}

	_ColorIntermediate operator*(const _ColorIntermediate c, const float k)
	{
		_ColorIntermediate ans(c);
		for (size_t ind = 0; ind != 4; ++ind)
			ans[ind] = c[ind] * k;
		return ans;
	}

	_ColorIntermediate operator*(const float k, const _ColorIntermediate c)
	{
		return c * k;
	}

	_ColorIntermediate operator/(const _ColorIntermediate c, const float k)
	{
		if (k == 0)
			return c;
		return (1 / k) * c;
	}

	_ColorIntermediate operator+(const Color c1, const Color c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 4; ++ind)
			ans[ind] = ans[ind] + static_cast<float>(c2[ind]);
		return ans;
	}

	_ColorIntermediate operator+(const _ColorIntermediate c1, const _ColorIntermediate c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 4; ++ind)
			ans[ind] = ans[ind] + c2[ind];
		return ans;
	}

	_ColorIntermediate operator+(const Color c1, const _ColorIntermediate c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 4; ++ind)
			ans[ind] = ans[ind] + c2[ind];
		return ans;
	}

	_ColorIntermediate operator+(const _ColorIntermediate c1, const Color c2)
	{
		return c2 + c1;
	}

	Image::Image()
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new Color[100 * 100];
		for (unsigned int i = 0; i != 100 * 100; ++i)
			this->canvas[i] = Color::Black;
	}

	Image::Image(const ImageGrey& image)
	{
		this->width = image.getWidth();
		this->height = image.getHeight();
		const unsigned char* imageCanvas = image.GetCanvas();
		this->canvas = new Color[static_cast<size_t>(this->width) * static_cast<size_t>(this->height)];
		for (size_t i = 0; i != static_cast<size_t>(this->width) * static_cast<size_t>(this->height); ++i)
			this->canvas[i] = Color(imageCanvas[i]);
	}

	Image::Image(unsigned int w, unsigned int h)
	{
		this->width = w;
		this->height = h;
		this->canvas = new Color[static_cast<size_t>(w) * static_cast<size_t>(h)];
		for (size_t i = 0; i != static_cast<size_t>(w) * static_cast<size_t>(h); ++i)
			this->canvas[i] = Color::Black;
	}

	Image::Image(const Image& image)
	{
		*this = image;
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

		this->width = image.width;
		this->height = image.height;
		this->canvas = new Color[image.width * image.height];
		for (unsigned int i = 0; i != image.width * image.height; ++i)
			this->canvas[i] = image.canvas[i];

		return *this;
	}

	Image& Image::operator=(const ImageGrey& image)
	{
		if (this->canvas)
			delete[] canvas;

		this->width = image.getWidth();
		this->height = image.getHeight();
		this->canvas = new Color[image.getWidth() * image.getHeight()];
		const unsigned char* imageCanvas = image.GetCanvas();
		for (unsigned int i = 0; i != image.getWidth() * image.getHeight(); ++i)
			this->canvas[i] = Color(imageCanvas[i]);

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

	void Image::write(ImageType type, std::string filename)
	{
		if (type == ImageType::TGA)
		{
			TGAImage image(this->width, this->height, TGAImage::RGBA);
			unsigned int w, h;
			for (unsigned int i = 0; i < width * height; ++i)
			{
				w = i % width;
				h = i / width;
				image.set(w, h, toTGAColor(canvas[i]));
			}
			image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
			image.write_tga_file((filename + ".tga").c_str());
		}
		if (type == ImageType::PNG)
		{
			stbi_flip_vertically_on_write(true);
			
			int info;
			info = stbi_write_png((filename + ".png").c_str(), this->width, this->height, 4, this->canvas, 0);
			if (!info)
				cout << "Writing to " << filename << ".png failed." << endl;
		}
	}

	ImageGrey::ImageGrey()
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new unsigned char[100 * 100];
		for (unsigned int i = 0; i != 100 * 100; ++i)
			this->canvas[i] = 0;
	}

	ImageGrey::ImageGrey(unsigned int w, unsigned int h)
	{
		this->width = w;
		this->height = h;
		this->canvas = new unsigned char[static_cast<size_t>(w) * static_cast<size_t>(h)];
		for (size_t i = 0; i != static_cast<size_t>(w) * static_cast<size_t>(h); ++i)
			this->canvas[i] = 0;
	}

	ImageGrey::~ImageGrey()
	{
		if (canvas)
			delete[] canvas;
	}

	ImageGrey& ImageGrey::operator= (const ImageGrey& image)
	{
		if (this->canvas)
			delete[] canvas;

		this->width = image.width;
		this->height = image.height;
		this->canvas = new unsigned char[image.width * image.height];
		for (unsigned int i = 0; i != image.width * image.height; ++i)
			this->canvas[i] = image.canvas[i];

		return *this;
	}

	void ImageGrey::set(unsigned int w, unsigned int h, Color c)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = c.r;
	}

	Color ImageGrey::get(unsigned int w, unsigned int h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return Color(this->canvas[(size_t)(h * this->width + w)]);
		return Color::Black;
	}

	void ImageGrey::clear()
	{
		for (unsigned int i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = 0;
	}

	void ImageGrey::write(ImageType type, std::string filename)
	{
		if (type == ImageType::TGA)
		{
			TGAImage image(this->width, this->height, TGAImage::RGBA);
			unsigned int w, h;
			for (unsigned int i = 0; i < width * height; ++i)
			{
				w = i % width;
				h = i / width;
				image.set(w, h, toTGAColor(Color(canvas[i])));
			}
			image.flip_vertically(); // Ensure x horizontal, y vertical, origin lower-left corner
			image.write_tga_file((filename + ".tga").c_str());
		}
		if (type == ImageType::PNG)
		{
			stbi_flip_vertically_on_write(true);

			Image imageRGBA(*this);
			int info;
			info = stbi_write_png((filename + ".png").c_str(), this->width, this->height, 4, imageRGBA.GetCanvas(), 0);
			if (!info)
				cout << "Writing to " << filename << ".png failed." << endl;
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
