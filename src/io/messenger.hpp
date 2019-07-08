#ifndef messenger_hpp
#define messenger_hpp

#include "../core/service.h"
#include "../core/callback.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <string>

enum LogMsgSeverity { err=1, warn, ok, out };

class Messenger : public Service<Messenger>
{    

	QDir* m_dir; 
	QFile* m_cmdfile;
	QFile* m_logfile;
	QTextStream* log_stream;
	QTextStream* cmd_stream;

	public:
		static void expose(const LogMsgSeverity& s, const std::string& msg, bool iscmd = false );
		static void log_command(const std::string& msg);
		//FIXME
		Messenger();
		void test(LeCallbackData& d);
	
	private:
		~Messenger();
		void init();
		void fini();
		std::string decorate(const LogMsgSeverity&);
		
	public:
        void expose_internal(const LogMsgSeverity& s, const std::string& msg, bool iscmd = false);
            
};

#endif
