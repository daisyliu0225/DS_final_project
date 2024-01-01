#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<iostream>

using namespace std;
const int ALPHABET_SIZE = 26;

// Utility Func

// string parser : output vector of strings (words) after parsing
vector<string> word_parse(vector<string> tmp_string){
	vector<string> parse_string;
	for(auto& word : tmp_string){
		string new_str;
    	for(auto &ch : word){
			if(isalpha(ch))
				new_str.push_back(ch);
		}
		parse_string.emplace_back(new_str);
	}
	return parse_string;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if("" == str) return res;

	char * strs = new char[str.length() + 1] ; 
	strcpy(strs, str.c_str());

	char * d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char *p = strtok(strs, d);
	while(p) {
		string s = p; 
		res.push_back(s); 
		p = strtok(NULL, d);
	}

	return res;
}

//trie functions
struct TrieNode
{
	struct TrieNode *children[ALPHABET_SIZE];

	// isEndOfWord is true if the node represents
	// end of a word
	bool isEndOfWord;
};

// Returns new trie node (initialized to NULLs)
struct TrieNode *getNode(void)
{
	struct TrieNode *pNode = new TrieNode;

	pNode->isEndOfWord = false;

	for (int i = 0; i < ALPHABET_SIZE; i++)
		pNode->children[i] = NULL;

	return pNode;
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just
// marks leaf node
void insert(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index;
		if(key[i]<='Z' && key[i]>='A') index = key[i] - 'A'; //A->0 ... Z->25
		else if(key[i]<='z' && key[i]>='a') index = key[i] - 'a'; //a -> 0, z->25
		
		if (!pCrawl->children[index])
			pCrawl->children[index] = getNode();

		pCrawl = pCrawl->children[index];
	}

	// mark last node as leaf
	pCrawl->isEndOfWord = true;
}

// Returns true if key presents in trie, else
// false
bool search(struct TrieNode *root, string key)
{
	struct TrieNode *pCrawl = root;

	for (int i = 0; i < key.length(); i++)
	{
		int index;
		if(key[i]<='Z' && key[i]>='A') index = key[i] - 'A'; //A->0 ... Z->25
		else if(key[i]<='z' && key[i]>='a') index = key[i] - 'a'; //a -> 26 , z->51

		if (!pCrawl->children[index])
			return false;

		pCrawl = pCrawl->children[index];
	}

	return (pCrawl->isEndOfWord);
}


int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1] + string("/");
	string query = string(argv[2]);
	string output = string(argv[3]);

	// Read File & Parser Example


	string file, title_name, tmp;
	fstream fi;
	vector<string> tmp_string;

	// from data_dir get file ....
	// eg : use 0.txt in data directory
	fi.open("data/0.txt", ios::in);

    // GET TITLENAME
	getline(fi, title_name);

    // GET TITLENAME WORD ARRAY
    tmp_string = split(title_name, " ");

	vector<string> title = word_parse(tmp_string);

	// for(auto &word : title){
	// 	cout << word << endl;
	// }

    // GET CONTENT LINE BY LINE
	vector<string> content;
	struct TrieNode *root = getNode();

	while(getline(fi, tmp)){

        // GET CONTENT WORD VECTOR
		tmp_string = split(tmp, " ");

		// PARSE CONTENT
		content = word_parse(tmp_string);

		for(auto &word : content){
			insert(root, word);
			cout << word << endl;
		}

	}

    // CLOSE FILE
	fi.close();
	char out[][32] = {"Not present in trie", "Present in trie"};

	// Search for different keys
	cout<<"the"<<" --- "<<out[search(root, "the")]<<endl;
	cout<<"these"<<" --- "<<out[search(root, "these")]<<endl;
	cout<<"their"<<" --- "<<out[search(root, "their")]<<endl;
	cout<<"thaw"<<" --- "<<out[search(root, "thaw")]<<endl;
}




// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
