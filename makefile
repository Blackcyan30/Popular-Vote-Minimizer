build:
	rm -f app.exe
	gcc app.c MinPopVote.c -o app.exe

run:
	./app.exe

run_quiet:
	./app.exe -q

test_run1:
	./app.exe -y 1828

test_run2:
	./app.exe -y 2018 -q

valgrind:
	valgrind -s --tool=memcheck --leak-check=yes --track-origins=yes ./app.exe

run_fast:
	./app.exe -f

built_test:
	gcc test.c MinPopVote.c -o test.exe

run_test:
	./test.exe