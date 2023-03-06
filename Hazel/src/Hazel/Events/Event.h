#pragma once

#include "Hazel/Core.h"
#include <string>
#include <iostream>
#include <functional>

namespace Hazel {

	enum class EventType {
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

// ���ú���к������壬����д
// �������ﶼ�Ƕ����virtual function��Ŀ�ľ���Ϊ��ʵ������̬������
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType(){return EventType::type;}\
								virtual EventType GetEventType()const override{return GetStaticType();}\
								virtual const char* GetName() const override {return #type;}

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override{return category;}

	class HAZEL_API Event {

		friend class EventDispatcher;
	public:
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category) {
			return GetCategoryFlags() & category;
		}

	// �������η������ú�������Ҫ�����
	protected:
		bool m_Handled = false;
	};

	// ����Ϊʲô����ÿ�����͵��¼���һ�����������أ�ʵ�ֶ�̬Ч����
	// ��һ�ֽ��;��ǣ�ÿ���¼��Ĵ�������Ҳ�ж���ѡ��ͨ�����ִ��ݺ����ķ�ʽ�������
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event) {}
		template <typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event); // ��һ������Ϊʲô���� dynamic_cast
				return true;
			}
			return false;
		}
	private:
		Event& m_Event; // ���൱�����ⲿ������const���ò���������
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}