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

	Color::Color(EQXFloat grey) : r(grey), g(grey), b(grey), a(255) {  }

	Color::Color(EQXFloat r, EQXFloat g, EQXFloat b, EQXFloat a) :
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

	_ColorIntermediate operator* (const EQXFloat k, const Color v)
	{
		_ColorIntermediate ans(v);
		for (size_t index = 0; index != 3; ++index)
			ans[index] = k * ans[index];
		return ans;
	}

#ifdef EQX_DEBUG
	void Print(const Color& c)
	{
		cout << "Color: " << static_cast<int>(c.r) << " " << static_cast<int>(c.g) << 
			" " << static_cast<int>(c.b) << " " << static_cast<int>(c.a) << endl;
	}
#endif

	_ColorIntermediate operator* (const Color v, const EQXFloat k)
	{
		return k * v;
	}

	_ColorIntermediate operator/(const Color v, const EQXFloat k)
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

	Color& Color::operator+=(const Color& c)
	{
		*this = *this + c;
		return *this;
	}

	bool Color::operator==(const Color& c)
	{
		return (c.r == this->r && c.g == this->g && c.b == this->b && c.a == this->a);
	}

	bool Color::operator!=(const Color& c)
	{
		return !(*this == c);
	}

	Color LitColor(const Color c1, const Color c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t index = 0; index != 3; ++index)
			ans[index] = ans[index] * static_cast<EQXFloat>(c2[index]) / 255;
		ans.a = std::max(c1.a, c2.a);
		return ans;
	}

	_ColorIntermediate::_ColorIntermediate(Color c)
	{
		for (size_t ind = 0; ind != 4; ++ind)
			(*this)[ind] = static_cast<EQXFloat>(c[ind]);
	}

	_ColorIntermediate::_ColorIntermediate(const _ColorIntermediate& c)
	{
		for (size_t ind = 0; ind != 4; ++ind)
			(*this)[ind] = c[ind];
	}

	EQXFloat& _ColorIntermediate::operator[](size_t index)
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

	const EQXFloat& _ColorIntermediate::operator[](size_t index) const
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

	_ColorIntermediate operator*(const _ColorIntermediate c, const EQXFloat k)
	{
		_ColorIntermediate ans(c);
		for (size_t ind = 0; ind != 3; ++ind)
			ans[ind] = c[ind] * k;
		return ans;
	}

	_ColorIntermediate operator*(const EQXFloat k, const _ColorIntermediate c)
	{
		return c * k;
	}

	_ColorIntermediate operator/(const _ColorIntermediate c, const EQXFloat k)
	{
		if (k == 0)
			return c;
		return (1 / k) * c;
	}

	_ColorIntermediate operator+(const Color c1, const Color c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 3; ++ind)
			ans[ind] = ans[ind] + static_cast<EQXFloat>(c2[ind]);
		return ans;
	}

	_ColorIntermediate operator+(const _ColorIntermediate c1, const _ColorIntermediate c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 3; ++ind)
			ans[ind] = ans[ind] + c2[ind];
		return ans;
	}

	_ColorIntermediate operator+(const Color c1, const _ColorIntermediate c2)
	{
		_ColorIntermediate ans(c1);
		for (size_t ind = 0; ind != 3; ++ind)
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
		for (EQXUInt i = 0; i != 100 * 100; ++i)
			this->canvas[i] = Color::Black;
	}

#pragma warning(push)
#pragma warning(disable:6385)
// MSVC cannot properly identify allocated vector size

	Image::Image(const ImageGrey& image)
	{
		this->width = image.GetWidth();
		this->height = image.GetHeight();
		const EQXFloat* imageCanvas = image.GetCanvas();
		this->canvas = new Color[static_cast<size_t>(this->width) * static_cast<size_t>(this->height)];
		for (size_t i = 0; i != static_cast<size_t>(this->width) * static_cast<size_t>(this->height); ++i)
			this->canvas[i] = Color(imageCanvas[i]);
	}

	Image::Image(EQXUInt w, EQXUInt h)
	{
		if (w > 2000)
			w = 2000;
		if (h > 2000)
			h = 2000;
		this->width = w;
		this->height = h;
		this->canvas = new Color[static_cast<size_t>(w) * static_cast<size_t>(h)];
		for (size_t i = 0; i != static_cast<size_t>(w) * static_cast<size_t>(h); ++i)
			this->canvas[i] = Color::Black;
	}

#pragma warning(pop)

	Image::Image(const Image& image)
	{
		*this = image;
	}

	Image::~Image()
	{
		if (canvas)
			delete[] canvas;
	}

#pragma warning(push)
#pragma warning(disable:6385)
// MSVC cannot properly identify allocated vector size

	Image& Image::operator= (const Image& image)
	{
		if (this->canvas)
			delete[] canvas;

		this->width = image.width;
		this->height = image.height;
		this->canvas = new Color[image.width * image.height];
		for (EQXUInt i = 0; i != image.width * image.height; ++i)
			this->canvas[i] = image.canvas[i];

		return *this;
	}

	Image& Image::operator=(const ImageGrey& image)
	{
		if (this->canvas)
			delete[] canvas;

		this->width = image.GetWidth();
		this->height = image.GetHeight();
		this->canvas = new Color[image.GetWidth() * image.GetHeight()];
		const EQXFloat* imageCanvas = image.GetCanvas();
		for (EQXUInt i = 0; i != image.GetWidth() * image.GetHeight(); ++i)
			this->canvas[i] = Color(imageCanvas[i]);

		return *this;
	}

#pragma warning(pop)

	// INSP change "h * this->width + w" to "h * w + w" for interesting results
	void Image::Set(EQXUInt w, EQXUInt h, Color c)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = c;
	}

	Color Image::Get(EQXUInt w, EQXUInt h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return Color::Black;
	}

	void Image::Clear()
	{
		for (EQXUInt i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Color::Black;
	}

	void Image::Write(ImageType type, std::string filename)
	{
		if (type == ImageType::TGA)
		{
			TGAImage image(this->width, this->height, TGAImage::RGBA);
			EQXUInt w, h;
			for (EQXUInt i = 0; i < width * height; ++i)
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

	void Image::Rescale(EQXUInt w, EQXUInt h, RescaleFunc func)
	{
		EQXFloat pixelLength = 1.0f / w * this->width;
		EQXFloat pixelHeight = 1.0f / h * this->height;
		int wStartIndex, wEndIndex, hStartIndex, hEndIndex;
		bool magnify = this->width > w && this->height > h;
		Image res(w, h);
#ifdef EQX_DEBUG
		EQX_LOG(w)
#endif
		for (int xpos = 0; xpos != w; ++xpos)
			for (int ypos = 0; ypos != h; ++ypos)
			{
				Vec2 curPos = Vec2((xpos + 1 / 2) * pixelLength, (ypos + 1 / 2) * pixelHeight);
				wStartIndex = std::floor(curPos.x - 0.5) - 2;
				hStartIndex = std::floor(curPos.y - 0.5) - 2;
				wEndIndex = wStartIndex + 5;
				hEndIndex = hStartIndex + 5;
				if (wStartIndex < 0)
					wStartIndex = 0;
				if (hStartIndex < 0)
					hStartIndex = 0;
				if (wEndIndex >= this->width)
					wEndIndex = this->width - 1;
				if (hEndIndex >= this->height)
					hEndIndex = this->height - 1;
				EQXFloat distance = 0;
				Color resultColor = Color::Black;
				Color pixelColor = Color::Black;
				for (int x = wStartIndex; x <= wEndIndex; ++x)
				{
					for (int y = hStartIndex; y <= hEndIndex; ++y)
					{
						pixelColor = this->Get(x, y);
						Vec2 sampledPos = Vec2(x + 0.5, y + 0.5);
						Vec2 disp = sampledPos + curPos.Neg();
						distance = disp.Norm();
						if (magnify)
						{
							if (std::abs(disp.x) <= 1 && std::abs(disp.y) <= 1)
								resultColor += 0.3f * pixelColor;
							else
								resultColor += EvalBSplineCubic(distance) / 1.33f * pixelColor;
						}
						else
							resultColor += EvalBSplineCubic(distance) * pixelColor;
					}
				}
				res.Set(xpos, ypos, resultColor);
			}
		*this = res;
	}

	ImageGrey::ImageGrey()
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new EQXFloat[100 * 100];
		for (EQXUInt i = 0; i != 100 * 100; ++i)
			this->canvas[i] = 0;
	}

	ImageGrey::ImageGrey(EQXUInt w, EQXUInt h)
	{
		this->width = w;
		this->height = h;
		this->canvas = new EQXFloat[static_cast<size_t>(w) * static_cast<size_t>(h)];
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
		this->canvas = new EQXFloat[image.width * image.height];
		for (EQXUInt i = 0; i != image.width * image.height; ++i)
			this->canvas[i] = image.canvas[i];

		return *this;
	}

	void ImageGrey::Set(EQXUInt w, EQXUInt h, EQXFloat c)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = c;
	}

	EQXFloat ImageGrey::Get(EQXUInt w, EQXUInt h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return 0.0f;
	}

	void ImageGrey::Clear()
	{
		for (EQXUInt i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = 0;
	}

	void ImageGrey::Write(ImageType type, std::string filename)
	{
		if (type == ImageType::TGA)
		{
			TGAImage image(this->width, this->height, TGAImage::RGBA);
			EQXUInt w, h;
			for (EQXUInt i = 0; i < width * height; ++i)
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
	Color blendColor(Color fore, Color back, EQXFloat coeff)
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
