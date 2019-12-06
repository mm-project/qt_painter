#ifndef DESIGN_HPP
#define DESIGN_HPP

#include "ishape.hpp"
#include "iobject_pool.hpp"

#include <vector>
#include <QObject>

class Design /*final*/ : public ObjectPoolBase
{
public:
	//
	//	Interface
	//
	virtual ~Design() = default;

public:
    IShapePtr addObject(IShapePtr) override;
	std::string getName() const noexcept override;
	void dumpToFile(const std::string&) const override;
};

using DesignPtr = std::shared_ptr<Design>;

#endif
