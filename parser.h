#ifndef _PARSER_H_
#define _PARSER_H_
#include <string>

namespace parser {

class ParserNumber {
  public:
    ParserNumber(int number);
    ~ParserNumber(void);

    std::string GetPt(void);

  private:
    std::string number_convert(int number, std::string suffix);
    std::string hundred_number_convert(int number, std::string suffix);

    int _number;
};

} // namespace parser

#endif // _PARSER_H_
