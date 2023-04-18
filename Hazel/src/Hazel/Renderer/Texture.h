#pragma once
#include "Hazel/Core.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Asset/Asset.h"
#include "Hazel/Ref.h"

namespace Hazel {

	/*
	*	1. 内部数据格式
	*	2. RenderID
	*/
	class Texture:public Asset {
	public:
		virtual ~Texture() {}

		virtual void Bind(uint32_t slot = 0) const = 0;

		virtual ImageFormat GetFormat() const = 0;
		virtual TextureType GetType() const = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetMipLevelCount() const = 0;

		virtual uint64_t GetHash() const = 0;
	};
	
	class Texture2D :public Texture {
	public:
		virtual ~Texture2D() {}

		virtual Ref<Image2D> GetImage() const = 0;
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
		virtual Buffer GetWriteableBuffer() = 0;
		virtual bool Loaded() const = 0;
		virtual const std::string& GetPath() const = 0;

		virtual TextureType GetType() const override { return TextureType::Texture2D; }
	public:
		/*
		*	1. 文件创建时采用数据立马更新
		*	2. 其余创建只构建内存，Update再更新数据---不同API有不同的方案，不涉及到接口虚类
		*/
		static Ref<Texture2D> Create(const std::string& path, TextureProperties properties = TextureProperties());
		static Ref<Texture2D> Create(ImageFormat format, uint32_t width, uint32_t height, const void* data = nullptr, TextureProperties properties = TextureProperties());
	};

	class TextureCubeMap :public Texture {
	public:
		virtual ~TextureCubeMap() {}

		virtual const std::string& GetPath() const = 0;
		virtual TextureType GetType() const override { return TextureType::TextureCubeMap; }
	public:
		static Ref<TextureCubeMap> Create(ImageFormat format, uint32_t width, uint32_t height, const void* data = nullptr, TextureProperties properties = TextureProperties());
		static Ref<TextureCubeMap> Create(const std::string& path, TextureProperties properties = TextureProperties());
		
	};
}