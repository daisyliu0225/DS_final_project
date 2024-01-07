#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<set>
#include<iostream>

#include <dirent.h> //it is fine, ignore
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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

	//UsedPara
	std::vector<int> UsedPara;
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
void insert(struct TrieNode *root, string key, int para)
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

	std::vector<int>::iterator it;
	it = find(pCrawl->UsedPara.begin() ,pCrawl->UsedPara.end(), para);
	if(it == pCrawl->UsedPara.end()) pCrawl->UsedPara.push_back(para);
}

// Returns true if key presents in trie, else
// false
bool search(struct TrieNode *root, string key, vector<vector<string>> title_table, fstream& outputfile)
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

	if(pCrawl->isEndOfWord == true){
		int sz = pCrawl->UsedPara.size();
		for(int i=0;i<sz;i++){
			int seq = pCrawl->UsedPara[i];
			for(int j=0;j<title_table.size();j++){
				if(j == seq){
					for(int k=0;k<title_table[seq].size();k++) outputfile<<title_table[j][k]<<" ";
					outputfile<<endl;
					break;
				}
			}
		}

	}

	return (pCrawl->isEndOfWord);
}

bool isEmpty(TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return false;
    return true;
}

void del(TrieNode* root)
{
	if (isEmpty(root)) {
		delete (root);
		root = NULL;
	}else {
		for(int i=0;i<ALPHABET_SIZE;i++){
			if(root->children[i]) del(root->children[i]);
		}
	} 
}

int main(int argc, char *argv[])
{

    // INPUT :
	// 1. data directory in data folder
	// 2. number of txt files
	// 3. output route

    string data_dir = argv[1];
	string query = string(argv[2]);
	string output = string(argv[3]);

	//code for reading in the data
	string file, title_name, tmp;
	fstream fi;
	string dir, filepath;
	vector<string> tmp_string;
	vector<string> title;
	vector<vector<string>> title_table;
	struct dirent *dirp;
	DIR *dr;
	dir = data_dir;
	dr = opendir(dir.c_str()); //open data directory

	struct TrieNode *root = getNode();
	vector<string> content;
	int counter = 0;
  	while ((dirp = readdir(dr)))
    {
    	filepath = dir + "/" + dirp->d_name;
    	// Endeavor to read a single number from the file and display it
    	fi.open(filepath.c_str());
    	getline(fi, title_name);
		tmp_string = split(title_name, " ");
		title = word_parse(tmp_string);
		title_table.push_back(title);

		while(getline(fi, tmp)){
			tmp_string = split(tmp, " ");

			// PARSE CONTENT
			content = word_parse(tmp_string);

			for(auto &word : content){
				//cout<<word<<endl;
				insert(root, word, counter);
			}
		}
    	fi.close();
		counter++;
    }
  	closedir(dr);

	
	fstream q_fi;
	vector<string> q_tmp_string;
	vector<vector<string>> queries;
	vector<vector<char>> op_queries; //the operation of the queries
	vector<char> ops; //the operation of one query

	string q_tmp;
	q_fi.open(query.c_str(), ios::in);
	

	while(getline(q_fi, q_tmp)){
		int len = q_tmp.length();
		for(int i=0;i<len;i++){
			if(q_tmp[i] == '\"'){
				ops.push_back('\"');
			}else if(q_tmp[i] == '+'){
				ops.push_back('+');
			}else if(q_tmp[i] == '-'){
				ops.push_back('-');
			}else if(q_tmp[i] == '<'){
				ops.push_back('<');
			}else if(q_tmp[i] == '>'){
				ops.push_back('>');
			}else if(q_tmp[i] == '*'){
				ops.push_back('*');
			}else if(q_tmp[i] == '/'){
				ops.push_back('/');
			}
		}
		if(ops.size() != 0) op_queries.push_back(ops);
		else{
			ops.push_back('.');
			op_queries.push_back(ops);
		}
		ops.clear();
		q_tmp_string = split(q_tmp, " ");
		vector<string> content = word_parse(q_tmp_string);
		queries.push_back(content);
	}

	q_fi.close();

	/*for(int i=0;i<op_queries.size();i++){
		for(int j=0;j<op_queries[i].size();j++){
			cout<<op_queries[i][j]<<" ";
		}
		cout<<endl;
	}*/

	// Search for different keys
	fstream outputfile;
	outputfile.open(output.c_str(), ios::out);
	for(int i=0;i<queries.size();i++){
		for(int j=0;j<queries[i].size();j++){
			//cout<<queries[i][j]<<" ";
			search(root, queries[i][j].c_str(), title_table, outputfile);
		}
		//cout<<endl;
	}

	outputfile.close();

	del(root);
}




// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
