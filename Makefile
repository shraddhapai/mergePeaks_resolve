all:
	g++ -I/opt/local/include -L/usr/local/include -lpthread -lboost_system -lboost_thread-mt -o bin/x86_64/mergePeaks_resolve src/mergePeaks_resolve.cpp

clean:
	rm bin/x86_64/mergePeaks_resolve
