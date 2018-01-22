#pragma once

// Using the design style of singleton mode.
// For any class inherited from this class, call getInstance() to get the single instance

template <class T>
class Singleton {
private:

protected:
	Singleton() {
		T();
	}
	static T* m_Instance;
public:
	static T* getInstance() {
		if (m_Instance == nullptr)
			m_Instance = new T();
		return m_Instance;
	}
};