#ifndef icommand_base_hpp
#define icommand_base_hpp

class icommand_base
{
    public:
        void virtual execute() {
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
        //void virtual execute_and_log() = 0;
        void virtual process() = 0;
        void virtual mouse_clicked(int, int) = 0;
        void virtual mouse_dbl_clicked(int, int) = 0;
        void virtual mouse_moved(int, int) = 0;
        void virtual update() = 0;
        //void virtual log() = 0;
        
        //FIXME bug, pure virtual dtor makes compiler sad:/
        virtual ~icommand_base() {}
};

#endif
