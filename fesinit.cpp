/*
    Copyright (C) 2009-2011  EPFL (Ecole Polytechnique Fédérale de Lausanne)
    Michele Tavella <michele.tavella@epfl.ch>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "serialport.h"
#include "feslanguage.h"
#include "fesencode.h"
#include "fesdecode.h"
#include "cnbicore/CcBasic.hpp"
#include <stdio.h>
#include <stdlib.h>

void usage(void) { 
	printf("Usage: fesinit [-d DEVICE]\n\n");
	printf("Where:\n");
	printf("-d DEVICE    where DEVICE is the device name (default /dev/ttyUSB0)\n\n");

	CcCore::Exit(1);
}

void pw_off(sp_tty* tty, unsigned int channel) { 
	fl_message message;
	fl_reply reply;
	fl_activate(&message, channel, 1, 1);
	
	//fl_deactivate(&message, channel, 1.00);
	sp_writem(tty, &message);
	if(sp_waitr(tty, &reply) != FESREPLY_RECVOK) {
		CcLogErrorS("Failed to zero Ch. " << channel);
	}
}

int main(int argc, char* argv[]) {
	int opt;
	std::string device("/dev/ttyUSB0");
	
	while((opt = getopt(argc, argv, "d:h")) != -1) {
		if(opt == 'd')
			device.assign(optarg);
		else if(opt == 'h') {
			usage();
			CcCore::Exit(EXIT_SUCCESS);
		}
	}

	CcCore::OpenLogger("fesinit");
	CcCore::CatchSIGINT();
	CcCore::CatchSIGTERM();
	
	sp_tty tty;
	sp_init(&tty);
	if(sp_open(&tty, "/dev/ttyUSB0", 1) <= 0) {
		CcLogFatalS("Cannot open FES over serial device: " << device);
		CcCore::Exit(EXIT_FAILURE);;
	} 
	CcLogInfoS("FES over serial device opened: " << device);
	
	for(unsigned int ch = 1; ch <= 8; ch++) 
		pw_off(&tty, ch);

	
shutdown:
	sp_close(&tty);
	CcCore::Exit(EXIT_SUCCESS);
}
