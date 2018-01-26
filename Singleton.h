#pragma once

// Using the design style of singleton mode.
// For any class inherited from this class, call getInstance() to get the single instance
#include <memory>

using namespace std;

class Garbo;

template <class T>
class Singleton {
private:
public:
	static T* getInstance() {
		static T m_Instance;
		return &m_Instance;
	}

};

