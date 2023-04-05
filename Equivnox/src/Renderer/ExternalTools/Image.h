#pragma once

#include "eqxpch.h"

#include "vendor/TGA/tgaimage.h"

namespace EQX {

	class Color;
	class _ColorIntermediate;
	class Image;
	class ImageGrey;

	template <typename T, T Zero>
	class ImageMask;

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
		bool operator== (const Color&);
		bool operator!= (const Color&);
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

		void Set(unsigned int, unsigned int, Color);
		Color Get(unsigned int, unsigned int) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, std::string filename = "output");

		void Rescale(unsigned int, unsigned int, RescaleFunc func = RescaleFunc::GAUSSIAN);

		inline const Color* GetCanvas() const { return this->canvas; };

		inline unsigned int GetWidth() const { return width; }
		inline unsigned int GetHeight() const { return height; }
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

		void Set(unsigned int, unsigned int, float);
		float Get(unsigned int, unsigned int) const;

		void Clear();
		void Write(ImageType type = ImageType::TGA, std::string filename = "output");

		inline bool IsPointOnCanvas(float x, float y) 
		{ 
			return 0 <= x && x < this->width && 0 <= y && y <= this->height; 
		}

		const float* GetCanvas() const { return this->canvas; };

		inline unsigned int GetWidth() const { return width; }
		inline unsigned int GetHeight() const { return height; }
	};

	/**
	 * @tparam T - the type of data that the canvas is to be filled with
	 * @tparam Zero - the Zero element for T
	 */
	template <typename T, T Zero>
	class ImageMask
	{
	private:
		unsigned int width, height;
		T* canvas;

	public:
		ImageMask();
		ImageMask(const T& val);	// Initialize the image mask uniformly with {val}
		ImageMask(unsigned int width, unsigned int height);
		ImageMask(unsigned int width, unsigned int height, const T& val);
		~ImageMask();

		void Set(unsigned int, unsigned int, const T&);
		const T& Get(unsigned int, unsigned int) const;

		void Clear();
	};

	Color blendColor(Color fore, Color back, float coeff);

	TGAColor toTGAColor(Color);

	template<typename T, T Zero>
	ImageMask<T, Zero>::ImageMask() : ImageMask<T, Zero>(Zero) {	}

	// TODO add the zero value for type T to the parameters of this class
	template <typename T, T Zero>
	ImageMask<T, Zero>::ImageMask(const T& val)
	{
		this->width = 100;
		this->height = 100;
		this->canvas = new T[100 * 100];
		for (unsigned int i = 0; i != 100 * 100; ++i)
			this->canvas[i] = val;
	}

	/*  Implementation of Template Class {ImageMask}  */

	template<typename T, T Zero>
	ImageMask<T, Zero>::ImageMask(unsigned int width, unsigned int height) :
		ImageMask<T, Zero>(width, height, Zero) {	}

	template <typename T, T Zero>
	ImageMask<T, Zero>::ImageMask(unsigned int width, unsigned int height, const T& val)
	{
		this->width = width;
		this->height = height;
		this->canvas = new T[width * height];
		for (unsigned int i = 0; i != width * height; ++i)
			this->canvas[i] = val;
	}

	template <typename T, T Zero>
	ImageMask<T, Zero>::~ImageMask()
	{
		if (canvas)
			delete[] canvas;
	}

	template <typename T, T Zero>
	void ImageMask<T, Zero>::Set(unsigned int w, unsigned int h, const T& val)
	{
		if (!(!canvas || w >= width || h >= height))
			this->canvas[(size_t)(h * this->width + w)] = val;
	}

	template <typename T, T Zero>
	const T& ImageMask<T, Zero>::Get(unsigned int w, unsigned int h) const
	{
		if (!(!canvas || w >= width || h >= height))
			return this->canvas[(size_t)(h * this->width + w)];
		return Zero;
	}

	template <typename T, T Zero>
	void ImageMask<T, Zero>::Clear()
	{
		for (unsigned int i = 0; i != this->width * this->height; ++i)
			this->canvas[i] = Zero;
	}

}
