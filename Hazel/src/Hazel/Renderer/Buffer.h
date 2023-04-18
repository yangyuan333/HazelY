#pragma once

#include <cstdint>

#include "Hazel/Core.h"
#include "Hazel/Log.h"


// 还没封装完吧
// 顶点属性的记录分配不封装嘛？
// VAO这种不封装？
// 那这样封装有什么意义呢？opengl的代码还是散在外面
// Vertex attribute完成了封装
namespace Hazel {

	struct Buffer {
	public:
		Buffer()
			: Data(nullptr), Size(0)
		{
		}

		Buffer(void* data, uint32_t size)
			: Data(data), Size(size)
		{
		}

		/*
		* Buffer不要自定义析构，防止数据丢失
		*/
		//virtual ~Buffer() {
		//	Release();
		//}

		static Buffer Copy(const void* data, uint32_t size) {
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}

		void Allocate(uint32_t size) {
			delete[] Data;
			Data = nullptr;
			if (size == 0)
				return;
			Data = new uint8_t[size];
			Size = size;
		}

		void Release() {
			delete[] Data;
			Data = nullptr;
			Size = 0;
		}

		void ZeroInitialize() {
			if (Data) {
				memset(Data, 0, Size);
			}
		}

		template<typename T>
		T& Read(uint32_t offset = 0) {
			return *((T*)Data + offset);
		}

		uint8_t* ReadBytes(uint32_t size, uint32_t offset) {
			HZ_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
			uint8_t* buffer = new uint8_t[size];
			memcpy(buffer, (uint8_t*)Data + offset, size);
			return buffer;
		}

		void Write(void* data, uint32_t size, uint32_t offset = 0) {
			HZ_CORE_ASSERT(offset + size <= Size, "Buffer overflow!");
			memcpy((uint8_t*)data, (uint8_t*)Data + offset, size);
		}

		operator bool() const {
			return Data;
		}

		uint8_t& operator[](int index) {
			return *((uint8_t*)Data + index);
		}

		uint8_t operator[](int index) const {
			return *((uint8_t*)Data + index);
		}

		template<typename T>
		T* As() {
			return (T*)Data;
		}

		inline uint32_t GetSize() const { return Size; }

	public:
		void* Data;
		uint32_t Size;

	};

	/*
	enum class ShaderDataType {
		None = 0,
		Float, Float2, Float3, Float4,
		Mat3, Mat4,
		Int, Int2, Int3, Int4,
		Bool,
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type)
		{
		case Hazel::ShaderDataType::Float:
			return 4;
		case Hazel::ShaderDataType::Float2:
			return 4 * 2;
		case Hazel::ShaderDataType::Float3:
			return 4 * 3;
		case Hazel::ShaderDataType::Float4:
			return 4 * 4;
		case Hazel::ShaderDataType::Mat3:
			return 4 * 3 * 3;
		case Hazel::ShaderDataType::Mat4:
			return 4 * 4 * 4;
		case Hazel::ShaderDataType::Int:
			return 4;
		case Hazel::ShaderDataType::Int2:
			return 4 * 2;
		case Hazel::ShaderDataType::Int3:
			return 4 * 3;
		case Hazel::ShaderDataType::Int4:
			return 4 * 4;
		case Hazel::ShaderDataType::Bool:
			return 1;
		}
		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
	}

	struct BufferElements {
		ShaderDataType Type;
		std::string Name;
		uint32_t Size;
		uint32_t Offset;
		bool Nomalized;

		BufferElements() {}
		BufferElements(ShaderDataType type, const std::string& name, bool normalized = false)
			:Type(type), Name(name), Size(ShaderDataTypeSize(type)), Offset(0), Nomalized(normalized)
		{

		}

		uint32_t GetComponentCount()const {
			switch (Type)
			{
			case Hazel::ShaderDataType::Float:
				return 1;
			case Hazel::ShaderDataType::Float2:
				return 2;
			case Hazel::ShaderDataType::Float3:
				return 3;
			case Hazel::ShaderDataType::Float4:
				return 4;
			case Hazel::ShaderDataType::Mat3:
				return 3*3;
			case Hazel::ShaderDataType::Mat4:
				return 4*4;
			case Hazel::ShaderDataType::Int:
				return 1;
			case Hazel::ShaderDataType::Int2:
				return 2;
			case Hazel::ShaderDataType::Int3:
				return 3;
			case Hazel::ShaderDataType::Int4:
				return 4;
			case Hazel::ShaderDataType::Bool:
				return 1;
			}
			HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		}

	};

	class BufferLayout {
	public:
		// 为什么需要一个默认构造函数
		// 因为他可能作为其他类的一个成员变量---并且没有初值
		BufferLayout() {}
		BufferLayout(std::initializer_list<BufferElements> elements)
			:m_Elements(elements)
		{
			CalculateOffsetsAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElements>& GetElements() const { return m_Elements; }

		std::vector<BufferElements>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElements>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElements>::const_iterator begin() const{ return m_Elements.begin(); }
		std::vector<BufferElements>::const_iterator end() const{ return m_Elements.end(); }

	private:
		void CalculateOffsetsAndStride() {
			uint32_t offset = 0;
			m_Stride = 0;
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
	private:
		std::vector<BufferElements> m_Elements;
		uint32_t m_Stride{ 0 };
	};

	class VertexBuffer {
	public:
		virtual ~VertexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetBufferLayout() const = 0;

		static VertexBuffer* Create(float* vertices, uint32_t size);
	};
	class IndexBuffer {
	public:
		virtual ~IndexBuffer() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t count);
	};
	*/
}