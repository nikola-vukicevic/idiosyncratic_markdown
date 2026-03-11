/* -------------------------------------------------------------------------- */
// Copyright (c) Nikola Vukićević 2022.
/* -------------------------------------------------------------------------- */
function daLiJeSlovo(c) {
  return c.toLowerCase() != c.toUpperCase();
}
/* -------------------------------------------------------------------------- */
function daLiJeCifra(c) {
	return c >= '0' && c <= '9';
}
/* -------------------------------------------------------------------------- */
function proveraIda(s, mapa) {
	let i = mapa.get(s)

	if (i) {                  // Ako h2 već postoji u mapi ....
		++i;                  // .... indeks se uvećava.
		mapa.set(s, i);
		return `${s}__${i}`;
	} else {                  // Ako se h2 pojavljuje prvi put ....
		mapa.set(s, 1);       // .... ubacuje se u mapu.
		return s;
	}
}
/* -------------------------------------------------------------------------- */
export function generisanjeIda(s, mapa) {
	let s_p = "";

	for(let i = 0; i < s.length; i++) {
		let z = s[i];

		if(z == ' ') {
			s_p += "_";
			continue;
		}

		if(!daLiJeCifra(z) && !daLiJeSlovo(z)) continue;

		s_p += z.toLowerCase();
	}

	return proveraIda(s_p, mapa);
}
/* -------------------------------------------------------------------------- */

