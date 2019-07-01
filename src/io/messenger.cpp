#include "messenger.hpp"


#include <QString>

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
	m_dir = new QDir;//;("logs");
	
	if (!m_dir->exists(m_path))
	m_dir->mkpath(m_path); // You can check the success if needed

	m_cmdfile = new QFile(m_path + "/painter.out");
	m_logfile = new QFile(m_path + "/painter.log");
	
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
		default:
			break;
		}
}

//FIXME
void Messenger::expose_internal(const LogMsgSeverity& s, const std::string& msg , bool cmd) 
{
	std::stringstream z;
	z << decorate(s) << msg << "\n";
	
	m_cmdfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	m_logfile->open( QIODevice::WriteOnly | QIODevice::Append ); 
	
	log_stream = new QTextStream(m_logfile);
	cmd_stream = new QTextStream(m_cmdfile);

	(*log_stream) << z.str().c_str();
	
	if ( cmd )
		(*cmd_stream) << msg
	
	m_cmdfile->flush();
	m_logfile->flush();

	m_cmdfile->close();
	m_logfile->close();
	
	std::cout << z.str();
		
}
			
void Messenger::expose(const LogMsgSeverity& s, const std::string& msg)
{
	Messenger::get_instance()->expose_internal(s,msg);
}
    	
void Messenger::log_command(const std::string& msg) 
{
	Messenger::expose(ok,msg,true);	
}

	

