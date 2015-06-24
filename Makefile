all:
	g++ -v -std=c++11 -O1 keywords_extract_en.cpp -o keywords_extract_en -lboost_regex
