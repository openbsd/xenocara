#!/usr/bin/perl -w
# $XTermId: tcapquery.pl,v 1.10 2006/07/20 00:37:00 tom Exp $
# $XFree86: xc/programs/xterm/vttests/tcapquery.pl,v 1.3 2006/03/13 01:28:02 dickey Exp $
#
# -- Thomas Dickey (2004/3/3)
# Test the tcap-query option of xterm.

use strict;

use IO::Handle;

sub get_reply($) {
	open TTY, "+</dev/tty" or die("Cannot open /dev/tty\n");
	autoflush TTY 1;
	my $old=`stty -g`;
	system "stty raw -echo min 0 time 5";

	print TTY @_;
	my $reply=<TTY>;
	close TTY;
	system "stty $old";
	return $reply;
}

sub hexified($) {
	my $value = $_[0];
	my $result = "";
	my $n;

	for ( $n = 0; $n < length($value); ++$n) {
		$result .= sprintf("%02X", ord substr($value,$n,1));
	}
	return $result;
}

sub query_tcap($$) {
	my $tcap = $_[0];
	my $tinfo = $_[1];
	my $param1 = hexified($tcap);
	my $param2 = hexified($tinfo);

	# uncomment one of the following lines
	my $reply=get_reply("\x1bP+q" . $param1 . ";" . $param2 . "\x1b\\");
	#my $reply=get_reply("\x1bP+q" . $param2 . "\x1b\\");

	return unless defined $reply;
	if ( $reply =~ /\x1bP1\+r[[:xdigit:]]+=[[:xdigit:]]*.*/ ) {
		my $value = $reply;
		my $n;

		$value =~ s/^\x1bP1\+r//;
		$value =~ s/\x1b\\//;

		my $result = "";
		for ( $n = 0; $n < length($value); ) {
			my $c = substr($value,$n,1);
			# handle semicolon and equals
			if ( $c =~ /[[:punct:]]/ ) {
				$n += 1;
				$result .= $c;
			} else {
				# handle hex-data
				my $k = hex substr($value,$n,2);
				if ( $k == 0x1b ) {
					$result .= "\\E";
				} elsif ( $k == 0x7f ) {
					$result .= "^?";
				} elsif ( $k == 32 ) {
					$result .= "\\s";
				} elsif ( $k < 32 ) {
					$result .= sprintf("^%c", $k + 64);
				} elsif ( $k > 128 ) {
					$result .= sprintf("\\%03o", $k);
				} else {
					$result .= chr($k);
				}
				$n += 2;
			}
		}

		printf "%s\n", $result;
	}
}

# See xtermcapKeycode()
query_tcap(	"#2",	"kHOM");
query_tcap(	"*7",	"kEND");
query_tcap(	"#4",	"kLFT");
query_tcap(	"%c",	"kNXT");
query_tcap(	"%e",	"kPRV");
query_tcap(	"%i",	"kRIT");

query_tcap(	"kh",	"khome");
query_tcap(	"\@7",	"kend");
query_tcap(	"kl",	"kcub1");
query_tcap(	"kd",	"kcud1");
query_tcap(	"ku",	"kcuu1");
query_tcap(	"kr",	"kcuf1");

query_tcap(	"%1",	"khlp");
query_tcap(	"#1",	"kHLP");
query_tcap(	"*6",	"kslt");
query_tcap(	"#6",	"kSLT");
query_tcap(	"\@0",	"kfnd");
query_tcap(	"*0",	"kFND");

query_tcap(	"k1",	"kf1");
query_tcap(	"k2",	"kf2");
query_tcap(	"k3",	"kf3");
query_tcap(	"k4",	"kf4");
query_tcap(	"k5",	"kf5");
query_tcap(	"k6",	"kf6");
query_tcap(	"k7",	"kf7");
query_tcap(	"k8",	"kf8");
query_tcap(	"k9",	"kf9");
query_tcap(	"k;",	"kf10");
query_tcap(	"F1",	"kf11");
query_tcap(	"F2",	"kf12");
query_tcap(	"F3",	"kf13");
query_tcap(	"F4",	"kf14");
query_tcap(	"F5",	"kf15");
query_tcap(	"F6",	"kf16");
query_tcap(	"F7",	"kf17");
query_tcap(	"F8",	"kf18");
query_tcap(	"F9",	"kf19");
query_tcap(	"FA",	"kf20");
query_tcap(	"FB",	"kf21");
query_tcap(	"FC",	"kf22");
query_tcap(	"FD",	"kf23");
query_tcap(	"FE",	"kf24");
query_tcap(	"FF",	"kf25");
query_tcap(	"FG",	"kf26");
query_tcap(	"FH",	"kf27");
query_tcap(	"FI",	"kf28");
query_tcap(	"FJ",	"kf29");
query_tcap(	"FK",	"kf30");
query_tcap(	"FL",	"kf31");
query_tcap(	"FM",	"kf32");
query_tcap(	"FN",	"kf33");
query_tcap(	"FO",	"kf34");
query_tcap(	"FP",	"kf35");
query_tcap(	"FQ",	"kf36");
query_tcap(	"FR",	"kf37");
query_tcap(	"FS",	"kf38");
query_tcap(	"FT",	"kf39");
query_tcap(	"FU",	"kf40");
query_tcap(	"FV",	"kf41");
query_tcap(	"FW",	"kf42");
query_tcap(	"FX",	"kf43");
query_tcap(	"FY",	"kf44");
query_tcap(	"FZ",	"kf45");
query_tcap(	"Fa",	"kf46");
query_tcap(	"Fb",	"kf47");
query_tcap(	"Fc",	"kf48");
query_tcap(	"Fd",	"kf49");
query_tcap(	"Fe",	"kf50");
query_tcap(	"Ff",	"kf51");
query_tcap(	"Fg",	"kf52");
query_tcap(	"Fh",	"kf53");
query_tcap(	"Fi",	"kf54");
query_tcap(	"Fj",	"kf55");
query_tcap(	"Fk",	"kf56");
query_tcap(	"Fl",	"kf57");
query_tcap(	"Fm",	"kf58");
query_tcap(	"Fn",	"kf59");
query_tcap(	"Fo",	"kf60");
query_tcap(	"Fp",	"kf61");
query_tcap(	"Fq",	"kf62");
query_tcap(	"Fr",	"kf63");

query_tcap(	"K1",	"ka1");
query_tcap(	"K4",	"kc1");

query_tcap(	"kB",	"kcbt");
query_tcap(	"kC",	"kclr");

query_tcap(	"kD",	"kdch1");
query_tcap(	"kI",	"kich1");

query_tcap(	"kN",	"knp");
query_tcap(	"kP",	"kpp");

query_tcap(	"kb",	"kbs");

query_tcap(	"Co",	"colors");
