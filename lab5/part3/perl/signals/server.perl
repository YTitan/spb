#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);
use lib '.';
use ServerData 'SERVER_SOCKET_PATH';
use POSIX ();
use IO::Socket::UNIX;

# get server start time
my $startTime = time();

# id's
my $pid = POSIX::getpid();
my $uid = POSIX::getuid();
my $gid = POSIX::getgid();

# data to update and print
my $elapsedTime = 0;
my @loadavg = ();

$SIG{INT} = \&sigintHandler;
sub sigintHandler {
	print "pid = $pid\n";
}
$SIG{HUP} = sub { print "uid = $uid\n"; };
$SIG{TERM} = sub { print "gid = $gid\n"; };
$SIG{USR1} = sub { print "elapsedTime: $elapsedTime\n";};
$SIG{USR2} = sub { print "avg load 1 minute: $loadavg[8]\n";
		   print "avg load 5 minutes: $loadavg[9]\n";
		   print "avg load 15 minutes: $loadavg[10]\n";
	   };

# create socket
unlink(SERVER_SOCKET_PATH);
my $server = IO::Socket::UNIX->new(
	Type => SOCK_STREAM(),
	Local => SERVER_SOCKET_PATH,
	Listen => 1
) or die "Couldn't create socket";

# listen for client requests
while (1){
	my $client = $server->accept();
	if ($client){
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
}

$server->close();
unlink(SERVER_SOCKET_PATH);
