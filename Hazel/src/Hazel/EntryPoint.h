#pragma once

/*
* .h文件自身不构成一个转换单元
* 因此如果这个项目没有include这个.h文件，则编译器是不会对它进行处理的
*/

#ifdef HZ_PLATFORM_WINDOWS

// 这个声明也是可要可不要的，因为这个头文件最终会被include到sandbox中执行

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	HZ_INFO("Hello!");
	// 这里为什么可以直接用呢？
	// 因为这个.h文件不会被单独的include到.cpp中
	// 它是通过Hazel.h，间接include到.cpp中的
	// 而在Hazel.h中就定义了Application的类信息，同时.cpp中也定义了函数定义
	// 单独编译dll不报错的原因就在于，.h不会被编译
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif