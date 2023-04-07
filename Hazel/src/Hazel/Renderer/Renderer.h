#pragma once

#include "RenderCommandQueue.h"
#include "VertexArray.h"

namespace Hazel {

	/*
	*	Renderer�����û������ڶ�������װ
	*	�û�ָ���������ܡ�������Renderer�ڲ���ָ�����Commandת��������ӽ�CommandQueue��
	*	Commandλ����ָ�������Ҫ������ת�����������CommandQueueʹ�ã��ڲ����RenderAPI���о����ʵ��
	*/
	/*
	*	����಻��Ҫ֪��API�������Ϣ
	*/
	class Renderer {
	public:
		typedef void(*RenderCommandFn)(void*);
		static void Init();
		// ���ĺ���
		static void Clear(float r, float g, float b, float a);
		static void DrawIndexed(VertexArray* vertexArray, bool depthTest);
		static void WaitAndRender();
	public:
		static void* Submit(RenderCommandFn fn, unsigned int size) {
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}
	public:
		inline static Renderer* GetRenderer() { return s_Instance; }
	private:
		static Renderer* s_Instance;
		RenderCommandQueue m_CommandQueue; // ����Ϊʲô����Ƴɾ�̬���أ�// ��Ϊ����౾����α����
	};

}

#define HZ_RENDER_PASTE2(a, b) a ## b
#define HZ_RENDER_PASTE(a, b) HZ_RENDER_PASTE2(a, b)
#define HZ_RENDER_UNIQUE(x) HZ_RENDER_PASTE(x, __LINE__)

#define HZ_RENDER(code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)();\
	}\

#define HZ_RENDER_1(arg0, code) \
	do {\
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0);\
	} } while(0)

#define HZ_RENDER_2(arg0, arg1, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1);\
	}\

#define HZ_RENDER_3(arg0, arg1, arg2, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2);\
	}\

#define HZ_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define HZ_RENDER_5(arg0, arg1, arg2, arg3, arg4, code) \
    struct HZ_RENDER_UNIQUE(HZRenderCommand) \
    {\
		HZ_RENDER_UNIQUE(HZRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg4)>::type>::type arg4)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3), arg4(arg4) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg3;\
			auto& arg4 = ((HZ_RENDER_UNIQUE(HZRenderCommand)*)argBuffer)->arg4;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg4)>::type>::type arg4;\
    };\
	{\
		auto mem = Renderer::Submit(HZ_RENDER_UNIQUE(HZRenderCommand)::Execute, sizeof(HZ_RENDER_UNIQUE(HZRenderCommand)));\
		new (mem) HZ_RENDER_UNIQUE(HZRenderCommand)(arg0, arg1, arg2, arg3, arg4);\
	}

#define HZ_RENDER_S(code) auto self = this;\
	HZ_RENDER_1(self, code)

#define HZ_RENDER_S1(arg0, code) auto self = this;\
	HZ_RENDER_2(self, arg0, code)

#define HZ_RENDER_S2(arg0, arg1, code) auto self = this;\
	HZ_RENDER_3(self, arg0, arg1, code)

#define HZ_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	HZ_RENDER_4(self, arg0, arg1, arg2, code)

#define HZ_RENDER_S4(arg0, arg1, arg2, arg3, code) auto self = this;\
	HZ_RENDER_5(self, arg0, arg1, arg2, arg3, code)