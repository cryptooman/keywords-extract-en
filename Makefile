all:
	#-O3
	g++ -v -std=c++11 keywords_extract_en.cpp -o keywords_extract_en -lboost_regex