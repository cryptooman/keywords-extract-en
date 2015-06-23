#
# Keywords extractor from text
#

Language: english

Analyzed words: 1 522 456 687

Unique words: 5 787 992

Total words in vocabulary: 41 276

Upper threshold frequency: 0.04324 % (stop-words)

Lower threshold frequency: 0.00006 % (namings, rare words, trash, etc.)

#
# Usage
#

Request

cat <text-file> | ./keywords_extract_en [--help] [--keywords-limit 20] [--html-entity-decode]

    --keywords-limit        - Limit of keywords to extract. Sorted by frequency desc. 
                              Default is 20.

    --html-entity-decode    - Unescape html entities in input text. 
                              Default is off.

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

#
# Builds
#

Binary compilled in Ubuntu 14.04 x64
