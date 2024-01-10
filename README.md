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
|6|2024/01/04|pushing the words in the paragraph into a trie with numbers and printing the numbers out when searching|
|7|2024/01/04|can transfer the number into the title|
|8|2024/01/04|making the folder path able to input by user|
|9|2024/01/05|release the memory of the trie tree at the end of the program|
|10|2024/01/05|able to read from query file and find the words in the files|
|11|2024/01/05|able to find the words in the file and output them into a file|
|12|2024/01/06|pushing the query operator into a vector|
|13|2024/01/07|making the queries into the postfix|
|13-1|2024/01/07|postfix fixed|
|13-3|2024/01/08|postfix another bug fixed|
|14|2024/01/08|the search uses a vector to save the answer. Exact search is finished.|
|15|2024/01/08|Prefix search finished.|
|16|2024/01/09|and or exclude finished|
|17-1|2024/01/10|reverse the word->suffix search starting|
|17-2|2024/01/10|suffix search finished|

## Info to Self
### Command Lines
compile -- g++ -O2 -std=c++17 -o essay_search main.cpp <br />
execute -- ./essay_search ./data_folder ./query_path ./output_path

## Reference
1. [trie](https://www.geeksforgeeks.org/trie-insert-and-search/)
2. [trie delete](https://www.geeksforgeeks.org/trie-delete/)
3. [auto complete feature using trie](https://www.geeksforgeeks.org/auto-complete-feature-using-trie/)
4. [vector usage](https://www.geeksforgeeks.org/2d-vector-in-cpp-with-user-defined-size/)
5. [vector usage-2](https://cplusplus.com/reference/vector/vector/)
6. [vector pop back](https://cplusplus.com/reference/vector/vector/pop_back/)
7. [read folder](https://cplusplus.com/forum/beginner/10292/)
8. [find algo](https://cplusplus.com/reference/algorithm/find/)
9. [fstream as function parameter](https://stackoverflow.com/questions/14503605/using-fstream-object-as-a-function-parameter)
10. [fstream](https://cplusplus.com/doc/tutorial/files/)
11. [stack](https://cplusplus.com/reference/stack/stack/)
12. [postorder](https://www.tinytsunami.info/preorder-inorder-postorder/)
13. [vector char and original char are different types](https://stackoverflow.com/questions/38513512/how-to-create-a-2-dimensional-matrix-of-char-using-vectors-in-c)
14. [wildcard search](https://www.geeksforgeeks.org/wildcard-pattern-matching/)
15. [wildcard to regex](https://www.codeproject.com/Articles/11556/Converting-Wildcards-to-Regexes)
16. [suffix](https://www.geeksforgeeks.org/auto-complete-feature-using-trie/)
17. [trie dictionary wildcard](https://gist.github.com/shehabic/5a004258793d7cf8cfa0ca15ffebb6a1)
18. [trie for all suffixes](https://www.geeksforgeeks.org/pattern-searching-using-trie-suffixes/)
19. [copy vector to string](https://stackoverflow.com/questions/7382644/how-to-efficiently-copy-a-stdvectorchar-to-a-stdstring)
20. [prefix and suffix](https://home.gamer.com.tw/artwork.php?sn=5157312)
21. [strings with given suffix](https://www.geeksforgeeks.org/find-strings-that-end-with-a-given-suffix/)
