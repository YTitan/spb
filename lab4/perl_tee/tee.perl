#!/usr/bin/perl 
use strict;
use warnings qw(FATAL all);
use Symbol;

# flags
my $append = 0;
my $ignoreInterrupts = 0;
my $ignoreFlags = 0;

# array to store names of files
my @filenames = ();

# parse flags and get fienames
for (my $i = 0; $i <= $#ARGV; ++$i){
	my $str = $ARGV[$i];
	my $firstLetter = substr ($str, 0, 1);
	my $len = length($str);
	if ($firstLetter eq "-" && $len > 1 && !$ignoreFlags) {
		# str sets a flag
		my $letter = substr($str, 1, 1);
		if ($letter eq "-") {
			# parse the flag as a whole
			# in our case only look at the third charcter
			if ( $len == 2){
				$ignoreFlags = 1;
			}
			elsif ( $len == 3 ){
				$letter = substr($str, 2, 1);
				if ( $letter eq "a"){
					$append = 1;
				}
				elsif ($letter eq "i"){
					$ignoreInterrupts = 1;
				}
				else{
					die "Invalid option: $str\n";
				}
			}
			else{
				die "Invalide option: $str\n";
			}

		}
		else {
			for (my $j = 1; $j < $len; ++$j){
				$letter = substr($str, $j, 1);
                                if ( $letter eq "a"){
                                        $append = 1;
				}
                                elsif ($letter eq "i"){
                                        $ignoreInterrupts = 1;
				}
                                else{
                                        die "Invalid option: $str\n";
				}
			}
		}
	}
	else {
		# str is a filename
		push @filenames, $str;
	}
}

my $openMode = ">";
if ( $append){
	$openMode = ">>";
}

if ($ignoreInterrupts){
	$SIG{INT} = sub {};
}

my @filehandles = ();

# open files
for (@filenames){
	my $fh = gensym;
	open $fh, $openMode.$_;
	push @filehandles, $fh;
}

# write to files
while (<STDIN>) {
	print STDOUT $_;
	my $line = $_;
	for my $handle (@filehandles){
		print $handle $line;
	}
}

# close files
for (@filehandles){
	close $_;
}

