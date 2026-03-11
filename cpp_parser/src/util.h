/* -------------------------------------------------------------------------- */
// Copyright (c) Nikola Vukićević 2025.
/* -------------------------------------------------------------------------- */
#ifndef UTIL
#define UTIL
/* -------------------------------------------------------------------------- */
#include <bits/stdc++.h>
// #include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <unicode/unistr.h>
#include <unicode/urename.h>
#include <unicode/uchar.h>
/* -------------------------------------------------------------------------- */
typedef std::map<std::string, int> MapaNaslovi;
/* -------------------------------------------------------------------------- */
typedef struct {
	bool daLiJeNaslov;
	int nivo;
	std::string nivo_str;	
} NaslovInfo;
/* -------------------------------------------------------------------------- */
typedef struct {
	bool parse;
	int brojacSlika;
	std::vector<std::string> redovi;
	std::vector<std::string> izlazneDatoteke;
	std::string izlazniTekst;
	MapaNaslovi mapa;
	// int brojacRedova; // TODO - možda nepotrebno?!?!
} ParserInfo;
/* -------------------------------------------------------------------------- */
std::string stringJoin(std::vector<std::string> vect, std::string delimiter);
/* -------------------------------------------------------------------------- */
bool stringStartsWith(std::string s, std::string pattern);
/* -------------------------------------------------------------------------- */
bool daLiJeAsciiSlovo(char c);
/* -------------------------------------------------------------------------- */
bool daLiJeUnicodeSlovo(uint kodnaPozicija);
/* -------------------------------------------------------------------------- */
bool daLiJeCifra(char c);
/* -------------------------------------------------------------------------- */
void ucitavanjeUnicodeZnaka(std::string s, uint &i, std::string &znak);
/* -------------------------------------------------------------------------- */
std::string obradaUnicodeZnaka(std::string);
/* -------------------------------------------------------------------------- */
std::string proveraIda(std::string s, ParserInfo &parser);
/* -------------------------------------------------------------------------- */
std::string generisanjeIda(std::string s, ParserInfo &parser);
/* -------------------------------------------------------------------------- */
void pisanjeUDatoteku(std::string putanja, std::string tekst);
/* -------------------------------------------------------------------------- */
#endif
/* -------------------------------------------------------------------------- */
