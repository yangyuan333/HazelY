#pragma once

/*
* .h�ļ���������һ��ת����Ԫ
* �����������Ŀû��include���.h�ļ�����������ǲ���������д����
*/

#ifdef HZ_PLATFORM_WINDOWS

// �������Ҳ�ǿ�Ҫ�ɲ�Ҫ�ģ���Ϊ���ͷ�ļ����ջᱻinclude��sandbox��ִ��

extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_WARN("Initialized Log!");
	HZ_INFO("Hello!");
	// ����Ϊʲô����ֱ�����أ�
	// ��Ϊ���.h�ļ����ᱻ������include��.cpp��
	// ����ͨ��Hazel.h�����include��.cpp�е�
	// ����Hazel.h�оͶ�����Application������Ϣ��ͬʱ.cpp��Ҳ�����˺�������
	// ��������dll�������ԭ������ڣ�.h���ᱻ����
	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif