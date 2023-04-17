#pragma once
#include "Hazel/Ref.h"
#include "Buffer.h"

namespace Hazel {

	enum class ImageFormat {

		None = 0,
		RGB,
		RGBA,
		RGBA16F,
		RGBA32F,
		RG32F,

		SRGB,

		DEPTH32F,
		DEPTH24STENCIL8,

		// Default
		Depth = DEPTH24STENCIL8

	};

	enum class TextureWrap
	{
		None = 0,
		Clamp,
		Repeat
	};

	enum class TextureFilter
	{
		None = 0,
		Linear,
		Nearest
	};

	enum class TextureType
	{
		None = 0,
		Texture2D,
		TextureCubeMap
	};

	/*
	* Texture image Type why not in this
	*/
	struct TextureProperties
	{
		TextureWrap SamplerWrap = TextureWrap::Repeat;
		TextureFilter SamplerFilter = TextureFilter::Linear;
		bool GenerateMips = true;
		bool SRGB = false; // why has it?
	};

	/*
	* 类似多封装了一层数据，Texture的格式和Warp等信息都封存到了这个Image类中
	*/
	class Image :public RefCounted {
	public:
		virtual ~Image() {}

		virtual void Invalidate() = 0;
		virtual void Release() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual ImageFormat GetFormat() const = 0;

		virtual Buffer GetBuffer() const = 0;
		virtual Buffer& GetBuffer() = 0;

		virtual uint64_t GetHash() const = 0;
	};

	class Image2D :public Image {
		static Ref<Image2D> Create(ImageFormat format, uint32_t width, uint32_t height, Buffer buffer);
		static Ref<Image2D> Create(ImageFormat format, uint32_t width, uint32_t height, const void* data = nullptr);
	};

	namespace Utils {

		inline uint32_t GetImageFormatBPP(ImageFormat format)
		{
			switch (format)
			{
			case ImageFormat::RGB:
			case ImageFormat::SRGB:    return 3;
			case ImageFormat::RGBA:    return 4;
			case ImageFormat::RGBA16F: return 2 * 4;
			case ImageFormat::RGBA32F: return 4 * 4;
			}
			HZ_CORE_ASSERT(false, "Unknown ImageFormat");
			return 0;
		}

		inline uint32_t CalculateMipCount(uint32_t width, uint32_t height)
		{
			return std::floor(std::log2(glm::min(width, height))) + 1;
		}

		inline uint32_t GetImageMemorySize(ImageFormat format, uint32_t width, uint32_t height)
		{
			return width * height * GetImageFormatBPP(format);
		}

	}

}