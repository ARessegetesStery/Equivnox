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
		EQXUChar r, g, b, a;

	public:
		Color();
		Color(EQXFloat);
		Color(EQXFloat, EQXFloat, EQXFloat, EQXFloat);
		Color(_ColorIntermediate);			// clamps at 0 and 255
		Color(Vector4&);
		Color(Vector3&);
		Color(const Color&);

		Color& operator= (const Color&);
		Color& operator+= (const Color&);
		bool operator== (const Color&);
		bool operator!= (const Color&);
		EQXUChar& operator[] (size_t index);
		const EQXUChar& operator[] (size_t index) const;

		static Color White;
		static Color Black;
	};
#ifdef EQX_DEBUG
	void Print(const Color&);
#endif

	_ColorIntermediate operator* (const Color, const EQXFloat);
	_ColorIntermediate operator* (const EQXFloat, const Color);
	_ColorIntermediate operator/ (const Color, const EQXFloat);
	Color LitColor(const Color, const Color);

	// Class handling value overflow for class Color
	class _ColorIntermediate
	{
		friend class Color;
	public:
		EQXFloat r, g, b, a;

	public:
		_ColorIntermediate(Color);
		_ColorIntermediate(const _ColorIntermediate&);

		EQXFloat& operator[] (size_t index);
		const EQXFloat& operator[] (size_t index) const;
	};

	_ColorIntermediate operator* (const _ColorIntermediate, const EQXFloat);
	_ColorIntermediate operator* (const EQXFloat, const _ColorIntermediate);
	_ColorIntermediate operator/ (const _ColorIntermediate, const EQXFloat);
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
		EQXUInt width, height;
		Color* canvas;

	public:
		Image();
		Image(const ImageGrey&);
		Image(EQXUInt, EQXUInt);
		Image(const Image&);
		~Image();

		Image& operator= (const Image&);
		Image& operator= (const ImageGrey&);

		void Set(EQXUInt, EQXUInt, Color);
		Color Get(EQXUInt, EQXUInt) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, EQXString filename = "output");

		void Rescale(EQXUInt, EQXUInt, RescaleFunc func = RescaleFunc::GAUSSIAN);

		inline const Color* GetCanvas() const { return this->canvas; };

		inline EQXUInt GetWidth() const { return width; }
		inline EQXUInt GetHeight() const { return height; }
	};

	// Mono-channel image
	class ImageGrey
	{
	private:
		EQXUInt width, height;
		EQXFloat* canvas;

	public:
		ImageGrey();
		ImageGrey(EQXUInt, EQXUInt);
		ImageGrey(const ImageGrey&) = delete;
		~ImageGrey();

		ImageGrey& operator= (const ImageGrey&);

		void Set(EQXUInt, EQXUInt, EQXFloat);
		EQXFloat Get(EQXUInt, EQXUInt) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, EQXString filename = "output");

		inline bool IsPointOnCanvas(EQXFloat x, EQXFloat y) 
		{ 
			return 0 <= x && x < this->width && 0 <= y && y <= this->height; 
		}

		const EQXFloat* GetCanvas() const { return this->canvas; };

		inline EQXUInt GetWidth() const { return width; }
		inline EQXUInt GetHeight() const { return height; }
	};

	/**
	 * @tparam T - the type of data that the canvas is to be filled with
	 * @tparam ZT - the type of data for zero element
	 * @tparam Zero - the Zero element for ZT, which should be transformable to Zero element for T
	 */
	template <typename T, typename ZT, ZT Zero>
	class ImageBuffer
	{
	private:
		EQXUInt width, height;
		T* canvas;

	public:
		ImageBuffer();
		ImageBuffer(const T& val);	// Initialize the image mask uniformly with {val}
		ImageBuffer(EQXUInt width, EQXUInt height);
		ImageBuffer(EQXUInt width, EQXUInt height, const T& val);
		~ImageBuffer();

		void SetAll(const T&);
		void Set(EQXUInt, EQXUInt, const T&);
		const T& Get(EQXUInt, EQXUInt) const;

		void Clear();
	};

	Color blendColor(Color fore, Color back, EQXFloat coeff);

	TGAColor toTGAColor(Color);

	template<typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer() : ImageBuffer<T, ZT, Zero>(Zero) {	}

	template <typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(const T& val)
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new T[100 * 100];
		for (EQXUInt i = 0; i != 100 * 100; ++i)
			this->canvas[i] = val;
	}

	/*  Implementation of Template Class {ImageMask}  */

	template<typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(EQXUInt width, EQXUInt height) :
		ImageBuffer<T, ZT, Zero>(width, height, Zero) {	}

	template <typename T, typename ZT, ZT Zero>
	ImageBuffer<T, ZT, Zero>::ImageBuffer(EQXUInt width, EQXUInt height, const T& val)
	{
		this->width = width;
		this->height = height;
		this->canvas = new T[width * height];
		for (EQXUInt i = 0; i != width * height; ++i)
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
		for (EQXUInt i = 0; i != width * height; ++i)
			this->canvas[i] = val;
	}

	template <typename T, typename ZT, ZT Zero>
	void ImageBuffer<T, ZT, Zero>::Set(EQXUInt w, EQXUInt h, const T& val)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = val;
	}

	template <typename T, typename ZT, ZT Zero>
	const T& ImageBuffer<T, ZT, Zero>::Get(EQXUInt w, EQXUInt h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return Zero;
	}

	template <typename T, typename ZT, ZT Zero>
	void ImageBuffer<T, ZT, Zero>::Clear()
	{
		for (EQXUInt i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Zero;
	}

}
