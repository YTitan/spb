#!/usr/bin/perl
use strict;
use warnings qw (FATAL all);
use lib '.';
use ServerData 'SERVER_SOCKET_PATH';
use IO::Socket::UNIX;

my $client = IO::Socket::UNIX->new(
	Type => SOCK_STREAM(),
	Peer => SERVER_SOCKET_PATH,
);

while(my $line = $client->getline()){
	print "$line";
}
close($client);
