#ifndef I_OBJECT_POOL_HPP
#define I_OBJECT_POOL_HPP

///////////////////////////////////////////////////////////////////////////////
//
//	Inludes	
//
#include "ishape.hpp"

//
//Stl
//
#include <vector>
#include <memory>

///////////////////////////////////////////////////////////////////////////////
//
//	Object pool interface
//
class IObjectPool
{
public:
	//
	//	Interface
	//
	//	Desturctor
	virtual ~IObjectPool() = default;

public:
	//	Clear the content
	virtual void clear() noexcept = 0;

	//	Content modiifiers
	virtual std::vector<IShapePtr> getObjects() const noexcept = 0;
	virtual IShapePtr addObject(IShapePtr) = 0;
	virtual void removeObject(IShapePtr) noexcept = 0;
	
        virtual std::string getName() const noexcept = 0; 
        virtual void dumpToFile(const std::string&) const = 0;
};

using ObjectPoolPtr = std::shared_ptr<IObjectPool>;

//	default implementation
class ObjectPoolBase : public IObjectPool
{
public:
	//
	//	Interface
	//
	//	Desturctor
	virtual ~ObjectPoolBase() = default;

public:
	//	Clear the content
	void clear() noexcept override;

	//	Content modiifiers
	std::vector<IShapePtr> getObjects() const noexcept override;
	IShapePtr addObject(IShapePtr) override;
	void removeObject(IShapePtr) noexcept override;
        std::string getName() const noexcept override { return std::move("Unnamed");} 	
        void dumpToFile(const std::string&) const override;

protected:
	std::vector<IShapePtr> m_shapes;
};

#endif
