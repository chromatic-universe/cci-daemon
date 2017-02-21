import os
from time import sleep
import logging

if __name__ == '__main__':
    # logger
    _logger = logging.getLogger( "cci-trinity-server" )
    _logger.setLevel( logging.DEBUG )
    fh = logging.FileHandler(  'cci-trinity-server.log' + '-debug.log', mode = 'a' )
    fh.setLevel( logging.DEBUG )
    formatter = logging.Formatter( log_format )
    fh.setFormatter( formatter )
    _logger.addHandler( fh )

	
	with open( 'cci' , 'w' ) as f :
		f.write( 'the original corny snaps!\n' )
	
	while True :
		sleep( 2 )
		
