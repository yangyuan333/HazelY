#pragma once
#include "Hazel/Core.h"
#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Buffer.h"

namespace Hazel {

	enum class TextureFormat
	{
		None = 0,
		RGB = 1,
		RGBA = 2,
	};

	/*
	*	1. �ڲ����ݸ�ʽ
	*	2. RenderID
	*/
	class Texture {
	public:
		virtual ~Texture() {}
		virtual RendererID GetRendererID() const = 0;
		virtual TextureFormat GetFormat() const = 0;
	};
	
	class Texture2D :public Texture {
	public:
		virtual ~Texture2D() {}
		virtual void Bind(unsigned int slot) = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
	public:
		/*
		*	1. �ļ�����ʱ���������������
		*	2. ���ഴ��ֻ�����ڴ棬Update�ٸ�������---��ͬAPI�в�ͬ�ķ��������漰���ӿ�����
		*/
		static Texture2D* Create(std::string const& path, bool srgb = true);
		static Texture2D* Create(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);
	};

	class TextureCubeMap :public Texture {
	public:
		virtual ~TextureCubeMap() {}
		virtual void Bind(unsigned int slot) = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;
	public:
		static Texture2D* Create(std::string const& path, bool srgb = true);
		static Texture2D* Create(TextureFormat format, unsigned int width, unsigned int height, bool srgb = false);
		
	};
}