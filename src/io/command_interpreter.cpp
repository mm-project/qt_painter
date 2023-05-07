#include "command_interpreter.hpp"

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


bool CommandInterp::interpret_from_file(const std::string& path)
{
    FILE* fp = fopen( path.c_str(), "r" );
    int re = PyRun_SimpleFile( fp, path.c_str() );
    fclose( fp );
    return true;
}

bool CommandInterp::interpret_from_string(const std::string &n)
{
    //execute_cmd(get_cmd_obj(n));
    PyRun_SimpleString(n.c_str());
    return true;
}

CommandInterp::CommandInterp()
{
    PyImport_AppendInittab("mmproject", &PyInit_mmproject);
    Py_Initialize();
    PyRun_SimpleString("import mmproject\n");
}
