fejord_parser: main.cpp Parser/C_code/fejord_parser.cpp
	g++ -o fejord_parser main.cpp Parser/C_code/fejord_parser.cpp lib/cJSON.c -I .
	