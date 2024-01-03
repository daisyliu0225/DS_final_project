# DS final project 2023 -- a search engine simulation
## Announcments
This is a project of the 2023 Fall data structure class in National Tsing Hua University.
The program is written by daisyliu0225.

## Overalls
The program simulates a search engine. The search engine requires to find a piece of data efficiently. <br />
The engine uses the structure of trie.

## Versions
| Version | Date | Description |
|---|---|---|
|0|2023/12/28| making sure that everything runs in the program |
|1|2024/01/01| putting 0.txt into a trie tree but the testcase is fixed|
|2|2024/01/03| reading folder and output the titles|
|3|2024/01/03| reading folder and read the files in the folder|
|4, 5|2024/01/03| reading folder and save the title of the data into a vector|

## Info to Self
### Command Lines
compile -- g++ -O2 -std=c++17 -o essay_search main.cpp <br />
execute -- ./essay_search ./data_folder ./query_path ./output_path

## Reference
1. [trie](https://www.geeksforgeeks.org/trie-insert-and-search/)
2. [vector usage](https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/)
3. [read folder](https://cplusplus.com/forum/beginner/10292/)
