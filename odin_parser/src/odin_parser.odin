/* -------------------------------------------------------------------------- */
package idiosync_parser_odin
/* -------------------------------------------------------------------------- */
import "core:fmt"
import "core:time"
import "core:strings"
import "core:os"
/* -------------------------------------------------------------------------- */
DEBUG :: false
/* -------------------------------------------------------------------------- */
rana_terminacija :: proc(red: string) -> bool {
	if strings.has_prefix(red, "!!") do return true
	if red == ""   do return true
	if red == "\r" do return true
	
	return false
}
/* -------------------------------------------------------------------------- */
obrada_naslov :: proc(red: string, info: NaslovInfo, parser: ^ParserInfo) {
	s_p      := red[info.nivo + 1 : ]
	id       := generisanje_ida(s_p, parser)
	novi_red := strings.concatenate({ "<h" , info.nivo_str , " id=\"" , id , "\">" , s_p , "</h" , info.nivo_str , ">\n\n" }, context.temp_allocator)
	append(&parser^.redovi, novi_red)
}
/* -------------------------------------------------------------------------- */
ispitivanje_uslova_heading :: proc(s: string) -> NaslovInfo {
	info: NaslovInfo

	info.nivo            = 0
	info.nivo_str        = "0"
	info.da_li_je_naslov = false

	if strings.has_prefix(s, "####") {
		info.da_li_je_naslov = true
		info.nivo_str        = "4"
		info.nivo            = 4
	} else if strings.has_prefix(s, "###") {
		info.da_li_je_naslov = true
		info.nivo_str        = "3"
		info.nivo            = 3
	} else if strings.has_prefix(s, "##") {
		info.da_li_je_naslov = true
		info.nivo_str        = "2"
		info.nivo            = 2
	} else if strings.has_prefix(s, "#") {
		info.da_li_je_naslov = true
		info.nivo_str        = "1"
		info.nivo            = 1
	}

	return info
}
/* -------------------------------------------------------------------------- */
obrada_indeksa_slike :: proc(red: string, parser: ^ParserInfo) -> string {
	indeks        := -1
	slika_pattern := "#s#"

	pronadjena_slika := strings.contains(red, slika_pattern)

	if pronadjena_slika {
		str_bld: strings.Builder
		buff:    [4]u8
		strings.builder_init(&str_bld)
		strings.write_string(&str_bld, red)
		strings.builder_replace(&str_bld, slika_pattern, fmt.bprint(buff[:], parser.brojac_slika), 1)
		parser.brojac_slika += 1
		return strings.to_string(str_bld)
	} else {
		return ""
	}
}
/* -------------------------------------------------------------------------- */
upisivanje_tokena :: proc(red: string, parser: ^ParserInfo) {
	if red == "\n" do return

	if strings.has_prefix(red, "\t") {
		p        := strings.trim_space(red)
		novi_red := strings.concatenate({ "\t<li>" , p , "</li>\n" }, context.temp_allocator)
		append(&parser^.redovi, novi_red)
		return
	}

	red_p := strings.trim_space(red)
	info  := ispitivanje_uslova_heading(red_p)

	if info.da_li_je_naslov {
		obrada_naslov(red_p, info, parser)
		return
	}

	if strings.has_prefix(red_p, "**") {
		append(&parser^.redovi, "</ul>\n\n")
		return
	}

	if strings.starts_with(red_p, "*") {
		append(&parser^.redovi, "<ul>\n")
		return
	}

	if strings.has_prefix(red_p, "\t") {
		novi_red := strings.concatenate({ "\t<li>" , red_p[1 : ] , "</li>\n" }, context.temp_allocator)
		// TODO - rešiti substring !!!
		// novi_red := strings.concatenate({ "\t<li>" , red.substring(1) , "</li>\n" }, context.temp_allocator)
		append(&parser^.redovi, novi_red)
		return
	}

	if strings.has_prefix(red_p, "@@") {
		append(&parser^.izlazne_datoteke, red_p[ 2 : ]/* .substring(2) */) // TODO - rešiti substring!!!	
		return
	}

	novi_red := strings.concatenate({ "<p>\n\t" , red_p , "\n</p>\n\n" }, context.temp_allocator)
	append(&parser^.redovi, novi_red)
}
/* -------------------------------------------------------------------------- */
obrada_reda :: proc(red: string, parser: ^ParserInfo) {
	if parser.parse {
		if strings.has_prefix(red, "``") {
			parser.parse = false
			return
		}

		if rana_terminacija(red) do return

		upisivanje_tokena(red, parser)
	} else {
		if strings.has_prefix(red, "~~") {
			append(&parser^.redovi, "\n")
			parser.parse = true
			return
		}

		p := obrada_indeksa_slike(red, parser)

		if p != "" {
			append(&parser^.redovi, p)
			append(&parser^.redovi, "\n")
			return
		}

		append(&parser^.redovi, red)
		append(&parser^.redovi, "\n")
	}
}
/* -------------------------------------------------------------------------- */
ucitavanje_podataka_parser :: proc(parser: ^ParserInfo) {
	when DEBUG == false {
		putanja   := os.stdin
		check, ok := os.pipe_has_data(os.stdin)

	} else {
		// putanja := "/home/korisnik/git/idiosync_parser/cpp_parser/test/proba.imd"
		check   := true
	}

	if check {
		data_raw, ok := os.read_entire_file(putanja, context.temp_allocator)

		it := string(data_raw)
		
		for red in strings.split_lines_iterator(&it) {
			obrada_reda(red, parser)
		}

	} else {
		fmt.println("Nema stdina!")
	}
}
/* -------------------------------------------------------------------------- */
init_parser :: proc(parser: ^ParserInfo) {
	parser.parse            = true
	parser.brojac_slika     = 1
	parser.redovi           = make([dynamic]string)
	parser.izlazne_datoteke = make([dynamic]string)
}
/* -------------------------------------------------------------------------- */
idiosync_parse :: proc() {
	parser: ParserInfo
	defer delete(parser.izlazne_datoteke)
	defer delete(parser.redovi)
	defer delete(parser.mapa)

	init_parser(&parser)
	ucitavanje_podataka_parser(&parser)

	when DEBUG {
		util_ispis_redova(parser)
	} else {
		if len(parser.izlazne_datoteke) == 0 {
			fmt.println("Greška! Izlazna datoteka nije definisana.")
			return
		}

		for putanja in parser.izlazne_datoteke {
			fmt.println("----------------------")
			fmt.println("Idiosync parser (Odin)")
			fmt.println("----------------------")
			pisanje_u_datoteku(putanja, parser)
			fmt.println("Ulaz: Vim buffer")
			fmt.printf("Izlaz: datoteka %s\n", putanja)
		}
	}
}
/* -------------------------------------------------------------------------- */
main :: proc() {
	tajmer_start := time.now()

	idiosync_parse()

	tajmer_end := time.now()
	tajmer_diff := time.diff(tajmer_start, tajmer_end)
	fmt.printf("Tajmer: %.5fms\n", time.duration_milliseconds(tajmer_diff))
}
/* -------------------------------------------------------------------------- */

