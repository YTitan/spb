#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);

use IO::Socket::INET;
use threads;

sub ProcessClientRequest {
	my $client = $_[0];
	while(my $line = $client->getline()){
		chomp($line);
		if (opendir(my $dh, $line)){
			while (readdir $dh){
				$client->print("$line/$_\n");
			}
		}
	}
	$client->close();
}

my $port = '8085';

#create socket
my $serverSocket = new IO::Socket::INET (
	LocalHost => '0.0.0.0',
	LocalPort => $port,
	Proto => 'tcp',
	Listen => 5,
	Reuse => 1
);
die "Couldn't create socket\n" unless $serverSocket;


while(my $clientSocket = $serverSocket->accept() or 1){
	threads->create(\&ProcessClientRequest, $clientSocket);

}
$serverSocket->close();

