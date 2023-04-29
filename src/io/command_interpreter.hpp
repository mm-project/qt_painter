#ifndef command_interpreter_hpp
#define command_interpreter_hpp

#include "messenger.hpp"

#include "../commands/command_manager.hpp"
#include "../commands/direct_command_base.hpp"
#include "../commands/icommand_base.hpp"
#include "../core/service.hpp"

#include <QString>
#include <QStringList>

#include <iostream>
#include <string>
#include <cstdio>

#define PY_SSIZE_T_CLEAN
#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

//*
namespace {
static PyObject* python_dicmdCanvasMouseClick(PyObject *self, PyObject *args)
{

    int x;
    int y;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdCanvasMouseClick");

    if(!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;

    std::cout << x << " " << y << std::endl;
    QString coord_str = "(" +QString::number(x) + "," + QString::number(y) +")";
    cmd->set_arg("-point",coord_str.toStdString());
    //cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    cmd->execute_and_log();
    return PyLong_FromLong(0);

}

static PyObject* python_dicmdCanvasMousePress(PyObject *self, PyObject *args)
{

    int x;
    int y;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdCanvasMousePress");

    if(!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;

    std::cout << x << " " << y << std::endl;
    QString coord_str = "(" +QString::number(x) + "," + QString::number(y) +")";
    cmd->set_arg("-point",coord_str.toStdString());
    //cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    cmd->execute_and_log();
    return PyLong_FromLong(0);

}

static PyObject* python_dicmdCanvasMouseRelease(PyObject *self, PyObject *args)
{

    int x;
    int y;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdCanvasMouseRelease");

    if(!PyArg_ParseTuple(args, "ii", &x, &y))
        return NULL;

    std::cout << x << " " << y << std::endl;
    QString coord_str = "(" +QString::number(x) + "," + QString::number(y) +")";
    cmd->set_arg("-point",coord_str.toStdString());
    cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    return PyLong_FromLong(0);

}


static PyObject* python_dicmdCreateObjLine(PyObject *self, PyObject *args)
{

    int x1;
    int y1;
    int x2;
    int y2;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdCreateObjLine");

    if(!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2))
        return NULL;

    //std::cout << x << " " << y << std::endl;
    QString coord_str1 = "(" +QString::number(x1) + "," + QString::number(y1) +")";
    QString coord_str2 = "(" +QString::number(x2) + "," + QString::number(y2) +")";
    QString final_str = "{" + coord_str1 + ";" + coord_str2 + "}";

    cmd->set_arg("-brush","1");
    cmd->set_arg("-color","#0000ff");
    cmd->set_arg("-fill","9");
    cmd->set_arg("-points",final_str.toStdString());
    //cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    cmd->execute_and_log();
    return PyLong_FromLong(0);

}


static PyObject* python_dicmdCreateObjRectangle(PyObject *self, PyObject *args)
{

    //assert(0);
    int x1;
    int y1;
    int x2;
    int y2;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdCreateObjRectangle");

    if(!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2))
        return NULL;

    //std::cout << x << " " << y << std::endl;
    QString coord_str1 = "(" +QString::number(x1) + "," + QString::number(y1) +")";
    QString coord_str2 = "(" +QString::number(x2) + "," + QString::number(y2) +")";
    QString final_str = "{" + coord_str1 + ";" + coord_str2 + "}";

    cmd->set_arg("-brush","1");
    cmd->set_arg("-color","#0000ff");
    cmd->set_arg("-fill","9");
    cmd->set_arg("-points",final_str.toStdString());
    //cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    cmd->execute_and_log();
    return PyLong_FromLong(0);

}

static PyObject* python_dicmdSelectShapesByRegion(PyObject *self, PyObject *args)
{

    //assert(0);
    int x1;
    int y1;
    int x2;
    int y2;
    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdSelectShapesByRegion");

    if(!PyArg_ParseTuple(args, "iiii", &x1, &y1, &x2, &y2))
        return NULL;

    //std::cout << x << " " << y << std::endl;
    QString coord_str1 = "(" +QString::number(x1) + "," + QString::number(y1) +")";
    QString coord_str2 = "(" +QString::number(x2) + "," + QString::number(y2) +")";

    cmd->set_arg("-start",coord_str1.toStdString());
    cmd->set_arg("-end",coord_str2.toStdString());
    //cm.activate_command(dynamic_cast<CommandBase *>(cmd));
    cmd->execute_and_log();
    return PyLong_FromLong(0);

}

static PyObject* python_dicmdQaToolExit(PyObject *self, PyObject *args)
{

    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdQaToolExit");

    if(!PyArg_ParseTuple(args, ""))
        return NULL;

    cmd->execute_and_log();
    return PyLong_FromLong(0);

}

static PyObject* python_dicmdQaCompareSelection(PyObject *self, PyObject *args)
{

    command_manager& cm = command_manager::getInstance();
    CommandBase* cmd = cm.find_command("dicmdQaCompareSelection");

    if(!PyArg_ParseTuple(args, ""))
        return NULL;

    cmd->execute_and_log();
    return PyLong_FromLong(0);

}


//mmproject.dicmdCreateObjLine(128,219,328,100)
//mmproject.dicmdCreateObjLine -brush 1 -color #000000 -fill 9 -points {(128,219);(328,100)}
//mmproject.dicmdQaToolExit
//dicmdSelectShapesByRegion -end (471,236) -start (112,24)
//mmproject.dicmdQaCompareSelection()
static PyMethodDef MMProjectMethods[] = {
    {"dicmdCanvasMouseClick", python_dicmdCanvasMouseClick, METH_VARARGS , "Mouse click action on canvas."},
    {"dicmdCanvasMousePress", python_dicmdCanvasMousePress, METH_VARARGS , "Mouse press (and hold) action on canvas."},
    {"dicmdCanvasMouseRelease", python_dicmdCanvasMouseRelease, METH_VARARGS , "Mouse release action on canvas."},
    {"dicmdCreateObjLine", python_dicmdCreateObjLine, METH_VARARGS , "Creates line on canvas given coordinates."},
    {"dicmdCreateObjRectangle", python_dicmdCreateObjRectangle, METH_VARARGS , "Creates rectangle on canvas given coordinates."},
    {"dicmdSelectShapesByRegion", python_dicmdSelectShapesByRegion, METH_VARARGS , "Selects shapes in canvas given coordinates."},
    {"dicmdQaCompareSelection", python_dicmdQaCompareSelection, METH_VARARGS , "Selects shapes in canvas given coordinates."},
    {"dicmdQaToolExit", python_dicmdQaToolExit, METH_VARARGS , "Exit the tool."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef MMProjectModule = {
    PyModuleDef_HEAD_INIT, "emb", NULL, -1, MMProjectMethods,
    NULL, NULL, NULL, NULL
};

static PyObject*
PyInit_mmproject(void)
{
    return PyModule_Create(&MMProjectModule);
}

}
//*/

class CommandInterp : public Service<CommandInterp>
{
    command_manager &m_cm = command_manager::getInstance();
  public:
	CommandInterp() {
		PyImport_AppendInittab("mmproject", &PyInit_mmproject);
		Py_Initialize();
		PyRun_SimpleString("import mmproject\n");
	}

  public:
    bool interpret_from_file(const std::string& path)
    {
		FILE* fp = fopen( path.c_str(), "r" );
		int re = PyRun_SimpleFile( fp, path.c_str() );
		fclose( fp );
        return true;
    }

    bool interpret_from_string(const std::string &n)
    {
        //execute_cmd(get_cmd_obj(n));
        PyRun_SimpleString(n.c_str());
        return true;
    }

    CommandBase *get_cmd_obj(const std::string &n)
    {
        if (n.empty())
            return nullptr;
        QStringList tokens = QString(n.c_str()).split(" ");
        std::string cmd_name = tokens[0].toStdString();
        // std::cout << "<"<<n<<">" << tokens[0].toStdString() << std::endl;
        if (cmd_name.front() == '#')
            return 0;

        CommandBase *cmd = command_manager::getInstance().find_command(cmd_name);
        if (!cmd)
        {
            Messenger::expose_msg(usr, cmd_name);
            Messenger::expose_msg(err, cmd_name + " command not found"); // FIXME enhance with message.err file
            return 0;
        }

        // std::cout << cmd << std::endl;
        // std::cout << cmd->get_name() << std::endl;
        // std::cout << cmd->get_type() << std::endl;

        // fixme parsing
        for (int i = 1; i < tokens.size() - 1; i = i + 2)
        {
            // std::cout << "  ---<" << tokens[i].toStdString() << "> <=> <"<< tokens[i+1].toStdString() << ">" <<
            // std::endl;
            if (!cmd->set_arg(tokens[i].toStdString(), tokens[i + 1].toStdString()))
            {
                Messenger::expose_msg(usr,
                                      cmd_name + " " + tokens[i].toStdString() + " " + tokens[i + 1].toStdString());
                Messenger::expose_msg(err, tokens[i].toStdString() + " argument is in invalid ");
                return 0;
            }
        }

        return cmd;
    }

    void execute_cmd(CommandBase *cmd)
    {
        if (!cmd)
            return;
        // std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!<"<<n.toStdString()<<">" << tokens[0].toStdString() <<
        // std::endl;

        if (cmd->get_type() == Interactive)
        {
            // std::cout  << "?????????" << cmd->get_name() << std::endl;
            m_cm.activate_command(dynamic_cast<CommandBase *>(cmd));
        }
        else
        {
            cmd->execute_and_log();
            // std::cout  << "!!!!!!!!!!!!" << dynamic_cast<DirectCommandBase*>(cmd)->get_cmdname_and_stringified_opts()
            // << std::endl;
        }
    }
};

#endif
