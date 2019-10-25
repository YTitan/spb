#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);
use lib '.';
use ServerData 'SERVER_SOCKET_PATH';
use POSIX ();
use IO::Socket::UNIX;

# get server start time
my $startTime = time();

# handle sigint to properly finish the process
my $stop = 0;
$SIG{INT} = \&sigintHandler;
sub sigintHandler {
	$stop = 1;
}

# id's
my $pid = POSIX::getpid();
my $uid = POSIX::getuid();
my $gid = POSIX::getpid();

# data to update and print
my $elapsedTime = 0;
my @loadavg = ();

# create socket
unlink(SERVER_SOCKET_PATH);
my $server = IO::Socket::UNIX->new(
	Type => SOCK_STREAM(),
	Local => SERVER_SOCKET_PATH,
	Listen => 1
) or die "Couldn't create socket";

# listen for client requests
while (my $client = $server->accept() and !$stop){
	# update data
	$elapsedTime = time() - $startTime;
	@loadavg = split(/[ ]+/, qx(uptime));
	# print data to client
	$client->print("elapsedTime: $elapsedTime\n");
	$client->print("avg load 1 minute: $loadavg[8]\n");
	$client->print("avg load 5 minutes: $loadavg[9]\n");
	$client->print("avg load 15 minutes: $loadavg[10]\n");

	$client->close();
}

$server->close();
unlink(SERVER_SOCKET_PATH);
