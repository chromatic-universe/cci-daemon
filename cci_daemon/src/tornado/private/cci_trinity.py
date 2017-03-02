# cci-trinity.py    william k. johnson  2017
#
import sys
import os

try:
    from StringIO import StringIO
except ImportError:
    from io import StringIO 

import logging

from flask import Flask , request , send_file , render_template , url_for
from flask import redirect , Response

from flask_restful import Resource, Api
from wtforms import Form, TextField, TextAreaField, validators, StringField, SubmitField
import subprocess as proc
import time
import signal
import base64
import time
import requests
import json

from tornado.concurrent import run_on_executor
from concurrent.futures import ThreadPoolExecutor

from tornado.wsgi import WSGIContainer
from tornado.httpserver import HTTPServer
from tornado.ioloop import IOLoop
from tornado import gen
from tornado.queues import Queue

from time import sleep


# --------------------------------------------------------------------------------------
def sig_handler( sig , frame ) :
                """

                :param sig:
                :param frame:
                :return:
                """
#_logger.warning('...caught signal: %s', sig )
#tornado.ioloop.IOLoop.instance().add_callback( shutdown )
                print( "...tornado signal handler..."  )
                with open( 'goofy' , 'w' ) as f :
                    f.write( 'the original corny snaps!...\n' )

                #exit( 0 )



#import kafka

#cci
#from application import app ,\
#	                    mongo_no_resource_exception , \
#						kafka_no_resource_exception , \
#	                    _logger
#from streams import tr_mongo_rest , \
#					tr_bimini , \
#					tr_trinity , \
#					tr_utils , \
#					tr_sqlite , \
#					tr_kafka_rest

max_wait_seconds_before_shutdown  = 3

http_server = None


const_per_page = 20

'''
class query_session_form( Form ) :
    session_id = StringField( 'session_id:' )
    submit = SubmitField('Submit')

policy_map = dict()

# --------------------------------------------------------------------------------------------------------
@app.errorhandler( 404 )
def page_not_found( e ) :
    return render_template( 'kc_error.html' , e=e.message ) , 404



# --------------------------------------------------------------------------------------------------------
@app.errorhandler( mongo_no_resource_exception )
def handle_mongo_exception( e ) :
	return render_template( "mongo_404.html", e=e.message )  , 404



# --------------------------------------------------------------------------------------------------------
@app.errorhandler( kafka_no_resource_exception )
def handle_kafka_exception( e ) :
    return render_template( "kafka_404.html", e=e.message )  , 404



# ------------------------------------------------------------------------------
@app.route('/trinity-vulture')
def trinity_vulture() :


			try :


				document_policy = tr_sqlite.retrieve_policy( "default" , "document" )
				stream_policy = tr_sqlite.retrieve_policy( "default" , "stream" )




				return render_template(		 "index_vulture.html" ,
											 device = '"' + tr_utils.local_mac_addr() +  '"' ,
											 document_policy=document_policy ,
											 stream_policy=stream_policy
								      )



			except Exception as e :

				return render_template( "sqlite_404.html" , e=e.message )



# ------------------------------------------------------------------------------
@app.route('/index')
@app.route( "/" )
def index() :

			try :
				"""
				from sshtunnel import SSHTunnelForwarder

				server = SSHTunnelForwarder(
											('52.38.98.223', 22),
											ssh_username="ubuntu",
											ssh_pkey='/home/wiljoh/cci-develop.pem' ,
											local_bind_address=('127.0.0.1' , 3128 ) ,
											remote_bind_address=('127.0.0.1', 8888 )
											)
				_logger.info( '..good ssh tunnel....' )
				"""
				pass

			except Exception as e :
				_logger.error( '..bad ssh tunnel....%s' % e.message )

			try :
				_logger.info( '...index...' )
				return render_template( "index.html" ,
										device = '"' + tr_utils.local_mac_addr() + '"' )
			except Exception as e :
				_logger.error( e.message )

			return render_template( "index.html" )






def sig_handler( sig , frame ) :
		"""

		:param sig:
		:param frame:
		:return:
		"""
		_logger.warning('...caught signal: %s', sig )
		IOLoop.instance().add_callback( shutdown )


def shutdown() :
		"""

		:return:
		"""
		_logger.info(' ...stopping http server...')




		http_server.stop()

		_logger.info( '....will shutdown in %s seconds ...' , max_wait_seconds_before_shutdown )
		io_loop = IOLoop.instance()

		deadline = time.time() + max_wait_seconds_before_shutdown

		def stop_loop():
			now = time.time()
			if now < deadline and (io_loop._callbacks or io_loop._timeouts ) :
				io_loop.add_timeout( now + 1 , stop_loop )
				io_loop.add_timeout( now + 1 , stop_loop )
			else:
				io_loop.stop()
				_logger.info( '...shutdown....' )

		stop_loop()




# ------------------------------------------------------------------------------
if __name__ == "__main__"  :




		is_running = False
		try :
			 pid = None
			 try :
				 with open( 'pid' , 'r' ) as pidfile :
					pid = pidfile.read().strip()
			 except :
				 pass

			 # check if process is running
			 if pid :
				 try :
					# throws exception if process doesn't exist
					os.kill( int( pid ) , 0 )
					is_running = True
				 except :
					# pid not running
					pass

			 if not is_running :
				 # tornado wsgi server , flask application
				 http_server = HTTPServer( WSGIContainer( app ) )
				 http_server.listen( 7080 )

				 # signal handlers
				 signal.si )al( signal.SIGTERM, sig_handler )
				 signal.signal( signal.SIGINT, sig_handler )

				 # write pid
              cci_trinity.py
				 with open( 'pid' , 'w' ) as pidfile :
					 pidfile.write( str( os.getpid() ) + '\n'  )

				 # init kafka consumer
				 #tr_kafka_rest.init_kafka_consumer()

				 # start server response loop
				 IOLoop.instance().start()


			 else :
				 _logger.info( '...server already running... pid %s....'  % pid )
				 sys.exit( 1 )


		except Exception as e:
			_logger.error( '...error in  trinity server...' + e.message )
			sys.exit( 1 )
'''


# ------------------------------------------------------------------------------
if __name__ == "__main__"  :

                signal.signal( signal.SIGTERM , sig_handler )
                signal.signal( signal.SIGINT , sig_handler )

                print( 'cci-daemon-dispatcher...at last!' )

                with open( 'skippy' , 'w' ) as f :
                    f.write( 'the original corny snaps!...\n' )
                while( True ) :
                    print( "...tornado speaking here...." )
                    sleep( 10 )  




