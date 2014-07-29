all:
	g++ -I/opt/local/include -L/opt/local/lib -lboost_thread-mt -o bin/x86_64/mergePeaks_resolve src/mergePeaks_resolve.cpp

clean:
	rm bin/x86_64/mergePeaks_resolve
