fejord_parser: main.c Parser/C_code/fejord_parser.c
	g++ -o fejord_parser main.c Parser/C_code/fejord_parser.c lib/cJSON.c -I .
	