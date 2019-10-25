#!/usr/bin/perl
use strict;
use warnings qw(FATAL all);

my $str = qx(uptime);
my @arr = split(/[ ]+/, $str);
print "$arr[7], $arr[8], $arr[9]";
