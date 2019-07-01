#include "messenger.hpp"


#include <QString>
#include <QDateTime>

#include <iostream>
#include <sstream>
#include <cassert>

Messenger::Messenger() {
	init();
}

Messenger::~Messenger() {
	fini();
}

void Messenger::fini() {
	m_cmdfile->close();
	m_logfile->close(); 
}

void Messenger::init() {
	QString m_path("./logs");
	QString id = QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss");
	std::cout << id.toStdString() << std::endl;
	
	m_dir = new QDir;//;("logs");
	
	if (!m_dir->exists(m_path))
	m_dir->mkpath(m_path); // You can check the success if needed

	m_cmdfile = new QFile(m_path + "/painter."+id+".out");
	m_logfile = new QFile(m_path + "/painter."+id+".log");
	
	/*
	m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	m_logfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	
	log_stream = new QTextStream(m_logfile);
	cmd_stream = new QTextStream(m_cmdfile);
	*/
}
	
std::string Messenger::decorate(const LogMsgSeverity& r) {
	
	switch (r) {
		case err:
			return("#e ");
			//break;
		case ok:
			return("#i ");
			break;
		case warn:
			return("#w ");
			break;
		case out:
			return("#o ");
			break;
		default:
			break;
		}
}

//FIXME
void Messenger::expose_internal(const LogMsgSeverity& s, const std::string& msg , bool cmd) 
{
	std::stringstream z;
	z << decorate(s) << msg << "\n";
	
	m_logfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	log_stream = new QTextStream(m_logfile);
	(*log_stream) << z.str().c_str();
	m_logfile->flush();
	m_logfile->close();
	
	
	if ( cmd ) {
		m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
		cmd_stream = new QTextStream(m_cmdfile);
		(*cmd_stream) << (msg+"\n").c_str();
		m_cmdfile->flush();
		m_cmdfile->close();
	}
	
	
	std::cout << z.str();	
}
			
void Messenger::expose(const LogMsgSeverity& s, const std::string& msg, bool iscmd = false )
{
	Messenger::get_instance()->expose_internal(s,msg,iscmd);
}
    	
//used by CommandBase internally , fixme add friend		
void Messenger::log_command(const std::string& msg) 
{
	Messenger::expose(ok,msg,true);	
}

//Messenger::expose(err,"Error: ... ")
	

