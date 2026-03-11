/* -------------------------------------------------------------------------- */
// Copyright (c) Nikola Vukićević 2025.
/* -------------------------------------------------------------------------- */
#include <bits/stdc++.h>
// #include <iostream>
#include <boost/algorithm/string/trim.hpp>
#include <chrono>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "util.h"
/* -------------------------------------------------------------------------- */
// #define DEBUG
/* -------------------------------------------------------------------------- */
bool ranaTerminacija(std::string red) {
	if (stringStartsWith(red, "!!")) return true;
	if (red == "")             return true;
	if (red == "\r")           return true;
	return false;
}
/* -------------------------------------------------------------------------- */
void obradaNaslov(std::string red, NaslovInfo info, ParserInfo &parser) {
	// std::cout << "'" + red + "'" << std::endl;
	std::string s_p = red.substr(info.nivo + 1); // uklanjanjeTaraba()
	std::string id  = generisanjeIda(s_p, parser);

	parser.redovi.push_back("<h" + info.nivo_str + " id=\"" + id + "\">" + s_p + "</h" + info.nivo_str + ">\n\n");
}
/* -------------------------------------------------------------------------- */
NaslovInfo ispitivanjeUslovaHeading(std::string s) {
	NaslovInfo info;

	info.nivo         = 0;
	info.nivo_str     = "0";
	info.daLiJeNaslov = false;

	if (stringStartsWith(s, "####")) {
		info.daLiJeNaslov = true;
		info.nivo_str     = "4";
		info.nivo         = 4;
	} else if (stringStartsWith(s, "###")) {
		info.daLiJeNaslov = true;
		info.nivo_str     = "3";
		info.nivo         = 3;
	} else if (stringStartsWith(s, "##")) {
		info.daLiJeNaslov = true;
		info.nivo_str     = "2";
		info.nivo         = 2;
	} else if (stringStartsWith(s, "#")) {
		info.daLiJeNaslov = true;
		info.nivo_str     = "1";
		info.nivo         = 1;
	}

	return info;
}
/* -------------------------------------------------------------------------- */
bool obradaIndeksaSlike(std::string &red, ParserInfo &parser) {
	int indeks = -1;
	std::string slika_pattern = "#s#";

	indeks = red.find(slika_pattern);

	if (indeks != -1) {
		// std::cout << "----------\n";
		// std::cout << std::to_string(parser.brojacSlika) << std::endl;
		// std::cout << red << std::endl;
		red = std::regex_replace(red, std::regex(slika_pattern), std::to_string(parser.brojacSlika));
		// TODO
		parser.brojacSlika++;
		return true;
	} else {
		return false;
	}
}
/* -------------------------------------------------------------------------- */
void upisivanjeTokena(std::string red, ParserInfo &parser) {
	if (red == "\n") return;

	if (stringStartsWith(red, "\t")) {
		boost::trim(red);
		parser.redovi.push_back("\t<li>" + red + "</li>\n");
		return;
	}

	boost::trim(red);

	NaslovInfo info = ispitivanjeUslovaHeading(red);

	if (info.daLiJeNaslov) {
		obradaNaslov(red, info, parser);
		return;
	}

	if (stringStartsWith(red, "**")) {
		parser.redovi.push_back("</ul>\n\n");
		return;
	}

	if (stringStartsWith(red, "*")) {
		parser.redovi.push_back("<ul>\n");
		return;
	}

	if (stringStartsWith(red, "\t")) {
		parser.redovi.push_back("\t<li>" + red.substr(1) + "</li>\n");
		return;
	}

	if (stringStartsWith(red, "@@")) {
		parser.izlazneDatoteke.push_back(red.substr(2));
		return;
	}

	parser.redovi.push_back("<p>\n\t" + red + "\n</p>\n\n");
}
/* -------------------------------------------------------------------------- */
void obradaReda(std::string red, ParserInfo &parser) {
	if (parser.parse) {
		if (stringStartsWith(red, "``")) {
			parser.parse        = false;
			// parser.brojacRedova = 0;
			return;
		}

		if (ranaTerminacija(red)) return;

		upisivanjeTokena(red, parser);
	} else {
		// parser.brojacRedova += 1; 

		if (stringStartsWith(red, "~~")) {
			parser.redovi.push_back("\n");
			parser.parse = true;
			return;
		}

		if (obradaIndeksaSlike(red, parser)) {
			parser.redovi.push_back(red + "\n");
			return;
		}

		// if (parser.brojacRedova < 2) return;

		parser.redovi.push_back(red + "\n");
	}
}
/* -------------------------------------------------------------------------- */
void ucitavanjeStruktureParser(ParserInfo &parser) {
	#ifdef DEBUG
	std::string putanja = "/home/korisnik/git/idiosync_parser/proba.imd";
	std::ifstream datoteka(putanja);
	std::string red;

	while (getline(datoteka, red)) {
		obradaReda(red, parser);
	}

	parser.izlazniTekst = stringJoin(parser.redovi, "");

	#endif

	#ifndef DEBUG
	// obrada pajpovanog ulaza iz konzole:
	while(!std::cin.eof()) {
		std::string red;
		getline(std::cin, red);
		obradaReda(red, parser);
	}

	parser.izlazniTekst = stringJoin(parser.redovi, "");
	#endif
}
/* -------------------------------------------------------------------------- */
void idiosyncParse() {
	ParserInfo parser;
	parser.parse        = true;
	parser.brojacSlika  = 1;
	// parser.brojacRedova = 0;
	parser.izlazniTekst = "";

	ucitavanjeStruktureParser(parser);

	if (parser.izlazneDatoteke.empty()) {
		std::cout << "Greška! Izlazna datoteka nije definisana." << std::endl;
		return;
	}

	auto t1 = std::chrono::high_resolution_clock::now();

	for (auto putanja : parser.izlazneDatoteke) {
		std::cout << "---------------------" << std::endl;
		std::cout << "Idiosync parser (C++)" << std::endl;
		std::cout << "---------------------" << std::endl;
		pisanjeUDatoteku(putanja, parser.izlazniTekst);
		std::cout << "Ulaz: Vim buffer" << std::endl;
		std::cout << "Izlaz: datoteka " + putanja << std::endl;
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto t3 = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);
	std::cout << "Vreme: " << (double) t3.count() / 1000 << "ms";
}
/* -------------------------------------------------------------------------- */
int main (int argc, char *argv[]) {
	idiosyncParse();
	return 0;
}
/* -------------------------------------------------------------------------- */

