/*
#pragma once
#include "Hazel/Core.h"
#include <stdint.h>
#include <utility>

namespace Hazel {

	class RefCounted {
	public:
		void IncRefCount() const
		{
			m_RefCount++;
		}
		void DecRefCount() const
		{
			m_RefCount--;
		}
		uint32_t GetRefCount() const 
		{
			return m_RefCount; 
		}
	private:
		mutable uint32_t m_RefCount = 0; // TODO: atomic
	};

	template <class T>
	class Ref {
	public:
		friend class RefCounted;
		template <class T2>
		friend class Ref;
	public:
		Ref()
			:m_Instance(nullptr) {

		}
		Ref(T* instance)
			:m_Instance(instance) {
			
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");

			IncRef();
		}
		// it is a class, not a pointor
		Ref(std::nullptr_t)
			:m_Instance(nullptr) {

		}
		template <class T2>
		Ref(T2* instance) {

			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");
			static_assert(std::is_base_of<RefCounted, T2>::value, "Class is not RefCounted!");

			m_Instance = (T*)instance;
			IncRef();
		}

		Ref(const Ref<T>& other) {
			m_Instance = other.m_Instance;
			IncRef();
		}
		Ref(Ref<T>&& other) {
			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
		}
		template <class T2>
		Ref(const Ref<T2>& other) {
			m_Instance = (T*)other.m_Instance;
			IncRef();
		}
		template <class T2>
		Ref(Ref<T2>&& other) {
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
		}

		Ref<T>& operator=(const Ref<T>& other) {
			DecRef();
			m_Instance = other.m_Instance;
			IncRef();
			return *this;
		}
		Ref<T>& operator=(Ref<T>&& other) {
			DecRef();
			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}
		template <class T2>
		Ref<T>& operator=(const Ref<T2>& other) {
			DecRef();
			m_Instance = (T*)other.m_Instance;
			IncRef();
			return *this;
		}
		template <class T2>
		Ref<T>& operator=(Ref<T2>&& other) {
			DecRef();
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}
		Ref<T>& operator=(std::nullptr_t)
		{
			DecRef();
			m_Instance = nullptr;
			return *this;
		}

		virtual ~Ref() {
			DecRef();
		}

		T* operator->() {
			return m_Instance;
		}
		const T* operator->() const {
			return m_Instance;
		}
		T& operator*() {
			return *m_Instance;
		}
		const T& operator*() const {
			return *m_Instance;
		}
		T* Raw() {
			return m_Instance;
		}
		const T* Raw() const {
			return m_Instance;
		}

		void Reset(T* instance = nullptr) {
			DecRef();
			m_Instance = instance;
			IncRef();
		}

		template<typename T2>
		Ref<T2> As() const
		{
			return Ref<T2>(*this);
		}

		template<typename... Args>
		static Ref<T> Create(Args&&... args) {
			return Ref<T>(new T(std::forward<Args>(args)...));
		}

		operator bool() { return m_Instance != nullptr; }
		operator bool() const { return m_Instance != nullptr; }

	private:

		void IncRef() const {
			if (m_Instance)
				m_Instance->IncRefCount();
		}
		void DecRef() const {
			if (m_Instance) {
				m_Instance->DecRefCount();
				if (m_Instance->GetRefCount() == 0){
					delete m_Instance;
					// m_Instance = nullptr; 不能在这里赋值，因为会不符合const要求
				}
			}
		}

		T* m_Instance;
	};

}
*/

#pragma once

#include <stdint.h>

namespace Hazel {

	class RefCounted
	{
	public:
		void IncRefCount() const
		{
			m_RefCount++;
		}
		void DecRefCount() const
		{
			m_RefCount--;
		}

		uint32_t GetRefCount() const { return m_RefCount; }
	private:
		mutable uint32_t m_RefCount = 0; // TODO: atomic
	};

	template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_Instance(nullptr)
		{
		}

		Ref(std::nullptr_t n)
			: m_Instance(nullptr)
		{
		}

		Ref(T* instance)
			: m_Instance(instance)
		{
			static_assert(std::is_base_of<RefCounted, T>::value, "Class is not RefCounted!");

			IncRef();
		}

		template<typename T2>
		Ref(const Ref<T2>& other)
		{
			m_Instance = (T*)other.m_Instance;
			IncRef();
		}

		template<typename T2>
		Ref(Ref<T2>&& other)
		{
			m_Instance = (T*)other.m_Instance;
			other.m_Instance = nullptr;
		}

		~Ref()
		{
			DecRef();
		}

		Ref(const Ref<T>& other)
			: m_Instance(other.m_Instance)
		{
			IncRef();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecRef();
			m_Instance = nullptr;
			return *this;
		}

		Ref& operator=(const Ref<T>& other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		Ref& operator=(const Ref<T2>& other)
		{
			other.IncRef();
			DecRef();

			m_Instance = other.m_Instance;
			return *this;
		}

		template<typename T2>
		Ref& operator=(Ref<T2>&& other)
		{
			DecRef();

			m_Instance = other.m_Instance;
			other.m_Instance = nullptr;
			return *this;
		}

		operator bool() { return m_Instance != nullptr; }
		operator bool() const { return m_Instance != nullptr; }

		T* operator->() { return m_Instance; }
		const T* operator->() const { return m_Instance; }

		T& operator*() { return *m_Instance; }
		const T& operator*() const { return *m_Instance; }

		T* Raw() { return  m_Instance; }
		const T* Raw() const { return  m_Instance; }

		void Reset(T* instance = nullptr)
		{
			DecRef();
			m_Instance = instance;
		}

		template<typename T2>
		Ref<T2> As() const
		{
			return Ref<T2>(*this);
		}

		template<typename... Args>
		static Ref<T> Create(Args&&... args)
		{
			return Ref<T>(new T(std::forward<Args>(args)...));
		}

		bool operator==(const Ref<T>& other) const
		{
			return m_Instance == other.m_Instance;
		}

		bool operator!=(const Ref<T>& other) const
		{
			return !(*this == other);
		}

		bool EqualsObject(const Ref<T>& other)
		{
			if (!m_Instance || !other.m_Instance)
				return false;

			return *m_Instance == *other.m_Instance;
		}
	private:
		void IncRef() const
		{
			if (m_Instance)
				m_Instance->IncRefCount();
		}

		void DecRef() const
		{
			if (m_Instance)
			{
				m_Instance->DecRefCount();
				if (m_Instance->GetRefCount() == 0)
				{
					delete m_Instance;
				}
			}
		}

		template<class T2>
		friend class Ref;
		T* m_Instance;
	};

	// TODO: WeakRef

}
