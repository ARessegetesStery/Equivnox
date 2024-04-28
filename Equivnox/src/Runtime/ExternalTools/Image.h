#pragma once

#include <CoreMinimal.h>

#include "vendor/TGA/tgaimage.h"

namespace EQX {

	class Color;
	class _ColorIntermediate;
	class Image;
	class ImageGrey;

	template <typename T, typename ZT, ZT Zero>
	class ImageBuffer;

	enum class ImageType
	{
		TGA,	// Relies on tgaimage.h
		PNG		// Relies on stb libraries
	};

	enum class RescaleFunc
	{
		GAUSSIAN,
		B_SPLINE_CUBIC,
		CR_CUBIC
	};

	enum class RescaleDepth
	{
		THIN,		// vertices of the square box
		THICK		// square of width 4
	};

	class Color
	{
		friend class _ColorIntermediate;
	public:
		XUChar r, g, b, a;

	public:
		Color();
		Color(XFloat);
		Color(XFloat, XFloat, XFloat, XFloat);
		Color(_ColorIntermediate);			// clamps at 0 and 255
		Color(Vector4&);
		Color(Vector3&);
		Color(const Color&);

		Color& operator= (const Color&);
		Color& operator+= (const Color&);
		XBool operator== (const Color&);
		XBool operator!= (const Color&);
		XUChar& operator[] (size_t index);
		const XUChar& operator[] (size_t index) const;

		static Color White;
		static Color Black;
	};
#ifdef EQX_DEBUG
	void Print(const Color&);
#endif

	_ColorIntermediate operator* (const Color, const XFloat);
	_ColorIntermediate operator* (const XFloat, const Color);
	_ColorIntermediate operator/ (const Color, const XFloat);
	Color LitColor(const Color, const Color);

	// Class handling value overflow for class Color
	class _ColorIntermediate
	{
		friend class Color;
	public:
		XFloat r, g, b, a;

	public:
		_ColorIntermediate(Color);
		_ColorIntermediate(const _ColorIntermediate&);

		XFloat& operator[] (size_t index);
		const XFloat& operator[] (size_t index) const;
	};

	_ColorIntermediate operator* (const _ColorIntermediate, const XFloat);
	_ColorIntermediate operator* (const XFloat, const _ColorIntermediate);
	_ColorIntermediate operator/ (const _ColorIntermediate, const XFloat);
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
		XUInt width, height;
		Color* canvas;

	public:
		Image();
		Image(const ImageGrey&);
		Image(XUInt, XUInt);
		Image(const Image&);
		~Image();

		Image& operator= (const Image&);
		Image& operator= (const ImageGrey&);

		void Set(XUInt, XUInt, Color);
		Color Get(XUInt, XUInt) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, XString filename = "output");

		void Rescale(XUInt, XUInt, RescaleFunc func = RescaleFunc::GAUSSIAN);

		inline const Color* GetCanvas() const { return this->canvas; };

		inline XUInt GetWidth() const { return width; }
		inline XUInt GetHeight() const { return height; }
	};

	// Mono-channel image
	class ImageGrey
	{
	private:
		XUInt width, height;
		XFloat* canvas;

	public:
		ImageGrey();
		ImageGrey(XUInt, XUInt);
		ImageGrey(const ImageGrey&) = delete;
		~ImageGrey();

		ImageGrey& operator= (const ImageGrey&);

		void Set(XUInt, XUInt, XFloat);
		XFloat Get(XUInt, XUInt) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, XString filename = "output");

		inline XBool IsPointOnCanvas(XFloat x, XFloat y) 
		{ 
			return 0 <= x && x < this->width && 0 <= y && y <= this->height; 
		}

		const XFloat* GetCanvas() const { return this->canvas; };

		inline XUInt GetWidth() const { return width; }
		inline XUInt GetHeight() const { return height; }
	};

	// TODO Upgrade to c++20 to integrae T and ZT
	//     In c++17 or lower it is invalid to pass argument of non-integer type

	/**
	 * @tparam T - the type of data that the canvas is to be filled with
	 * @tparam ZT - the type of data for zero element
	 * @tparam Zero - the Zero element for ZT, which should be transformable to Zero element for T
	 */
	template <typename T, typename ZT, ZT Zero>
	class ImageBuffer
	{
	private:
		XUInt width, height;
		T* canvas;

	public:
		ImageBuffer();
		ImageBuffer(const T& val);	// Initialize the image mask uniformly with {val}
		ImageBuffer(XUInt width, XUInt height);
		ImageBuffer(XUInt width, XUInt height, const T& val);
		~ImageBuffer();

		void SetAll(const T&);
		void Set(XUInt, XUInt, const T&);
		const T& Get(XUInt, XUInt) const;

		void Clear();
	};

	Color blendColor(Color fore, Color back, XFloat coeff);

	TGAColor toTGAColor(Color);

	template<typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer() : ImageBuffer<T, ZT, Zero>(Zero) {	}

	template <typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(const T& val)
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new T[100 * 100];
		for (XUInt i = 0; i != 100 * 100; ++i)
			this->canvas[i] = val;
	}

	/*  Implementation of Template Class {ImageMask}  */

	template<typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(XUInt width, XUInt height) :
		ImageBuffer<T, ZT, Zero>(width, height, Zero) {	}

	template <typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(XUInt width, XUInt height, const T& val)
	{
		this->width = width;
		this->height = height;
		this->canvas = new T[width * height];
		for (XUInt i = 0; i != width * height; ++i)
			this->canvas[i] = val;
	}

	template <typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::~ImageBuffer()
	{
		if (canvas)
			delete[] canvas;
	}

	template<typename T, typename ZT, ZT Zero>
	inline void ImageBuffer<T, ZT, Zero>::SetAll(const T& val)
	{
		for (XUInt i = 0; i != width * height; ++i)
			this->canvas[i] = val;
	}

	template <typename T, typename ZT, ZT Zero>
	void ImageBuffer<T, ZT, Zero>::Set(XUInt w, XUInt h, const T& val)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = val;
	}

	template <typename T, typename ZT, ZT Zero>
	const T& ImageBuffer<T, ZT, Zero>::Get(XUInt w, XUInt h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return Zero;
	}

	template <typename T, typename ZT, ZT Zero>
	void ImageBuffer<T, ZT, Zero>::Clear()
	{
		for (XUInt i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Zero;
	}

}
