#ifndef messenger_hpp
#define messenger_hpp

#include "../core/service.hpp"
#include "../core/callback.hpp"

#include <QFile>
#include <QDir>
#include <QTextStream>

#include <string>
#include <functional>

enum LogMsgSeverity { err=1, usr, warn, ok, out, test, cont, modal, info };

class MessengerCallbackData : public LeCallbackData 
{
    std::string m_msg;
    std::string m_ecodemsg;
    LogMsgSeverity m_sev;
    
    public:
        MessengerCallbackData(const LogMsgSeverity& s, const std::string& msg, const std::string& ecodemsg):m_sev(s),m_msg(msg),m_ecodemsg(ecodemsg) {}
        std::string get_message() { return m_msg; }
        std::string get_errorcode() { return m_ecodemsg; }
        
        LogMsgSeverity get_severity() { return m_sev; }
        
        
};

class Messenger : public Service<Messenger>
{    

	QDir* m_dir; 
	QFile* m_cmdfile;
	QFile* m_logfile;
	QTextStream* log_stream;
	QTextStream* cmd_stream;

public:
        static void expose_msg(const LogMsgSeverity& s, const std::string& msg, bool iscmd = false );
        static void log_command(const std::string& msg, bool iscmd);
        static std::string decorate_for_logging(const LogMsgSeverity&);
	//FIXME
	~Messenger();
		
        
private:
	void init();
	void fini();
        
        void write_entry_to_logfile(const std::string& msg);
        void write_entry_to_cmdfile(const std::string& msg);
        void write_entry_to_console_gui(const LogMsgSeverity& s, const std::string& msg);   
        void expose_internal(const LogMsgSeverity& s, const std::string& msg, bool iscmd = false);
            
public:
	std::function<void(const std::string&)> m_console_callback = nullptr;
};

#endif
