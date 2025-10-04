#ifndef qa_commands_hpp
#define qa_commands_hpp

#include "command_manager.hpp"
#include "direct_command_base.hpp"
#include "gui_commands.hpp"

#include "../core/application.hpp"
#include "../core/postman.hpp"
#include "../core/ishape.hpp"
#include "../core/runtime_pool.hpp"
#include "../core/selection.hpp"
#include "../gui/canvas.hpp"
#include "../io/messenger.hpp"

#include <QApplication>
#include <QImage>
#include <QPixmap>
#include <QPicture>
#include <QWidget>

#include <fstream>
#include <sstream>
#include <set>

/*
#include <filesystem>
namespace fs = std::filesystem;
*/

enum qaCompType
{
    DESIGN,
    SELECTION,
    CANVAS,
    RUNTIME,
    SELECTIONCANVAS,
    SELECTIONCANVAS2,
    VIEWPORT_RQ
};

namespace
{

bool are_textfiles_different(const QString &file1, const QString &file2)
{
    QFile data1(file1);
    if (!data1.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return true;
    }

    QFile data2(file2);
    if (!data2.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return true;
    }

    QTextStream in1(&data1), in2(&data2);
    while (!in1.atEnd() && !in2.atEnd())
    {
        QString num1 = in1.readLine();
        QString num2 = in2.readLine();
        if (num1 != num2)
            return true;
    }

    return false;
}


bool are_imagefiles_different_python_magic(const QString &file1, const QString &file2)
{
    auto qa_dir = QString::fromLocal8Bit(qgetenv("PAINTER_QA_DIR").constData()).toStdString();
    std::string script(qa_dir + "/etc/scripts/image_diff.py");
    std::string current = file1.toStdString();
    std::string expected = file2.toStdString();
    //QString diffFile = file1;
    //diffFile.chop(4); // remove ".png"
    //diffFile += ".diff.png";
    //std::string diff = diffFile.toStdString();
    //std::string cmd1 = "python3 " + script + " " + current + " " + expected + " " + " method1 &> method1.txt";
    //std::string cmd2 = "python3 " + script + " " + current + " " + expected + " " + " method2 &> method2.txt";
    //std::string cmd3 = "python3 " + script + " " + current + " " + expected + " " + " method3 &> method3.txt";

    //std::string cmd1 = "python3 " + script + " " + current + " " + expected + " method1 > method1.txt 2>&1";
    //std::string cmd2 = "python3 " + script + " " + current + " " + expected + " method2 > method2.txt 2>&1";
    std::string cmd3 = "python3 " + script + " " + current + " " + expected + " method3 > method3.txt 2>&1";

    //bool res1 = system(cmd1.c_str());
    //bool res2 = system(cmd2.c_str());
    bool res3 = system(cmd3.c_str());

    std::cout << "========= IMGDIFF RES:" << std::endl;
    //std::cout << "                       res1 " << res1 << std::endl;
    //std::cout << "                       res2 " << res2 << std::endl;
    std::cout << "                       res3 " << res3 << std::endl;
    std::cout << "********* IMGDIFF RES:" << std::endl;
    
    //we conclude images different if all 3 methods fail
    //if (res1 && res2 && res3 )
        return res3;
    
    //otherwise they are the same (even if 1 or 2 methods failed)
    //return false;
}

bool are_imagefiles_different_old(const QString &file1, const QString &file2)
{
    QImage img1(file1);
    QImage img2(file2);

    // Check size.
    if (img1.size() != img2.size())
    {
        // qDebug("Different size - %dx%d vs %dx%d.", img1.width(), img1.height(), img2.width(), img2.height());
        return true;
    }

    int w = img1.width();
    int h = img1.height();
    for (int ii = 0; ii < w; ii++)
    {
        for (int jj = 0; jj < h; jj++)
        {
            const QRgb px1 = img1.pixel(ii, jj);
            const QRgb px2 = img2.pixel(ii, jj);
            if (px1 != px2)
            {
                // qDebug("Pixel (%d,%d) differs - (%d,%d,%d,%d) vs (%d,%d,%d,%d).\n",
                //    ii, jj,
                //   qAlpha(px1), qRed(px1), qGreen(px1), qBlue(px1),
                //`  qAlpha(px2), qRed(px2), qGreen(px2), qBlue(px2));
                return true;
            }
        }
    }

    return false;
}

bool are_imagefiles_different_per_pixel(const QString &file1, const QString &file2)
{
    QImage img1(file1);
    QImage img2(file2);

    if (img1.isNull() || img2.isNull()) {
        return true;
    }

    int w = img1.width();
    int h = img1.height();

    // Prepare diff file name
    QString diffFile = file1;
    if (diffFile.endsWith(".png", Qt::CaseInsensitive)) {
        diffFile.chop(4); // remove ".png"
        diffFile += ".diff.png";
    } else {
        diffFile += ".diff.png";
    }

    // Allocate diff image with same size and ARGB format
    QImage diff(w, h, QImage::Format_ARGB32);
    diff.fill(Qt::white); // background for unchanged pixels

    bool different = false;

    for (int ii = 0; ii < w; ++ii) {
        for (int jj = 0; jj < h; ++jj) {
            const QRgb px1 = img1.pixel(ii, jj);
            const QRgb px2 = img2.pixel(ii, jj);

            if (px1 != px2) {
                different = true;
                diff.setPixel(ii, jj, qRgb(0, 0, 0)); // highlight differences in red
            } 
            //else {
            //    diff.setPixel(ii, jj, px1); // keep original pixel for context
            //}
        }
    }

    diff.save(diffFile);

    return different;
}

double similarity(const QByteArray &a, const QByteArray &b) {
    int len = std::min(a.size(), b.size());
    int same = 0;
    for (int i = 0; i < len; ++i) {
        if (a[i] == b[i]) same++;
    }
    return 100.0 * same / std::max(a.size(), b.size());
}

bool comparePictureData(const QString &f1, const QString &f2) 
{
    std::cout << "comparing " << f1.toStdString() << " with " << f2.toStdString() << std::endl;
    QFile file1(f1), file2(f2);
    if (!file1.open(QIODevice::ReadOnly) || !file2.open(QIODevice::ReadOnly))
        return true;

    QByteArray d1 = file1.readAll();
    QByteArray d2 = file2.readAll();
    std::cout << "similarity: " << similarity(d1,d2) << std::endl;
    return d1 != d2;
}

bool check_pics(const QString &file1, const QString &file2)
{
    auto f1 = file1 + ".pic";
    auto f2 = file2 + ".pic";

    return comparePictureData(f1, f2);
}

bool are_imagefiles_different(const QString &file1, const QString &file2)
{
    //first check per pixel
    //if (are_imagefiles_different_per_pixel(file1,file2))
        //if different per pixel do more sophisiticated comparisions
        //return are_imagefiles_different_python_magic(file1,file2);

    check_pics(file1, file2);
    are_imagefiles_different_per_pixel(file1,file2);
    are_imagefiles_different_python_magic(file1,file2);

    return true;
}

bool are_two_files_different(qaCompType type, const QString &file1, const QString &file2)
{
    if (type == CANVAS)
        return are_imagefiles_different(file1, file2);
    return are_textfiles_different(file1, file2);
}

} // namespace

class dicmdQaReplyStep : public NonTransactionalDirectCommandBase
{
  public:
    virtual std::string get_name()
    {
        return "dicmdQaReplyStep";
    }

    virtual void execute()
    {
        LeCallbackData d;
        NOTIFY(STEP_REPLY, d);
    }
};

class dicmdQaReplyingBreak : public NonTransactionalDirectCommandBase
{
  public:
    virtual std::string get_name()
    {
        return "dicmdQaReplyingBreak";
    }

    virtual void execute()
    {
        LeCallbackData d;
        NOTIFY(STOP_REPLY, d);
    }
};

class dicmdQaReplyingResume : public NonTransactionalDirectCommandBase
{
  public:
    virtual std::string get_name()
    {
        return "dicmdQaReplyingResume";
    }

    virtual void execute()
    {
        LeCallbackData d;
        NOTIFY(RESUME_REPLY, d);
    }
};

class dicmdQaToolExit : public NonTransactionalDirectCommandBase
{
  public:
    virtual std::string get_name()
    {
        return "dicmdQaToolExit";
    }

    virtual void execute()
    {
        // FIXME
        if (Application::getInstance().is_debug_mode())
            return;

        QApplication::quit();
        QApplication::exit();
        exit(0);
    }
};

namespace
{
std::string qaCompType2string(qaCompType type)
{
    switch (type)
    {
    case DESIGN:
        return ("Design");
        break;
    case SELECTION:
        return ("Selection");
        break;
    case CANVAS:
        return ("Canvas");
        break;
    case RUNTIME:
        return ("Runtime");
        break;
    case VIEWPORT_RQ:
        return ("ViewportRQ");
        break;
    default:
        return "";
    }
    return "";
}
} // namespace

template <qaCompType T> class dicmdQaDump : public NonTransactionalDirectCommandBase
{
    std::string m_fname;

  public:
    dicmdQaDump()
    {
        add_option("-filename", new StringCommandOptionValue("hopar.png"));
    }

    virtual std::string get_name()
    {
        return "dicmdQaDump" + qaCompType2string(T);
    }

    virtual void execute()
    {
        m_fname = GET_CMD_ARG(StringCommandOptionValue, "-filename");
        switch (T)
        {
        case RUNTIME:
            return dump_runtimes();
            break;
        case DESIGN:
            return dump_design();
            break;
        case SELECTION:
            return dump_selection();
            break;
        case CANVAS:
            return dump_canvas();
            break;
        case SELECTIONCANVAS:
            return dump_canvas_wrapper();
            break;
        case SELECTIONCANVAS2:
            return dump_canvas_wrapper(true);
            break;
        case VIEWPORT_RQ:
            return dump_rq();
            break;
        }
    }

  private:
    void dump_canvas_wrapper(bool isrt = false)
    {
        // std::cout << "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" << std::endl;
        // if (!isrt)
        Selection::getInstance().highlight_last_selected_region(true);

        dump_canvas(isrt);

        // if (!isrt)
        Selection::getInstance().highlight_last_selected_region(false);
    }

    void dump_canvas(bool onlyrt = false)
    {
        QWidget *w = command_manager::getInstance().get_main_widget()->findChild<QWidget *>("CANVAS");

        //dynamic_cast<canvas *>(w)->get_renderer()->hint_drawing_cursor_one_time();
        // FIXME exception on error or what?
        if (!w)
            return;

        if (onlyrt)
            dynamic_cast<canvas *>(w)->get_renderer()->rendering_des_mode_change();
        
        //*
        QPixmap pixmap(w->size());
        w->render(&pixmap);
        pixmap.save(m_fname.c_str());
        /**/

        /*
        QImage image(w->size(), QImage::Format_Mono);
        image.fill(Qt::transparent);
        //QPainter p(&image);
        w->render(&image);
        //p.end();
        image.save(m_fname.c_str()); 
        */

        QPicture picture;
        //QPainter painter(&picture);
        w->render(&picture);
        //painter.end();
        auto pic_name = m_fname + ".pic";
        picture.save(pic_name.c_str());

        //dynamic_cast<canvas *>(w)->get_renderer()->hint_drawing_cursor_one_time();

        if (onlyrt)
            dynamic_cast<canvas *>(w)->get_renderer()->rendering_des_mode_change();

        std::cout << m_fname.c_str() << "\n\n\n\n";
    }

    void dump_rq()
    {
        QWidget *w = command_manager::getInstance().get_main_widget()->findChild<QWidget *>("CANVAS");
        QRect v = dynamic_cast<canvas *>(w)->get_renderer()->get_viewport();
        RegionQuery& rq = RegionQuery::getInstance();
        auto objs = rq.getShapesUnderRect(v);
        
        QFile file(m_fname.c_str());
        file.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream z(&file);

        // Sort objects before dumping.
        std::vector<std::multiset<ShapeProperties>> shapes_sorted_info(4);
        for (auto i : objs)
            shapes_sorted_info[i->getType()].insert(i->getProperties());

        z << "Name: Viewport RQ" ;
        z << "\nObjCount: " << QString::number(objs.size());
        z << "\n======\n";
        for(size_t i=0; i<shapes_sorted_info.size(); i++)
            for (auto const& y : shapes_sorted_info[i])
            {
                z << ObjType2String(ObjectType(i)).c_str();
                z << ":\t"; // i->getPoints();
                z << y.toString().c_str();
                z << "\n";
            }
        z << "--------";
        z << "\n\n";

        file.flush();
        file.close();
    }

    void dump_selection()
    {
        Selection::getInstance().dumpToFile(m_fname);
    }

    void dump_design()
    {
        // DesignManager::getInstance().dump_to_file(fname);
    }

    void dump_runtimes()
    {
        for (auto it : RuntimePoolManager::getInstance().getChildren())
        {
            it.second->dumpToFile(m_fname);
        }
    }
};

template <qaCompType T> class dicmdQaCompare : public NonTransactionalDirectCommandBase
{
    static int n_index;

    std::string get_index_str()
    {
        std::stringstream z;
        z << qaCompType2string(T) << "_Compare_" << n_index;
        if (T == CANVAS)
            z << ".png";
        else
            z << ".txt";
        return z.str();
    }

  public:
    virtual std::string get_name()
    {
        return "dicmdQaCompare" + qaCompType2string(T);
    }

    static int get_current_index()
    {
        return n_index;
    }

    virtual void execute();
};

template <qaCompType T> class dicmdQaCompareInternal : public NonTransactionalDirectCommandBase
{
  public:
    dicmdQaCompareInternal()
    {
        add_option("-dumpfile", new StringCommandOptionValue("hopar.png"));
        add_option("-goldenfile", new StringCommandOptionValue("hopar.png.golden"));
    }

    virtual std::string get_name()
    {
        return "dicmdQaCompareInternal" + qaCompType2string(T);
    }

    virtual void execute()
    {
        std::stringstream z;
        std::string f(GET_CMD_ARG(StringCommandOptionValue, "-dumpfile"));
        std::string g(GET_CMD_ARG(StringCommandOptionValue, "-goldenfile"));

        dicmdQaDump<T>().set_arg("-filename", f)->execute();

        // std::cout << "regoooooldneeeen" << QString::fromLocal8Bit( qgetenv("ELEN_PAINTER_REGOLDEN").constData()
        // ).toStdString() << std::endl;
        bool regoldenmode = false;
        bool creationmode = false;
        if (!QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_REGOLDEN").constData()).isEmpty())
            regoldenmode = true;

        if (!QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_TESTCREATION").constData()).isEmpty())
            creationmode = true;
        
        if (creationmode) {
            Messenger::expose_msg(info, "Created comparision checkpoint:" + qaCompType2string(T) + " " + g);
            return;
        }

        if (regoldenmode)
        {
            // std::cout << "aaaaaaaaar" << std::endl;
// Messenger::expose_msg(test,"dicmdQaCanvasCompare-compare-regolden: "+f+" "+g);
// std::cout << "#/t CanvasCompare REGOLDENED: " << f << " " << g << std::endl;
// FIXME not compatible with other OS
#if defined(OS_LINUX) || defined(OS_MAC) || defined(__APPLE__)
            // std::cout << "hoparrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr" << std::endl;
            z << "cp " << f << " " << g;
            system(z.str().c_str());
            Messenger::expose_msg(test, "comparision->" + qaCompType2string(T) + ":PASS " + f + " " + g);
#else
            Messenger::expose_msg(err, "Autoregoldening is availble only in linux and mac ( currently )");
#endif
        }
        else
        {

            auto fnCheckBreak = [&](bool on_failure)
            {
                if (Application::is_debug_mode())
                {
                    // if on_failure is false then it's master, check env_variable
                    auto bResult = true;
                    if (on_failure == false)
                    {
                        const auto compareType = QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_COMPAREDBG").constData());
                        if (compareType.isEmpty())
                        {
                            return false;
                        }
                        std::cout << "Not empty" << std::endl;
                        bResult = false;
                    }
                    // Check if values are defined
                    // compare with type T 
                    const auto compareType = QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_TESTTYPE").constData());
                    if (compareType.isEmpty())
                    {
                        return bResult;
                    }
                    if (compareType.toLower().toStdString() != QString::fromStdString(qaCompType2string(T)).toLower().toStdString())
                    {
                        return false;
                    }
                    // counter 
                    // read the value 
                    const auto compareCounter = QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_COUNTER").constData());
                    if (compareCounter.isEmpty())
                    {
                        return bResult;
                    }
                    if (dicmdQaCompare<T>::get_current_index() < compareCounter.toInt())
                    {
                        return false;
                    }
                    return true;
                }
                return false;
            };

            //check if ELEN_PAINTER_COMPAREDBG then stop at comparision number.
            if (are_two_files_different(T, f.c_str(), g.c_str()) || fnCheckBreak(false))
            {
                QString htmlv = generate_html_view(f, g);
                if (fnCheckBreak(true))
                { 
                    Messenger::expose_msg(err, "comparision->" + qaCompType2string(T) + ":MISMATCH " + f + " " + g +
                                                   ". Click <a href=\"file://" + htmlv.toStdString() +
                                                   "\">here</a> to see the diff.");
                    dicmdQaReplyingBreak().execute_and_log();
                }
                else
                {
                    Messenger::expose_msg(err, "comparision->" + qaCompType2string(T) + ":MISMATCH " + f + " " + g);
                }
            }
            else
                Messenger::expose_msg(test, "comparision->" + qaCompType2string(T) + ":PASS " + f + " " + g);
        }
    }

  private:
    // fixme need total refactoring !
    QString generate_html_view(const std::string &f, const std::string &g)
    {
        QString res(QDir::currentPath() + QString("/" + QString(f.c_str()) + ".html"));
#ifdef OS_LINUX
        // system();
        // std::string s("touch "+f+".html");
        std::string s1("cat html_diff.template | sed 's/%fname1%/" + g + "/' | sed 's/%fname2%/" + f + "/' > " + f +
                       ".html");
        // std::string s2("cat "+f+".html | sed 's/%f1%/cat "+g+"/e' > 1"+f+".html");
        // std::string s3("cat "+f+".html | sed 's/%f2%/cat "+f+"/e' > 2"+f+".html");
        std::string s2("sed -i 's/%f1%/cat " + g + "/e' " + f + ".html");
        std::string s3("sed -i 's/%f2%/cat " + f + "/e' " + f + ".html");

        system(s1.c_str());
        system(s2.c_str());
        system(s3.c_str());
#endif
        return res;
    }
};

template <qaCompType T> 
void dicmdQaCompare<T>::execute()
{
    // if not a canvas compare, do extra canvas compare in any case
    
    command_manager::getInstance().fix_last_qa_point();

    if (T != CANVAS)
    {
        dicmdQaDump<CANVAS>().set_arg("-filename", "CanvasFor_" + get_index_str() + ".png")->execute();
        // std::cout << "r1egoooooldneeeen" << QString::fromLocal8Bit( qgetenv("ELEN_PAINTER_REGOLDEN").constData()
        // ).toStdString() << std::endl;

        if (!QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_REGOLDEN").constData()).isEmpty())
        {
            // std::cout << "r?????" << std::endl;
            if (T == RUNTIME)
                dicmdQaDump<SELECTIONCANVAS2>()
                    .set_arg("-filename", "CanvasFor_" + get_index_str() + ".golden.png")
                    ->execute();
            else
                dicmdQaDump<SELECTIONCANVAS>()
                    .set_arg("-filename", "CanvasFor_" + get_index_str() + ".golden.png")
                    ->execute();
        }
    
        dicmdQaCompareInternal<T>()
            .set_arg("-dumpfile", get_index_str())
            ->set_arg("-goldenfile", get_index_str() + ".golden")
            ->execute();
    } else {
        if (!QString::fromLocal8Bit(qgetenv("ELEN_PAINTER_REGOLDEN").constData()).isEmpty())
            dicmdQaDump<CANVAS>()
                .set_arg("-filename", "CanvasFor_" + get_index_str() + ".golden.png")
                ->execute();
        //else 
            dicmdQaCompareInternal<CANVAS>()
                .set_arg("-dumpfile", "CanvasFor_" + get_index_str())
                ->set_arg("-goldenfile", "CanvasFor_" + get_index_str() + ".golden.png")
                ->execute();
    }


    n_index++;
}

class dicmdTestCmdListOptions : public NonTransactionalDirectCommandBase
{
  public:
    dicmdTestCmdListOptions()
    {
        add_option("-strings", new StringListCommandOptionValue());
        add_option("-points", new PointListCommandOptionValue());
    }

    virtual std::string get_name()
    {
        return "dicmdTestCmdListOptions";
    }

    virtual void execute()
    {
        // std::string f(GET_CMD_ARG(StringListCommandOptionValue,"-list1"));
        // std::string g(GET_CMD_ARG(StringListCommandOptionValue,"-list2"));
    }
};

#endif
