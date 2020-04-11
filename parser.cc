#include <iostream>
#include <string>

#include "parser.h"

namespace parser {

const std::string EMPTY = "";

const std::string pt_low[] = { EMPTY, "um ", "dois ", "tword ", "quatro ", "cinco ",
    "seis ", "sete ", "oito ", "nove ", "dez ", "onze ",
    "doze ", "treze ", "quatorze ", "quize ",
    "dezesseis ", "dezessete ", "dezoito ", "dezenove " };

const std::string pt_med[] = { EMPTY, EMPTY, "vinte ", "trinta ", "quarenta ", "cinquenta ",
    "sessenta ", "setenta ", "oitenta ", "noventa " };

const std::string pt_high[] = { EMPTY, "cento ", "duzentos ", "trezentos ", "quatrocentos ",
    "quinhentos ", "seissentos ", "setecentos ", "oitocentos ", "novecentos " };


const std::string en_low[] = { EMPTY, "one ", "two ", "three ", "four ", "five ",
    "six ", "seven ", "eight ", "nine ", "ten ", "eleven ",
    "twelve ", "thirteen ", "fourteen ", "fifteen ",
    "sixteen ", "seventeen ", "eighteen ", "nineteen " };

const std::string en_high[] = { EMPTY, EMPTY, "twenty ", "thirty ", "forty ", "fifty ",
    "sixty ", "seventy ", "eighty ", "ninety " };

ParserNumber::ParserNumber(int number) : _number(number) {

}

ParserNumber::~ParserNumber(void) {

}

void ParserNumber::SetNumber(int number) {
    _number = number;
}

std::string ParserNumber::pt_number_convert(int number, std::string suffix) {
    if (number == 0) {
        return EMPTY;
    }

    if (number < 20) {
        return pt_low[number] + suffix;
    }

    if (number % 10 ) {
        return pt_med[number / 10] +  "e " + pt_low[number % 10] + suffix;
    }

    return pt_med[number / 10] + suffix;
}

std::string ParserNumber::pt_hundred_number_convert(int number) {
    if (number == 0) {
        return EMPTY;
    }
    return pt_high[number];
}

std::string ParserNumber::en_number_convert(int number, std::string suffix) {
    if (number == 0) {
        return EMPTY;
    }

    if (number > 19) {
        return en_high[number / 10] + en_low[number % 10] + suffix;
    }
    else {
        return en_low[number] + suffix;
    }
}

std::string ParserNumber::GetPt(void) {
    int number = _number;
    std::string word;

    if (number > 99999 || number < -99999) return EMPTY;

    if (number == 0) return "zero";
    else if (number < 0) {
        word += "menos ";
        number = -number;
    }

    if ((number/1000) == 1) {
        word += "mil ";
    } else {
        word += pt_number_convert(((number / 1000) % 100), "mil ");
    }
    if (number > 1000 && number % 1000) {
        word += "e ";
    }

    if ((number % 1000) == 100) {
        word += "cem ";
    } else {
        word += pt_hundred_number_convert(((number / 100) % 10));
    }

    if (number > 100 && number % 100 && (number % 1000) > 100) {
        word += "e ";
    }

    word += pt_number_convert((number % 100), "");
    word.erase(word.size() - 1);
    return word;
}

std::string ParserNumber::GetEn(void) {
    int number = _number;
    std::string word;

    if (number > 99999 || number < -99999) return EMPTY;

    if (number == 0) return "zero";
    else if (number < 0) {
        word += "minus ";
        number = -number;
    }
    word += en_number_convert(((number / 1000) % 100), "thousand ");

    word += en_number_convert(((number / 100) % 10), "hundred ");
    word += en_number_convert((number % 100), "");

    word.erase(word.size() - 1);
    return word;
}

}
