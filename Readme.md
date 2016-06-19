##dictionaryAlgorithm

A simple dictionary generator in C++. The goal was to make an efficient algorythm that can create a list of all possibilities from a dictionary. It also easy to compile (C++ 11) and it simply use [SimpleJSON](https://github.com/nbsdx/SimpleJSON) (header-only library).

For example :
A dictionary composed of {"A", "B", "C"} and the maximum word length set to 2 will give ->
{"A", "AA", "AB", "AC", "B", "BA", "BB", "BC", "C", "CA", "CB", "CC"}.

Configuration file :
```json
{
  "dictionary" : ["A", "B", "C"],
  "logLevel" : 2,
  "output" : {
    "directory" : "output/",
    "enable" : true,
    "wordsBeforeSplittingFiles" : 10000000
  },
  "secretWord" : "Super",
  "statsEnabled" : true,
  "wordsLength" : 2
}
```

To do :
- Speed improvements (use all cores, code)
- Allow start at different positions (range system)