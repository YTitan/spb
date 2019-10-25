#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);
use POSIX;
use constant WC_PATH => "/usr/bin/wc";

# get filepath
if ($#ARGV == -1){
	die "usage: ./app.perl filename";
}
my $filepath = $ARGV[0];

# create a pipe
pipe my $read, my $write or die "coldn't create a pipe";

# fork process
my $cpid = fork();

if ($cpid){ # this is parent process
	# redirect stdout to child process' stdin
	POSIX::dup2(fileno($write), fileno(STDOUT)) or die "couldn't redirect parent's output";
	close($write);
	close($read);
	# get input from file
	open(my $file, "<", $filepath) or die "Couldn't open file $filepath";
	my $char;
	my $numRead = read $file, $char, 1;
	while ($numRead){
		$numRead = read $file, $char, 1;
		print $char;
		$numRead = read $file, $char, 1;
	}
	close $file;
	# if i don't close stdout child process will never finish
	close STDOUT;
	# wait for child process to finish
	waitpid $cpid, 0;
}
else { # this is child process
	POSIX::dup2(fileno($read), fileno(STDIN));
	close($write);
	exec ( WC_PATH );
}

