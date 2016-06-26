##dictionaryFinder

A simple dictionary generator in C++. The aim was to make an efficient algorithm that can create a list of all possibilities from a dictionary. It's easy to compile (C++11) and it simply use one (header-only) library : [SimpleJSON](https://github.com/nbsdx/SimpleJSON).  
For example : A dictionary composed of {"A", "B", "C"} and wordsLength set to 2 will give :
{"A", "AA", "AB", "AC", "B", "BA", "BB", "BC", "C", "CA", "CB", "CC"}.

### Configuration
Run the program in order to create config.json in it's folder.
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

### Current work
- Speed improvements (use all cores, improve code)
- Allow start at different positions (range system)
- More settings (on words length, priority system)