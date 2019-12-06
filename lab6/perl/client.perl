#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);

use IO::Socket::INET;


if ($#ARGV < 2){
	die "usage: \n./client.perl hostname port [dirname...]\n";
}

my $host = $ARGV[0];
my $port = $ARGV[1];

my $socket = new IO::Socket::INET (
	PeerHost => $host,
	PeerPort => $port,
	Proto => 'tcp'
);
die "Couldn't connect to server" unless $socket;

my $i = 2;
while ($i <= $#ARGV){
	$socket->print("$ARGV[$i]\n");
	++$i;
}
shutdown($socket, 1);
while(my$line = $socket->getline()){
	print "$line";
}
$socket->close();
