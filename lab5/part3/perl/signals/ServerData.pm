package ServerData;
use strict;
use warnings;
use base 'Exporter';

use constant SERVER_SOCKET_PATH => "/tmp/my_server_socket.socket";


our @EXPORT_OK = ( 'SERVER_SOCKET_PATH' );

#our @EXPORT = ( 'SERVER_SOCKET_PATH' );

1;
