# 2011-10-23  Michele Tavella <michele.tavella@epfl.ch>

PROJECT=fesinit
FLAGS=`pkg-config --cflags --libs feslanguage cnbiloop cnbicore tobiid`

all: 
	g++ ${PROJECT}.cpp -o ${PROJECT} ${FLAGS}

clean:
	rm -f ${PROJECT} core tags session.vim
