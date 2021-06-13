clean:
	echo Cleaning .exe files
	rm *.exe

interactive:
	echo Compiling the interactive app to interactive.exe
	g++ interactive.cpp -o interactive.exe

timing:
	echo Compiling the timing tests app to timingTests.exe
	g++ timingTests.cpp -o timingTests.exe