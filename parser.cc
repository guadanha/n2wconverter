#include <iostream>
#include <string>

#include "parser.h"

namespace parser {

const std::string EMPTY = "";

const std::string low[] = { EMPTY, "um ", "dois ", "tres ", "quatro ", "cinco ",
    "seis ", "sete ", "oito ", "nove ", "dez ", "onze ",
    "doze ", "treze ", "quatorze ", "quize ",
    "dezesseis ", "dezessete ", "dezoito ", "dezenove " };

const std::string med[] = { EMPTY, EMPTY, "vinte ", "trinta ", "quarenta ", "cinquenta ",
    "sessenta ", "setenta ", "oitenta ", "noventa " };

const std::string high[] = { EMPTY, "cento ", "duzentos ", "trezentos ", "quatrocentos ",
    "quinhentos ", "seissentos ", "setecentos ", "oitocentos ", "novecentos " };

ParserNumber::ParserNumber(int number) : _number(number) {

}

ParserNumber::~ParserNumber(void) {

}


std::string ParserNumber::number_convert(int number, std::string suffix) {
    if (number == 0) {
        return EMPTY;
    }

    if (number < 20) {
        return low[number] + suffix;
    }

    if (number % 10 ) {
        return med[number / 10] +  "e " + low[number % 10] + suffix;
    }

    return med[number / 10] + suffix;
}

std::string ParserNumber::hundred_number_convert(int number, std::string suffix) {
    if (number == 0) {
        return EMPTY;
    }
    return high[number];
}

std::string ParserNumber::GetPt(void) {
    int number = _number;
    std::string res;

    if (number > 99999 || number < -99999) return EMPTY;

    if (number == 0) return "zero";
    else if (number < 0) {
        res += "menos ";
        number = -number;
    }

    if ((number/1000) == 1) {
        res += "mil ";
    } else {
        res += number_convert(((number / 1000) % 100), "mil ");
    }
    if (number > 1000 && number % 1000) {
        res += "e ";
    }

    if ((number % 1000) == 100) {
        res += "cem ";
    } else {
        res += hundred_number_convert(((number / 100) % 10), "");
    }

    if (number > 100 && number % 100 && (number % 1000) > 100) {
        res += "e ";
    }

    res += number_convert((number % 100), "");
    res.erase(res.size() - 1);
    return res;
}

}
