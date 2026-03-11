/* -------------------------------------------------------------------------- */
// Copyright (c) Nikola Vukićević 2022.
/* -------------------------------------------------------------------------- */
import fs from 'fs'
import * as gen_s from './generisanje_sadrzaja.js'
/* -------------------------------------------------------------------------- */
const mapaNaslova = new Map();
/* -------------------------------------------------------------------------- */
function RanaTerminacija(red) {
	if (red.startsWith("!!")) return true
    if (red == "")            return true
    if (red == "\r")          return true
    // if (red == "\n")          return true

	return false
}
/* -------------------------------------------------------------------------- */
function uklanjanjeTaraba(s1) {
	let s2 = "";
	let i  = 0;

	while (s1[i] == '#') ++i;
	++i;
	
	while (i < s1.length) {
		s2 += s1[i];
		++i;
	}

	return s2;
}
/* -------------------------------------------------------------------------- */
function obradaNaslov(s, nivo, lista, mapa) {
	let s_p = uklanjanjeTaraba(s)
	let id  = gen_s.generisanjeIda(s_p, mapa)

	lista.push(`<h${nivo} id="${id}">${s_p}</h${nivo}>\n\n`)
}
/* -------------------------------------------------------------------------- */
function ispitivanjeUslovaHeading(s) {
	// NIKAKO ne menjati redosled za ** i * !!!!!
	if (s.startsWith("####")) return { naslov: true, nivo: 4 }
	if (s.startsWith("###"))  return { naslov: true, nivo: 3 }
	if (s.startsWith("##"))   return { naslov: true, nivo: 2 }
	if (s.startsWith("#"))    return { naslov: true, nivo: 1 }
	return { naslov: false, nivo: 0 }
}
/* -------------------------------------------------------------------------- */
function upisivanjeTokena(red, tokeni, mapa) {
	if (red == "\n") return

	const uslovH = ispitivanjeUslovaHeading(red)
	
	if (uslovH.naslov) {
		obradaNaslov(red, uslovH.nivo, tokeni, mapa)
		return
	}

	if(red.startsWith("**")){
		tokeni.push("</ul>\n\n")
		return
	}
	
	if(red.startsWith("*")){
		tokeni.push("<ul>\n")
		return
	}
	
	if(red.startsWith("\t")){
		tokeni.push(`\t<li>${red.trim()}</li>\n`)
		return
	}

	tokeni.push(`<p>\n\t${red.trim()}\n</p>\n\n`)
}
/* -------------------------------------------------------------------------- */
// function upisivanjeTokena_STARA_VERZIJA(red, tokeni) {
// 	if (red == "\n") return
//
// 	if(red.startsWith("####")) {
// 		tokeni.push(`<h4>${red.replace("####", "").trim()}</h4>\n\n`)
// 		return
// 	}
// 	
// 	if(red.startsWith("###")) {
// 		tokeni.push(`<h3>${red.replace("###", "").trim()}</h3>\n\n`)
// 		return
// 	}
// 	
// 	if(red.startsWith("##")) {
// 		tokeni.push(`<h2>${red.replace("##", "").trim()}</h2>\n\n`)
// 		return
// 	}
// 	
// 	if(red.startsWith("#")) {
// 		tokeni.push(`<h1>${red.replace("#", "").trim()}</h1>\n\n`)
// 		return
// 	}
//
// 	// NIKAKO ne menjati redosled za ** i * !!!!!
// 	if(red.startsWith("**")){
// 		tokeni.push("</ul>\n\n")
// 		return
// 	}
// 	
// 	if(red.startsWith("*")){
// 		tokeni.push("<ul>\n")
// 		return
// 	}
// 	
// 	if(red.startsWith("\t")){
// 		tokeni.push(`\t<li>${red.trim()}</li>\n`)
// 		return
// 	}
//
// 	tokeni.push(`<p>\n\t${red.trim()}\n</p>\n\n`)
// }
/* -------------------------------------------------------------------------- */
function obradaTokena(token, lista, wrapper, mapa) {
	if (RanaTerminacija(token)) return;

	if (wrapper[0] == true) {
		if (token.startsWith("``")) {
			wrapper[0] = false;
			wrapper[1] = 0;
			return;
		}
		upisivanjeTokena(token, lista, mapa);
	} else {
		wrapper[1] += 1;

		if (token.startsWith("~~")) {
			lista.push("\n");
			wrapper[0] = true;
			return;
		}

		if (wrapper[1] < 2) return;

		lista.push(token);
	}
}
/* -------------------------------------------------------------------------- */
function zamenaTokenaSlike(s) {
	let lista = s.split("#s#");
	let n = ""
	let i = 0

	while(i < lista.length - 1) {
		n += lista[i] + `${i+1}`
		i++
	}

	return n + lista[i]
}
/* -------------------------------------------------------------------------- */
function idiosyncParse(s, mapa) {
    let regex  = /(\r|\n)/im
    let redovi = s.split(regex)
    // console.log(redovi)
    let tokeni  = [ ]
    let parse   = true
    let brojac  = 0
    let wrapper = [ parse, brojac ]

    redovi.forEach(red => {
        obradaTokena(red, tokeni, wrapper, mapa)
        // brojac = obradaTokena(red, tokeni, wrapper)
	})

    return tokeni.join("")
}
/* -------------------------------------------------------------------------- */
function ucitavanje(datoteka) {
	try {
		const sadrzaj = fs.readFileSync(datoteka, 'utf8');
		return sadrzaj
		
	} catch (err) {
		console.error(`Greška pri čitanju datoteke ${datoteka}.`);
		console.error(err);
	}
}
/* -------------------------------------------------------------------------- */
function pisanje(datoteka, s) {
	try {
		fs.writeFileSync(datoteka, s, 'utf8')
	}
	catch(err) {
		console.error(err)
		console.log(`Greška pri upisu u datoteku ${datoteka}`)
		return
	}
}
/* -------------------------------------------------------------------------- */
let datotekeIzlaz = [ ]
let s             = ""
/* -------------------------------------------------------------------------- */
let stdinBuffer = fs.readFileSync(0).toString().split("\n")

stdinBuffer.forEach(line => {
	if(line.startsWith("@@")) {
		datotekeIzlaz.push(line.replace("@@", "").trim())
	}
	else {
		s += `${line}\n`
	}
})
/* -------------------------------------------------------------------------- */
let T1 = performance.now()
if (datotekeIzlaz.length > 0) {
	s = idiosyncParse(s, mapaNaslova)
	s = zamenaTokenaSlike(s)

	datotekeIzlaz.forEach(datoteka => {
		pisanje(datoteka, s)
		console.log("Idiosync parser (Node.js)")
		console.log("Ulaz: Vim buffer")
		console.log(`Izlaz: datoteka ${datoteka}`)
	})
}
else {
	console.error("Greška! Izlazna datoteka nije definisana.")
}
let T2 = performance.now()
console.log(`${(T2 - T1).toFixed(2)}ms`)
/* -------------------------------------------------------------------------- */

