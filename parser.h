#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>

namespace parser {

class ParserNumber {
  public:
    ParserNumber(int number = 0);
    ~ParserNumber(void);

    void SetNumber(int number);
    std::string GetPt(void);

  protected:
    std::string number_convert(int number, std::string suffix);
    std::string hundred_number_convert(int number);
  private:
    int _number;
};

} // namespace parser

#endif // _PARSER_H_
