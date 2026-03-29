/* -------------------------------------------------------------------------- */
package idiosync_parser_odin
/* -------------------------------------------------------------------------- */
import "core:os"
import "core:fmt"
import "core:strings"
import "core:unicode"
import "core:unicode/utf8"
/* -------------------------------------------------------------------------- */
MapaNaslovi :: map[string]uint
/* -------------------------------------------------------------------------- */
NaslovInfo :: struct {
	da_li_je_naslov: bool,
	nivo:            byte,
	nivo_str:        string,
}
/* -------------------------------------------------------------------------- */
ParserInfo :: struct {
	parse:            bool,
	brojac_slika:     uint,
	redovi:           [dynamic]string,
	izlazne_datoteke: [dynamic]string,
	mapa:             MapaNaslovi,
}
/* -------------------------------------------------------------------------- */
util_ispis_redova :: proc(parser: ParserInfo) {
	for red in parser.redovi {
		fmt.println(red)
	}
}
/* -------------------------------------------------------------------------- */
provera_ida :: proc(red: string, parser: ^ParserInfo) -> string {
	i := parser.mapa[red]

	if i > 0 {
		i += 1
		parser.mapa[red] = i
		buff: [2]u8 // 3 cifre -> max 99 istih naslova xD
		return strings.concatenate({ red , "__", fmt.bprint(buff[:], i) }, context.temp_allocator)
	} else {
		parser.mapa[red] = 1
		return red
	}
}
/* -------------------------------------------------------------------------- */
generisanje_ida :: proc(red: string, parser: ^ParserInfo) -> string {
	str_p := ""
	i     := 0

	for znak in red { // znak je rune
		if znak == ' ' {
			str_p  = strings.concatenate({ str_p, "_" }, context.temp_allocator)
			i     += 1
			continue
		}

		if !unicode.is_alpha(znak) && !unicode.is_digit(znak) {
			continue
		}

		buffer: [4]u8
		size:   int
		buffer, size = utf8.encode_rune(unicode.to_lower(znak))

		p := string(buffer[:size])
		str_p = strings.concatenate({ str_p, p }, context.temp_allocator)
	}

	return provera_ida(str_p, parser)
}
/* -------------------------------------------------------------------------- */
pisanje_u_datoteku :: proc(putanja: string, parser: ParserInfo) {
	str_bld: strings.Builder
	strings.builder_init(&str_bld)

	for red in parser.redovi {
		strings.write_string(&str_bld, red)
	}

	res := os.write_entire_file_from_string(putanja, strings.to_string(str_bld))
}
/* -------------------------------------------------------------------------- */

