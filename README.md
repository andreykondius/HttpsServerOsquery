# HttpsServerOsquery

Welcome to HttpsServerOsquery!
This application is a simple TLS server for forwarding metrics from osquery to clickhouse.

## Third party sources

Clickhouse-cpp, json11.

## Necessary condition for starting the server

ClickHouse must be installed on the local host.

## Starting the server

After the successful build of this project, the "server" application will appear in the "bin" folder.
From the "bin" folder of the current project, run the command in the console:

	./server

An inscription will be displayed in the terminal:
	
	Server was run.

## Starting the osqueryd agent

Install osquery on the local machine. This process should not be run as a daemon.
Just in case, run the command in the terminal console:

	systemctl stop osqueryd

In the file folder: "bin/certificates" create an sh file by running the command:
	
	sudo nano start_osquery.sh

Copy the text to this file:

	#!/bin/bash	

	osqueryd --verbose --ephemeral --disable_database  \
    		--tls_hostname localhost:8080 \
    		--config_tls_endpoint /config \
    		--logger_tls_endpoint /logger \
    		--tls_server_certs server_ca.pem \
    		--config_plugin tls \
    		--logger_plugin tls  \
    		--enroll_tls_endpoint /enroll

Close the file start_osquery.sh , saving the changes made and setting the "executable" attribute for this file.
To do this, run the command in the console in the folder with this file:

	sudo chmod +x start_osquery.sh
	
Run this file for execution in a new terminal window:

	sudo ./start_osquery.sh
	
Standard osquery logs will be displayed in the terminal.
In the terminal with the server running, metrics transmitted by the osquery agent will be displayed every 15 seconds.
The metrics data will be written to the "base" table of the "metrics" schema of the ClickHouse database.

## Stopping the server

In the terminal with the server process running, press the keyboard shortcut: Ctrl-C

## Stopping "start_osquery.sh"

In a terminal with a file running for execution "start_osquery.sh " press the keyboard shortcut: Ctrl-C

## License

[MIT License](LICENSE)
