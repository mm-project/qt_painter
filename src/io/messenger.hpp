#ifndef messenger_hpp
#define messenger_hpp

#include "../core/service.h"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <string>

enum LogMsgSeverity { err=1, warn, ok };

class Messenger : public Service<Messenger>
{    
    std::string m_logname;
	QDir* m_dir; 
	QFile* m_cmdfile;
	QFile* m_logfile;
	QTextStream* log_stream;//(&file)
	QTextStream* cmd_stream;//(&file)

	public:
		static void expose(const LogMsgSeverity& s, const std::string& msg);
		static void log_command(const std::string& msg);
		//FIXME
		Messenger();
	private:
		
		~Messenger();
		void init();
		void fini();
		std::string decorate(const LogMsgSeverity&);
		
	public:
        void expose_internal(const LogMsgSeverity& s, const std::string& msg);
            
};

#endif
