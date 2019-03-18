#ifndef icommand_base_hpp
#define icommand_base_hpp

class icommand_base
{
    public:
        virtual void execute() {
                //while ( isFinished() ) {
                //	process();
                //}
                //if (LOG_MODE) 
                //	execute();
                //	log();
                //else
                //	execute_and_log();
        }
        
        //bool virtual isFinished() = 0;
        //virtual void execute_and_log() = 0;
        virtual void process() = 0;
        virtual void mouse_clicked(int, int) = 0;
        virtual void mouse_dbl_clicked(int, int) = 0;
        virtual void mouse_moved(int, int) = 0;
        virtual void update() = 0;
        //virtual void log() = 0;
        
        //FIXME bug, pure virtual dtor makes compiler sad:/
        virtual ~icommand_base() {}
};

#endif
