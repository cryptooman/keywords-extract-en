ifeq ($(DEBUG), 1)
    CFLAGS = 
else
    CFLAGS = -O1
endif

all:
	@echo "Debug mode : $(DEBUG)"
	g++ -v $(CFLAGS) -std=c++11 keywords_extract_en.cpp -o keywords_extract_en -lboost_regex
