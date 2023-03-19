#ifndef modal_dialog_hpp
#define modal_dialog_hpp

#include <string>

class mmModalDialog
{

  public:
    static bool ask_yn_question(const std::string &tl, const std::string &bd);
    static bool critical(const std::string &tl, const std::string &bd);

  private:
};

#endif
