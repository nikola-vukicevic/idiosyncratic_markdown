# ------------------------------------------------------------------------------
# Copyright (C) 2021. Nikola Vukićević
# ------------------------------------------------------------------------------

import re
import os

def idiosyncParse(s):
	redovi = s.split("\n")
	print(redovi)
	tokeni = []
	s      = ""
	parse  = True

	for r in redovi:
		if r == "\r" or r == "":
			continue
		
		if parse == True:
			if r.startswith("``"):
				parse = False
				continue
			ucitavanje_tokena(r, tokeni)			
		else:
			if r.startswith("~~"):
				parse = True
				tokeni.append(["", "", ""])
				continue
			tokeni.append([r, "", ""])

	for t in tokeni:
		s = s + t[0] + t[1] + t[2] + "\n"

	return s

# ------------------------------------------------------------------------------

def ucitavanje_tokena(r, tokeni):
	
	if r.startswith("!!"):
		return
	
	# Za sada mi ne trebaju h5 i h6, ali, ako zatreba ....
	# if r.startswith("######"):
	# 	tokeni.append(["<h6>", r.lstrip('#').strip(), "</h6>\n"])
	# 	continue
	# if r.startswith("#####"):
	# 	tokeni.append(["<h5>", r.lstrip('#').strip(), "</h5>\n"])
	# 	continue
	
	if r.startswith("####"):
		tokeni.append(["<h4>", r.lstrip('#').strip(), "</h4>\n"])
		return
	if r.startswith("###"):
		tokeni.append(["<h3>", r.lstrip('#').strip(), "</h3>\n"])
		return
	if r.startswith("##"):
		tokeni.append(["<h2>", r.lstrip('#').strip(), "</h2>\n"])
		return
	if r.startswith("#"):
		tokeni.append(["<h1>", r.lstrip('#').strip(), "</h1>\n"])
		return
	if r.startswith("**"):
		tokeni.append(["</ul>\n", "",        ""])
		return
	if r.startswith("*"):
		tokeni.append(["<ul>", "",        ""])
		return
	if r.startswith("\t"):
		tokeni.append(["\t<li>", r.strip(), "</li>"])
		return

	# Ako red nije ništa 'posebno', onda je <p> :)
	tokeni.append(["<p>\n\t", r.strip(), "\n</p>\n"])

# --------------------------------------------------
# Automatska numeracija slika u tekstu. Za ovo
# koristim i posebne snipete, a sve zajedno to
# verovatno ne bi bilo zanimljivo svima, pa se
# zato metoda ne poziva po default-u
# --------------------------------------------------

def zamena_tokena_slike(s):
    lista = re.split("#s#", s)

    n = ""
    i = 1

    for l in lista:
    	n = n + l + str(i)
    	i = i + 1

    return n.rstrip('0123456789\n ') + "\n"

# ------------------------------------------------------------------------------

def ucitavanje():
	f = open("ulaz.txt", "rb")
	s = f.read().decode("utf-8")
	f.close()
	return s

# ------------------------------------------------------------------------------

def pisanje(s):
	s = s.encode("utf-8")
	f = open("izlaz.html", "wb")
	f.write(s)
	f.close()
	return

# ------------------------------------------------------------------------------

s = ucitavanje()

s = idiosyncParse(s)
s = zamena_tokena_slike(s)

pisanje(s)

# ------------------------------------------------------------------------------
