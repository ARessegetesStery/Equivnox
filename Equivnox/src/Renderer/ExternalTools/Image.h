#pragma once

#include "eqxpch.h"

#include "vendor/TGA/tgaimage.h"

namespace EQX {
	// TODO create compact image for ZBuf; create RGBA ZBuf for higher precision depth info

	class Color;
	class _ColorIntermediate;
	class Image;
	class ImageGrey;

	enum class ImageType
	{
		TGA,	// Relies on tgaimage.h
		PNG		// Relies on stb libraries
	};

	class Color
	{
		friend class _ColorIntermediate;
	public:
		unsigned char r, g, b, a;

	public:
		Color();
		Color(float);
		Color(float, float, float, float);
		Color(_ColorIntermediate);			// clamps at 0 and 255
		Color(Vector4&);
		Color(Vector3&);
		Color(const Color&);

		Color& operator= (const Color&);
		Color& operator+= (const Color&);
		unsigned char& operator[] (size_t index);
		const unsigned char& operator[] (size_t index) const;

		static Color White;
		static Color Black;
	};
#ifdef EQX_DEBUG
	void Print(const Color&);
#endif

	_ColorIntermediate operator* (const Color, const float);
	_ColorIntermediate operator* (const float, const Color);
	_ColorIntermediate operator/ (const Color, const float);
	Color LitColor(const Color, const Color);

	// Class handling value overflow for class Color
	class _ColorIntermediate
	{
		friend class Color;
	public:
		float r, g, b, a;

	public:
		_ColorIntermediate(Color);
		_ColorIntermediate(const _ColorIntermediate&);

		float& operator[] (size_t index);
		const float& operator[] (size_t index) const;
	};

	_ColorIntermediate operator* (const _ColorIntermediate, const float);
	_ColorIntermediate operator* (const float, const _ColorIntermediate);
	_ColorIntermediate operator/ (const _ColorIntermediate, const float);
	_ColorIntermediate operator+ (const Color c1, const Color c2);
	_ColorIntermediate operator+ (const _ColorIntermediate c1, const _ColorIntermediate c2);
	_ColorIntermediate operator+ (const Color c1, const _ColorIntermediate c2);
	_ColorIntermediate operator+ (const _ColorIntermediate c1, const Color c2);

	/**
	 * The lower-left corner is of coordinate (0, 0)
	 * x extends rightward; y extends upward
	 * One-dimensional array: y=0 x=0..width-1; y=1 x=width..2*width-1 ..
	 */
	class Image
	{
	private:
		unsigned int width, height;
		Color* canvas;

	public:
		Image();
		Image(const ImageGrey&);
		Image(unsigned int, unsigned int);
		Image(const Image&);
		~Image();

		Image& operator= (const Image&);
		Image& operator= (const ImageGrey&);

		void set(unsigned int, unsigned int, Color);
		Color get(unsigned int, unsigned int) const;

		void clear();
		void write(ImageType type = ImageType::TGA, std::string filename = "output");

		inline const Color* GetCanvas() const { return this->canvas; };

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }
	};

	// Mono-channel image
	class ImageGrey
	{
	private:
		unsigned int width, height;
		float* canvas;

	public:
		ImageGrey();
		ImageGrey(unsigned int, unsigned int);
		ImageGrey(const ImageGrey&) = delete;
		~ImageGrey();

		ImageGrey& operator= (const ImageGrey&);

		void set(unsigned int, unsigned int, float);
		float get(unsigned int, unsigned int) const;

		void clear();
		void write(ImageType type = ImageType::TGA, std::string filename = "output");

		inline bool IsPointOnCanvas(float x, float y) 
		{ 
			return 0 <= x && x < this->width && 0 <= y && y <= this->height; 
		}

		const float* GetCanvas() const { return this->canvas; };

		inline unsigned int getWidth() const { return width; }
		inline unsigned int getHeight() const { return height; }
	};

	Color blendColor(Color fore, Color back, float coeff);

	TGAColor toTGAColor(Color);

}
