#ifndef Templates_h__
#define Templates_h__
#include "pch.h"


namespace Engine
{
	namespace Singleton
	{
		template<typename T>
		class Singleton
		{
		protected:
			Singleton() {}
			virtual ~Singleton() {}
			Singleton(const Singleton& s) {	}
			Singleton& operator = (const Singleton&);
		private:
			static void Destroy()
			{
				if (m_pInstance != nullptr)
					delete m_pInstance;
			}
		private:
			static T* GetInstance()
			{
				if (m_pInstance = nullptr)
				{
					m_pInstance = new T();
					atexit(Destroy);
				}
				return m_pInstance;
			}
		private:
			static T* m_pInstance;
		};

		template <typename T> T* Singleton <T>::m_pInstance;

		template<typename T>
		class CSingleton
		{
		private:
			static std::shared_ptr<T> m_pInstance;
			static std::once_flag m_onceFlag;
		public:
			static T* GetInstance()
			{
				call_once(m_onceFlag, [] {m_pInstance.reset(new T);
				});
				atexit(Destroy);
				return m_pInstance.get();
			}
		private:
			static void Destroy()
			{
				m_pInstance.reset();
			}

		};
		template<typename T> std::shared_ptr<T> CSingleton<T>::m_pInstance = nullptr;
		template<typename T> std::once_flag CSingleton<T>::m_onceFlag;


	}
}




#endif // Templates_h__
