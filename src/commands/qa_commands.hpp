#ifndef qa_commands_hpp
#define qa_commands_hpp

#include "command_manager.hpp"
#include "direct_command_base.hpp"
#include "gui_commands.hpp"

#include "../io/messenger.hpp"

#include <QApplication>
#include <QPixmap>
#include <QWidget>
#include <QImage>


#include <sstream>
#include <fstream>
/*
#include <filesystem>
namespace fs = std::filesystem;
*/

class dicmdQaToolExit: public NonTransactionalDirectCommandBase
{
    public:
        
        virtual std::string get_name() {
            return "dicmdQaToolExit";
        }
        
        virtual void execute() {
            //FIXME
            QApplication::quit();
            QApplication::exit();
            exit(0);
        }
    
};

class dicmdQaDumpCanvas: public NonTransactionalDirectCommandBase
{
    public:        
        
        dicmdQaDumpCanvas() {
            add_option("-filename",new StringCommandOptionValue("hopar.png"));
        }
        
        virtual std::string get_name() {
            return "dicmdQaDumpCanvas";
        }

        virtual void execute() {
                        
            QWidget* w = command_manager::get_instance()->get_main_widget()->findChild<QWidget*>("CANVAS");
            
            //FIXME exception on error or what?
            if ( !w )
                return;
            
            QPixmap pixmap(w->size());
            w->render(&pixmap);
            pixmap.save(GET_CMD_ARG(StringCommandOptionValue,"-filename").c_str());
        }

};


class dicmdQaCanvasCompareInternal: public NonTransactionalDirectCommandBase
{
    public:        
        dicmdQaCanvasCompareInternal() {
            add_option("-dumpfile",new StringCommandOptionValue("hopar.png"));
            add_option("-goldenfile",new StringCommandOptionValue("hopar.png.golden"));
        }
        
        virtual std::string get_name() {
            return "dicmdQaCanvasCompareInternal";
        }

        virtual void execute() {
            std::stringstream z;
            std::string f(GET_CMD_ARG(StringCommandOptionValue,"-dumpfile"));
            std::string g(GET_CMD_ARG(StringCommandOptionValue,"-goldenfile"));
            
            dicmdQaDumpCanvas().set_arg("-filename",f)->execute();
            z << "cp " << f << " " << g;
            
            bool regoldenmode = true;
        
            if ( QString::fromLocal8Bit( qgetenv("ELEN_PAINTER_REGOLDEN").constData() ).isEmpty() )
                bool regoldenmode = false;
            
            if ( regoldenmode ) {
                Messenger::expose_msg(test,"dicmdQaCanvasCompare-compare-pass: "+f+" "+g);
                //Messenger::expose_msg(test,"dicmdQaCanvasCompare-compare-regolden: "+f+" "+g);
                //std::cout << "#/t CanvasCompare REGOLDENED: " << f << " " << g << std::endl;
                //FIXME not compatible with other OS
                //system(z.str().c_str());
            } else {
            
                if ( are_images_different(f.c_str(),g.c_str()) )
                    Messenger::expose_msg(test,"dicmdQaCanvasCompare-compare-mismatch: "+f+" "+g);
                else 
                    Messenger::expose_msg(test,"dicmdQaCanvasCompare-compare-pass: "+f+" "+g);
            }
            
        }
        
    private:
        
        bool are_images_different(const QString &file1, const QString &file2) {
            QImage img1(file1);
            QImage img2(file2);

            // Check size.
            if (img1.size() != img2.size()) {
                //qDebug("Different size - %dx%d vs %dx%d.", img1.width(), img1.height(), img2.width(), img2.height());
                return true;
            }

            int w = img1.width();
            int h = img1.height();
            for (int ii = 0; ii < w; ii++) {
                for (int jj = 0; jj < h; jj++) {
                    const QRgb px1 = img1.pixel(ii, jj);
                    const QRgb px2 = img2.pixel(ii, jj);
                    if (px1 != px2) {
                        //qDebug("Pixel (%d,%d) differs - (%d,%d,%d,%d) vs (%d,%d,%d,%d).\n",
                         //   ii, jj,
                          //  qAlpha(px1), qRed(px1), qGreen(px1), qBlue(px1),
                          //`  qAlpha(px2), qRed(px2), qGreen(px2), qBlue(px2));
                        return true;
                    }
                }
            }

            return false;
        }
    
};

class dicmdQaCanvasCompare: public NonTransactionalDirectCommandBase 
{
    static int n_index;
        
        std::string get_index_str() {
            std::stringstream z;
            z << "canvas_compare_"<<n_index<<".png";
            
            return z.str();
        }
    
    
    public:        
        virtual std::string get_name() {
            return "dicmdQaCanvasCompare";
        }

        virtual void execute() {
            dicmdQaCanvasCompareInternal()
            .set_arg("-dumpfile",get_index_str())
            ->set_arg("-goldenfile",get_index_str()+".golden")
            ->execute();
            
            n_index++;
        }
};    


class dicmdTestCmdListOptions: public NonTransactionalDirectCommandBase
{
    public:        
        dicmdTestCmdListOptions() {
            add_option("-strings",new StringListCommandOptionValue());
            add_option("-points",new PointListCommandOptionValue());
        }
        
        virtual std::string get_name() {
            return "dicmdTestCmdListOptions";
        }

        virtual void execute() {
            //std::string f(GET_CMD_ARG(StringListCommandOptionValue,"-list1"));
            //std::string g(GET_CMD_ARG(StringListCommandOptionValue,"-list2"));
        }
};



#endif
