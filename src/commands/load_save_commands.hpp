#ifndef save_laod_command_hpp
#define save_laod_command_hpp

#include "direct_command_base.hpp"
#include "shape_creation_directive_commands.hpp"

#include "../core/runtime_pool.hpp"

#include "../io/log_reader.hpp"

#include <QPoint>

#include <iostream>

#include "../core/design/design_manager.hpp"
#include "../core/rq/RegionQueryService.hpp"

class dicmdDesignLoad : public DirectCommandBase
{
  public:
    dicmdDesignLoad()
    {
        add_option("-filename", new StringCommandOptionValue());
    }

    dicmdDesignLoad(const std::string &fname)
    {
        add_option("-filename", new StringCommandOptionValue(fname));
    }

    virtual void execute()
    {
        RegionQuery::getInstance().clear();
        std::string fname(GET_CMD_ARG(StringCommandOptionValue, "-filename"));
        if (fname.ends_with(".spd"))
        {
            auto& dm = DesignManager::getInstance();
            auto pActiveDesign = dm.getActiveDesign();
            if (pActiveDesign)
            {
                pActiveDesign->clear();
            }
            dm.openDesign(fname, 0);
            dm.setActiveDesign(0);
            auto arrObjects = dm.getActiveDesign()->getObjects();
            auto& rq = RegionQuery::getInstance();
            rq.insertObjects(arrObjects);
        }
        else
        {
            if (!LogReader().replay_logfile_imi(fname))
                throw 1;
        }
    }

    virtual std::string get_name()
    {
        return "dicmdDesignLoad";
    }
};

class dicmdDesignSave : public DirectCommandBase
{
  public:
    dicmdDesignSave()
    {
        add_option("-filename", new StringCommandOptionValue());
    }

    dicmdDesignSave(const std::string &fname)
    {
        add_option("-filename", new StringCommandOptionValue(fname));
    }

    // fixme , refactor
    virtual void execute()
    {
        std::string fname(GET_CMD_ARG(StringCommandOptionValue, "-filename"));
        CommandBase *cmd;
        auto& dm = DesignManager::getInstance();
        auto pActiveDesign = dm.getActiveDesign();
        if (fname.ends_with(".spd"))
        {
            pActiveDesign->saveToFile(fname);
            return;
        }
        auto arrIds = pActiveDesign->getObjectIDs();
        std::sort(arrIds.begin(), arrIds.end());
        for (auto it : arrIds)
        {
            IShapePtr pObject = std::dynamic_pointer_cast<IShape>(pActiveDesign->getObject(it));
            switch (pObject->getType())
            {
            case LINE:
                cmd = new dicmdCreateObj<LINE>(transform(pObject->getPoints()), pObject->getProperties());
                // cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                dynamic_cast<dicmdCreateObj<LINE> *>(cmd)->dump(fname);
                break;
            case RECTANGLE:
                cmd = new dicmdCreateObj<RECTANGLE>(transform(pObject->getPoints()), pObject->getProperties());
                // cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                dynamic_cast<dicmdCreateObj<RECTANGLE> *>(cmd)->dump(fname);
                break;
            case ELLIPSE:
                cmd = new dicmdCreateObj<ELLIPSE>(transform(pObject->getPoints()), pObject->getProperties());
                // cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                dynamic_cast<dicmdCreateObj<ELLIPSE> *>(cmd)->dump(fname);
                break;
            case POLYGON:
                cmd = new dicmdCreateObj<POLYGON>(transform(pObject->getPoints()), pObject->getProperties());
                // cmd->set_arg("-points",PointListCommandOptionValue(transform(it->getPoints())).to_string());
                dynamic_cast<dicmdCreateObj<POLYGON> *>(cmd)->dump(fname);
                break;
            }

            // dicmdCreateObj<T>(m_internal_vec,ws).silent_execute();
            // delete cmd;
            // cmd = 0;
            // std::cout << it.getType() << getPoints << std::endl;
        }
    }

    virtual std::string get_name()
    {
        return "dicmdDesignSave";
    }

  private:
    std::vector<PointCommandOptionValue> transform(const std::vector<QPoint> &v)
    {
        std::vector<PointCommandOptionValue> res; //(v.size());
        for (auto it : v)
        {
            res.push_back(PointCommandOptionValue(it));
        }
        return res;
    }
};

#endif
