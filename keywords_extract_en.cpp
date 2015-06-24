/*
    Keywords extractor from text
    Language: english

    Analyzed words: 1 522 456 687
    Unique words: 5 787 992
    Total words in vocabulary: 41 276
    Upper threshold frequency: 0.04324 % (stop-words)
    Lower threshold frequency: 0.00006 % (namings, rare words, trash, etc.)

    Usage:

    Request
    cat <text-file> | ./keywords_extract_en [--help] [--keywords-limit 20] [--html-entity-decode]
        --keywords-limit        - Limit of keywords to extract. Sorted by frequency desc. Default is 20.
        --html-entity-decode    - Unescape html entities in input text. Default is off.

    Response
    {
        "content_length"                : 90826,
        "letters_count"                 : 61619,
        "words_count"                   : 13525,
        "keywords_count"                : 5670,
        "keywords_uniq_count"           : 1081,
        "keywords_frequency_ratio"      : 0.4192,
        "keywords_uniqueness_ratio"     : 0.1907,
        "keywords":
        [
            { "dictionary":227 },
            { "word":174 },
            { "file":158 },
            { "user":136 },
            { "base":94 },
            { "mode":92 },
            { "filename":67 },
            { "dic":63 },
            { "command":63 },
            { "menu":60 },
            ...
        ]
    }
*/

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <fstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>

#include "html_entities.cpp"
#include "keywords/en_all.cpp"

const int DEFAULT_KEYWORDS_LIMIT = 20;

using namespace std;

int main(int argc, char* argv[])
{
    // Get args

    string usage = "Usage: cat <text-file> | <bin> [--help] [--keywords-limit=<int>] [--html-entity-decode]";

    if(argc < 1)
    {
        cerr << "Incorrect input args. " << usage << "\n";
        exit(1);
    }

    bool print_help = false;

    int keywords_limit = 0;
    bool html_entiry_decode = false;

    for(int i = 1; i < argc; i++)
    {
        if(string(argv[i]) == "--help")
        {
            print_help = true;
        }
        else if(string(argv[i]) == "--keywords-limit")
        {
            keywords_limit = atoi(argv[++i]);
        }
        else if(string(argv[i]) == "--html-entity-decode")
        {
            html_entiry_decode = true;
        }
        else
        {
            cerr << "Unknown input arg [" << argv[i] << "]. " << usage << "\n";
            exit(1);
        }
    }

    if(keywords_limit <= 0)
        keywords_limit = DEFAULT_KEYWORDS_LIMIT;

    // Help

    if(print_help)
    {
        cout << usage << "\n";
        exit(0);
    }

    // Get input text

    string input_text;

    ios_base::sync_with_stdio(false);
    for(string line; getline(cin, line);)
    {
        input_text += line + "\n";
    }

    int content_length = input_text.length();

    // Get keywords map

    unordered_map<string, int> defined_keywords_all = get_defined_keywords_all_en();

    // Unescape html entities

    if(html_entiry_decode)
    {
        html_entity_decode(input_text);
    }

    // Remove non-letter chars

    input_text = boost::regex_replace(input_text, boost::regex {"[^a-zA-Z]"}, std::string {" "});
    input_text = boost::regex_replace(input_text, boost::regex {"[ \r\n\t]+"}, std::string {" "});

    // Split text to word tokens

    vector<string> input_text_words;
    boost::split(input_text_words, input_text, boost::is_any_of(" "));

    // Get & count keywords

    int letters_count       = 0;
    int words_count         = 0;
    int keywords_count      = 0;
    int keywords_uniq_count = 0;

    map<string,int> found_keywords;
    int w_length;

    for(string& word : input_text_words)
    {
        boost::trim(word);

        w_length = word.length();

        if(w_length)
        {
            words_count++;
            letters_count += w_length;

            if(defined_keywords_all[word] == 1)
            {
                if(!found_keywords[word])
                {
                    found_keywords[word] = 0;
                    keywords_uniq_count++;
                }

                found_keywords[word] += 1;

                keywords_count++;
            }
        }
    }

    if(!found_keywords.size())
    {
        cout << "{\"error\":\"No keywords found\"}";
        exit(1);
    }

    // Sort by frequency desc

    vector<pair<string, int> > found_keywords_sort(found_keywords.begin(), found_keywords.end());

    struct
    {
        bool operator() (pair<string, int> a, pair<string, int> b)
        {
            return a.second > b.second;   // desc
        }
    } sort_keywords_desc;

    sort(found_keywords_sort.begin(), found_keywords_sort.end(), sort_keywords_desc);

    // Prepare results

    vector<string> result_raw;

    int count = 0;
    string row;
    for(vector<pair<string, int>>::iterator word = found_keywords_sort.begin() ; word != found_keywords_sort.end(); ++word)
    {
        row = "{\"";
        row.append(word->first)
           .append("\":")
           .append(to_string(word->second))
           .append("}");

        result_raw.push_back(row);

        if(++count >= keywords_limit)
            break;
    }

    string result = boost::algorithm::join(result_raw, string(","));

    cout.precision(4);

    cout << "{\"content_length\":" << content_length <<
            ",\"letters_count\":" << letters_count <<
            ",\"words_count\":" << words_count <<
            ",\"keywords_count\":" << keywords_count <<
            ",\"keywords_uniq_count\":" << keywords_uniq_count <<
            ",\"keywords_frequency_ratio\":" << ((double) keywords_count / (double) words_count) <<
            ",\"keywords_uniqueness_ratio\":" << ((double) keywords_uniq_count / (double) keywords_count) <<
            ",\"keywords\":[" << result << "]}";

    return 0;
}