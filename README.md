# dictionary-generator

A simple dictionary generator in C++ with the aim of making it very efficient.  
For example : A dictionary composed of {"A", "B", "C"} and wordsLength set to 2 will give:
{"A", "AA", "AB", "AC", "B", "BA", "BB", "BC", "C", "CA", "CB", "CC"}.

## Configuration

Run the program in order to create `config.json`.
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

## To-do

- Speed improvements (multi-threading, improve code)
- More settings (on words length, priority system)