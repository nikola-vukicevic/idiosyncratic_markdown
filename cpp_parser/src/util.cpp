/* -------------------------------------------------------------------------- */
// Copyright (c) Nikola Vukićević 2025.
/* -------------------------------------------------------------------------- */
#include "util.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <unicode/umachine.h>
#include <unicode/unistr.h>
#include <vector>
/* -------------------------------------------------------------------------- */
# define ASCII_GORNJA_GRANICA 127
/* -------------------------------------------------------------------------- */
std::string stringJoin(std::vector<std::string> vect, std::string delimiter) {
	std::string s = "";

	for (auto line : vect) {
		s += line + delimiter;
	}

	return s;
}
/* -------------------------------------------------------------------------- */
bool stringStartsWith(std::string s, std::string pattern) {
	return s.find(pattern) == 0;
}
/* -------------------------------------------------------------------------- */
bool daLiJeAsciiSlovo(char c) {
	return std::tolower(c) != toupper(c);
}
/* -------------------------------------------------------------------------- */
bool daLiJeUnicodeSlovo(uint kodnaPozicija) {
	return u_toupper(kodnaPozicija) != u_tolower(kodnaPozicija);
}
/* -------------------------------------------------------------------------- */
bool daLiJeCifra(char c) {
	return c >= '0' && c <= '9';
}
/* -------------------------------------------------------------------------- */
std::string proveraIda(std::string s, ParserInfo &parser) {
	int i = parser.mapa[s];

	if (i > 0) {
		++i;
		parser.mapa[s] = i;
		return s + "__" + std::to_string(i);
	} else {
		parser.mapa[s] = 1;
		return s;
	}
}
/* -------------------------------------------------------------------------- */
void ucitavanjeUnicodeZnaka(std::string s, uint &i, std::string &znak) {
	while (s[i] < 0) { // > ASCII_GORNJA_GRANICA) {
		znak += s[i];
		++i;
	}
}
/* -------------------------------------------------------------------------- */
std::string obradaUnicodeZnaka(std::string s) {
	// std::cout << s << std::endl;
	std::vector<uint> kodnePozicije;
	icu::UnicodeString strPom = icu::UnicodeString::fromUTF8(s);

	for (UChar c : strPom) {
		if (daLiJeUnicodeSlovo(c)) {
			// std::cout << c << " ";
			kodnePozicije.push_back(c);
		}
	}

	// std::cout << std::endl;

	icu::UnicodeString strOutPom;
	std::string strOut;

	for (int32_t kodnaPozicija : kodnePozicije) {
		strOutPom.append(kodnaPozicija);
	}

	strOutPom.toLower().toUTF8String(strOut);

	return strOut; // if (k)
}
/* -------------------------------------------------------------------------- */
std::string generisanjeIda(std::string s, ParserInfo &parser) {
	std::string s_p = "";
	uint i = 0;

	while (i < s.length()) {
		char c = s[i];

		if (c == ' ') {
			s_p += "_";
			++i;
			continue;
		}

		if (c < 0) { // > ASCII_GORNJA_GRANICA) {
			std::string unicodeZnak = "";
			ucitavanjeUnicodeZnaka(s, i, unicodeZnak);
			s_p += obradaUnicodeZnaka(unicodeZnak);
			continue;
		}

		if (!daLiJeCifra(c) && !daLiJeAsciiSlovo(c)) {
			++i;
			continue;
		}

		s_p += std::tolower(c);

		++i;
	}

	return proveraIda(s_p, parser);
}
/* -------------------------------------------------------------------------- */
void pisanjeUDatoteku(std::string putanja, std::string tekst) {
	std::ofstream datoteka(putanja);
	datoteka << tekst;
	datoteka.close();
}
/* -------------------------------------------------------------------------- */

