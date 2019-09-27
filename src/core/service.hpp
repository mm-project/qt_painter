#ifndef service_h
#define service_h

#include <memory>
#include <vector>
#include <functional>

//	Interface for working with servies 
class Singleton
{
public:
	virtual ~Singleton() {}

public:
	// some servies may be don't need this
	virtual void startUp() {}
	virtual void shutDown() {}

protected:
	// can't copy
	Singleton() = default;
	Singleton(const Singleton&) = delete;
	Singleton(const Singleton&&) = delete;
	Singleton& operator=(const Singleton&) = delete;
	Singleton& operator=(const Singleton&&) = delete;
};

class ServiceManager;

//	getInstacne returns reference with from c++11 (it's safe)
//	Default constructor is private
//	Copy/Move constructor/assignment operator is not supported
template <typename T>
class Service : public Singleton
{
public:
	using ServicePtr = std::unique_ptr<T>;

public:

	static T& getInstance()
	{
		if (m_instance == nullptr)
		{
			// first need to register deps
			registerDependecies();
			m_instance = std::unique_ptr<T>(new T);
			ServiceManager::getInstance().addService(*m_instance);
		}
		return *m_instance;
	}

	static void registerDependecies() {}

	template <typename U>
	static void addDependecy()
	{
		if (std::is_base_of<Service<U>, U>())
			Service<U>::getInstance();
	}

public:
	virtual ~Service() = default;

protected:
	Service() = default;
	Service(const Service&) = delete;
	Service(const Service&&) = delete;
	Service& operator=(const Service&) = delete;
	Service& operator=(const Service&&) = delete;

private:
	static ServicePtr m_instance;
};

template <typename T>
typename Service<T>::ServicePtr Service<T>::m_instance = nullptr;

class ServiceManager : public Service<ServiceManager>
{
public:
	//	Things needed for main application
	void startUp() override;
	//	Delete content in order of registering
	void shutDown() override;

	template <typename T>
	void addService(T& obj)
	{
		m_services.push_back(&obj);
	}

private:
	std::vector<Singleton*> m_services;
};

#endif
