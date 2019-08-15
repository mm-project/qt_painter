#include "messenger.hpp"

#include "../core/postman.hpp"
#include "../core/callback.hpp"

#include <QString>
#include <QDateTime>

#include <iostream>
#include <sstream>
#include <functional>
#include <cassert>


Messenger::Messenger() {
	init();
	REGISTER_CALLBACK(INTERACTIVE_COMMAND_PRE_COMMIT, &Messenger::test1);
	REGISTER_CALLBACK(INTERACTIVE_COMMAND_POST_COMMIT,&Messenger::test1);
}

Messenger::~Messenger() {
	fini();
}

void Messenger::test1(LeCallbackData& d) {
	Messenger::expose_msg(out,"Callback working");
}

void Messenger::fini() {
	m_cmdfile->close();
	m_logfile->close(); 
}

void Messenger::init() {
    
        QString pathenv = QString::fromLocal8Bit( qgetenv("PAINTER_LOGS_DIR").constData() );
        QString idpostfix = QString::fromLocal8Bit( qgetenv("PAINTER_LOGFILE_PREFIX").constData());
        
	QString m_path = pathenv.isEmpty()?"./logs/":pathenv;
        
        
	QString postfix = "painter"+QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
	postfix = idpostfix.isEmpty()?postfix:idpostfix;
        //std::cout << id.toStdString() << std::endl;
	
	m_dir = new QDir;//;("logs");
	
	if (!m_dir->exists(m_path))
	m_dir->mkpath(m_path); // You can check the success if needed

	m_cmdfile = new QFile(m_path + postfix +".lvi");
	m_logfile = new QFile(m_path + postfix +".log");
	
	/*
	m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	m_logfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	
	log_stream = new QTextStream(m_logfile);
	cmd_stream = new QTextStream(m_cmdfile);
	*/
}
	
std::string Messenger::decorate_for_logging(const LogMsgSeverity& r) {
	
	switch (r) {
                case ok:
                        return("");
                        break;
                case err:
			return("#e --> ERROR: ");
			break;
		case info:
			return("#i --> ");
			break;
		case warn:
			return("#w --> WARNING: ");
			break;
		case out:
			return("#o --> ");
			break;
                case cont:
                        return("#c ");
                        break;
                case test:
                        return("#t --> TEST: ");
                        break;
                case modal:
                        return("#m ");
                        break;                           
                default:
			return("#? ");
                        break;
		}
}


//FIXME
void Messenger::expose_internal(const LogMsgSeverity& severity, const std::string& msg , bool iscmd) 
{
	
        
        std::stringstream z;
	z << decorate_for_logging(severity) << msg;
        write_entry_to_console_gui(severity,z.str());
        z << "\n";
        write_entry_to_logfile(z.str());

	
	// if this is <real> command, write also to lvi file.
	if ( iscmd ) 
            write_entry_to_cmdfile(msg);
        
}

void Messenger::write_entry_to_console_gui(const LogMsgSeverity& s, const std::string& msg) {
        std::cout << msg << std::endl;
        //nagaina update please update here :)
        if (m_console_callback != nullptr)
            m_console_callback(msg);
}
	
	
//fixme duplicates
void Messenger::write_entry_to_logfile(const std::string& msg) {
	m_logfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	log_stream = new QTextStream(m_logfile);
	(*log_stream) << msg.c_str();
	m_logfile->flush();
	m_logfile->close();
}

void Messenger::write_entry_to_cmdfile(const std::string& msg) {
        m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
        cmd_stream = new QTextStream(m_cmdfile);
        (*cmd_stream) << (msg+"\n").c_str();
        m_cmdfile->flush();
        m_cmdfile->close();
}
			
//static			
void Messenger::expose_msg(const LogMsgSeverity& s, const std::string& msg, bool iscmd )
{
	Messenger::get_instance()->expose_internal(s,msg,iscmd);
}
    	
//static used by CommandBase internally , fixme add friend		
void Messenger::log_command(const std::string& msg, bool iscmd) 
{
	Messenger::expose_msg(ok,msg,iscmd);	
}

//Messenger::expose(err,"Error: ... ")
	

