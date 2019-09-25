#ifndef service_h
#define service_h

#include <memory>
#include <vector>

class command_manager;
//	getInstacne returns reference with from c++11 (it's safe)
//	Default constructor is private
//	Copy/Move constructor/assignment operator is not supported
template <typename T>
class Service
{
public:
	template <typename U>
	class Dependency;

	template <>
	class Dependency<command_manager>
	{
	public:
	};


	static T& getInstance()
	{
		if (m_instance == nullptr)
			m_instance = std::unique_ptr<T>(new T);
		return *m_instance;
	}

	template <typename U>
	static void registerDependecies()
	{
		if (std::is_base_of<Service<U>, U>())
			Service<U>::getInstance();
	}

	template <>
	static void registerDependecies<Service>()
	{
		//if constexpr (std::is_same<U, Service>::value)
	}

	virtual ~Service() = default;

public:
	//virtual void startUp() = 0;
	//virtual void shutDown() = 0;

protected:
	Service() = default;
	Service(const Service&) = delete;
	Service(const Service&&) = delete;
	Service& operator=(const Service&) = delete;
	Service& operator=(const Service&&) = delete;

private:
	static std::unique_ptr<T> m_instance;
};

template <typename T>
std::unique_ptr<T> Service<T>::m_instance = nullptr;

//class ServiceManager : public Service<ServiceManager>
//{
//public:
//	void startUp();
//	void shutDown();
//};

#endif
