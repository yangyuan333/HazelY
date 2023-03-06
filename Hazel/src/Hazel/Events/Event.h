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

// 利用宏进行函数定义，简化书写
// 并且这里都是定义的virtual function，目的就是为了实现类别多态，很妙
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

	// 三种修饰符的作用喝区别还是要搞清楚
	protected:
		bool m_Handled = false;
	};

	// 这里为什么不给每个类型的事件加一个处理函数呢？实现多态效果。
	// 有一种解释就是，每种事件的处理可能也有多种选择，通过这种传递函数的方式更加灵活
	class EventDispatcher {
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event) :m_Event(event) {}
		template <typename T>
		bool Dispatch(EventFn<T> func) {
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.m_Handled = func(*(T*)&m_Event); // 查一下这里为什么不用 dynamic_cast
				return true;
			}
			return false;
		}
	private:
		Event& m_Event; // 这相当于是外部变量，const作用不到它身上
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}

}