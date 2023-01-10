#pragma once

#include "eqxpch.h"

#include "vendor/TGA/tgaimage.h"

namespace EQX {

	enum class ImageType
	{
		TGA,	// Relies on tgaimage.h
		PNG		// Relies on stb libraries
	};

	class Color
	{
	public:
		unsigned char r, g, b, a;

	public:
		Color();
		Color(float);
		Color(float, float, float, float);
		Color(Vector4&);
		Color(Vector3&);
		Color(const Color&);

		Color& operator= (const Color&);
		Color operator+ (Color&);
		unsigned char& operator[] (size_t index);
		const unsigned char& operator[] (size_t index) const;

		static Color White;
		static Color Black;
	};

	// Includes clamping beneath 255
	Color operator* (const Color, const float);
	Color operator* (const float, const Color);
	Color operator/ (const Color, const float);
	Color LitColor(const Color, const Color);


	/**
	 * The lower-left corner is of coordinate (0, 0)
	 * x extends rightward; y extends upward
	 * One-dimensional array: y=0 x=0..width-1; y=1 x=width..2*width-1 ..
	 */
	class Image
	{
	private:
		unsigned int width, height;
		ImageType type;
		Color* canvas;
		std::string filename;

	public:
		Image();
		Image(unsigned int, unsigned int, ImageType, std::string = "output");
		Image(const Image&) = delete;
		~Image();

		Image& operator= (const Image&);

		void set(unsigned int, unsigned int, Color);
		Color get(unsigned int, unsigned int) const;

		void clear();
		void write();

		inline unsigned int getWidth() { return width; }
		inline unsigned int getHeight() { return height; }
		inline ImageType getType() { return type; }
		inline void setType(ImageType t) { type = t; };
	};

	Color blendColor(Color fore, Color back, float coeff);

	TGAColor toTGAColor(Color);
}
