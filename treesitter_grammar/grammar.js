/**
 * @file Imd grammar for tree-sitter
 * @author Nikola Vukićević
 * @license MIT
 */

/// <reference types="tree-sitter-cli/dsl" />
// @ts-check

export default grammar({
	name: "imd",

	extras: ($) => [
		$.komentar,
		// $.html_tag
	],

	rules: {
		document: ($) => repeat($._block),
		_block: ($) =>
			choice(
				$.datoteka_naziv,
				$.heading_2,
				$.heading_3,
				$.heading_4,
				$.pasus,
				$.html_blok,
				$.lista,
				"\n",
			),

		datoteka_naziv: ($) => prec(1, seq(/@@[^\n]+/, "\n")),
		pasus:          ($) => prec(0, seq(/[^\n]*/, "\n")),
		heading_2: ($) => /\n## .*/,
		heading_3: ($) => /\n### .*/,
		heading_4: ($) => /\n#### .*/,

		// HTML blok

		html_blok: ($) =>
			seq(
				$.html_marker_otvaranje,
				optional($.html_kod),
				$.html_marker_zatvaranje,
			),

		html_marker_otvaranje:  ($) => "``\n",
		html_kod:               ($) => repeat1(seq(/[^\n]*/, "\n")),
		html_marker_zatvaranje: ($) => "~~\n",

		// LISTA:
		lista: ($) =>
			seq(
				$.lista_marker_otvaranje,
				optional($.lista_elementi),
				$.lista_marker_zatvaranje,
			),

		lista_marker_otvaranje:  ($) => "\n\*\n",
		lista_elementi:          ($) => repeat1(seq(/[^\n]*/, "\n")),
		lista_marker_zatvaranje: ($) => "\*\*\n",

		// EXTRA (def):

		komentar: ($) => token.immediate(seq("!!", /.*/)),
		// html_tag: ($) => /<[a-z]+>/
	},
});

