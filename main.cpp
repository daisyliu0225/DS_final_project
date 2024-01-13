#define FILE_EXTENSION ".txt"
#include<fstream>
#include<string>
#include<cstring>
#include<vector>
#include<algorithm>
#include<stack>
#include<set>
#include<iostream>
#include<utility>

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

std::vector<int> exact_search(struct TrieNode* root, string key){
	struct TrieNode *pCrawl = root;
	vector<int> answer;

	for (int i = 0; i < key.length(); i++)
	{
		int index;
		if(key[i]<='Z' && key[i]>='A') index = key[i] - 'A'; //A->0 ... Z->25
		else if(key[i]<='z' && key[i]>='a') index = key[i] - 'a'; //a -> 26 , z->51

		if (!pCrawl->children[index])
			return answer;

		pCrawl = pCrawl->children[index];
	}

	if(pCrawl->isEndOfWord == true){
		int sz = pCrawl->UsedPara.size();
		for(int i=0;i<sz;i++){
			answer.push_back(pCrawl->UsedPara[i]);
		}
		return answer;
	}

	return answer;
}

// Returns 0 if current node has a child
// If all children are NULL, return 1.
bool isLastNode(struct TrieNode* root)
{
    for (int i = 0; i < ALPHABET_SIZE; i++)
        if (root->children[i])
            return 0;
    return 1;
}

std::vector<int> suggestionString(struct TrieNode*root, string curPrefix, vector<int> curvec){
	if(root->isEndOfWord == true){
		for(int i=0;i<root->UsedPara.size();i++){
			auto it = find(curvec.begin(), curvec.end(), root->UsedPara[i]);
			if(it == curvec.end()) curvec.push_back(root->UsedPara[i]);
		}
	}

	for(int i=0;i<ALPHABET_SIZE;i++){
		if(root->children[i]){
			char child = 'a' + i;
			curvec = suggestionString(root->children[i], curPrefix+ child, curvec);
		}
	}
	return curvec;
}

std::vector<int> prefix_search(struct TrieNode* root, string key){
	struct TrieNode *pCrawl = root;
	vector<int> answer;

	for (int i = 0; i < key.length(); i++)
	{
		int index;
		if(key[i]<='Z' && key[i]>='A') index = key[i] - 'A'; //A->0 ... Z->25
		else if(key[i]<='z' && key[i]>='a') index = key[i] - 'a'; //a -> 26 , z->51

		if (!pCrawl->children[index])
			return answer;

		pCrawl = pCrawl->children[index];
	}

	if(isLastNode(pCrawl)){
		for(int i=0;i<pCrawl->UsedPara.size();i++) answer.push_back(pCrawl->UsedPara[i]);
	}

	vector<int> v = suggestionString(pCrawl, key, answer);
	return v;
}


//operate function
std::vector<int> operate(struct TrieNode *root, string keyword, string operate_ele){
	std::vector<int> result;
	if(operate_ele[0] == '\"'){
		result = exact_search(root, keyword);
	}else if(operate_ele[0] == '*'){
		vector<char> rev_word(keyword.begin(), keyword.end());
		reverse(rev_word.begin(), rev_word.end());
		string rev_str(rev_word.begin(), rev_word.end());
		result = prefix_search(root, rev_str); //it uses prefix search but the keyword is reversed
	}else if(operate_ele[0] == '.'){
		result = prefix_search(root, keyword);
	}

	for(int i=0;i<keyword.length();i++){
		cout<<keyword[i];
	}
	cout<<endl;
	for(int i=0;i<result.size();i++){
		cout<<"result = "<<result[i]<<" ";
	}
	cout<<endl;
	return result;
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
	vector<pair<string, int>> title_table;

	struct dirent *dirp;
	DIR *dr;
	dir = data_dir;
	dr = opendir(dir.c_str()); //open data directory

	struct TrieNode *root = getNode();
	struct TrieNode *rev_root  = getNode();
	vector<string> content;
	vector<string> rev_content;
	vector<int> file_title;
  	while ((dirp = readdir(dr)))
    {
    	filepath = dir + "/" + dirp->d_name;
    	// Endeavor to read a single number from the file and display it
    	fi.open(filepath.c_str());
		string dn = dirp->d_name;
		cout<<"dn "<<dn<<endl;
		for(int i=0;i<dn.length();i++){
			if(dn[i] == '.') break;
			else file_title.push_back(dn[i]- '0');
		}
		int counter = 0;
		for(auto d : file_title){
			counter = counter *10 + d;
		}
		//cout<<"counter "<<counter<<endl;
		file_title.clear();
    	getline(fi, title_name);
		title_table.push_back(make_pair(title_name, counter));
		tmp_string = split(title_name, " ");
		title = word_parse(tmp_string);
		for(auto &title_word : title){
			insert(root, title_word, counter);
			vector<char> rev_word_title(title_word.begin(), title_word.end());
			reverse(rev_word_title.begin(), rev_word_title.end());
			string rev_title_str(rev_word_title.begin(), rev_word_title.end());
			insert(rev_root, rev_title_str, counter);
		}

		while(getline(fi, tmp)){
			tmp_string = split(tmp, " ");
			// PARSE CONTENT
			content = word_parse(tmp_string);

			for(auto &word : content){
				//cout<<word<<endl;
				insert(root, word, counter);
				vector<char> rev_word(word.begin(), word.end());
				reverse(rev_word.begin(), rev_word.end());
				string rev_str(rev_word.begin(), rev_word.end());
				insert(rev_root, rev_str, counter);
			}

		}
    	fi.close();
    }
  	closedir(dr);

	
	fstream q_fi;
	vector<string> q_tmp_string;
	vector<vector<string>> queries;
	vector<char> ops; //the operation of queries
	vector<char> c_stack;
	stack<char> elements;

	string q_tmp;
	string q_string;
	q_fi.open(query.c_str(), ios::in);

	
	
	while(getline(q_fi, q_tmp)){
		int popcount = 0;
		bool paran = 0;
		bool wild = 0;
		bool star = 0;
		int len = q_tmp.length();
		//cout<<"len = "<<len<<endl;
		for(int i=0;i<len;i++){
			if(q_tmp[i] == '\"'){
				if(paran == 0) {
					elements.push('\"');
					paran = 1;
				}else{
					paran = 0;
					elements.pop();
					q_string.assign(c_stack.begin(), c_stack.end());
					q_tmp_string.push_back(q_string);
					c_stack.clear();
					ops.push_back('\"');
					q_string.assign(ops.begin(), ops.end());
					q_tmp_string.push_back(q_string);
					ops.clear();
					popcount++;
					if(popcount == 2){
						if(!elements.empty()){
							char c = elements.top();
							ops.push_back(c);
							elements.pop();
							q_string.assign(ops.begin(), ops.end());
							q_tmp_string.push_back(q_string);
							ops.clear();
							popcount = 1;
						}
					}
				}
			}else if(q_tmp[i] == '<'){
				wild = 1;
				elements.push('<');
			}else if(q_tmp[i] == '>'){
				wild = 0;
				elements.pop();
				q_string.assign(c_stack.begin(), c_stack.end());
				q_tmp_string.push_back(q_string);
				c_stack.clear();
				ops.push_back('<');
				q_string.assign(ops.begin(), ops.end());
				q_tmp_string.push_back(q_string);
				ops.clear();
				popcount++;
				if(popcount == 2){
					if(!elements.empty()){
						char c = elements.top();
						ops.push_back(c);
						elements.pop();
						q_string.assign(ops.begin(), ops.end());
						q_tmp_string.push_back(q_string);
						ops.clear();
						popcount = 1;
					}
				}
			}else if(q_tmp[i] == '*'){
				if(wild == 1) c_stack.push_back(q_tmp[i]);
				else{
					if(star == 0){
						star = 1;
						elements.push('*');
					}else if(star == 1){
						star = 0;
						elements.pop();
						q_string.assign(c_stack.begin(), c_stack.end());
						q_tmp_string.push_back(q_string);
						c_stack.clear();
						ops.push_back('*');
						q_string.assign(ops.begin(), ops.end());
						q_tmp_string.push_back(q_string);
						ops.clear();
						popcount++;
						if(popcount == 2){
							if(!elements.empty()){
								char c = elements.top();
								ops.push_back(c);
								elements.pop();
								q_string.assign(ops.begin(), ops.end());
								q_tmp_string.push_back(q_string);
								ops.clear();
								popcount = 1;
							}
						}
					}
				}	
			}else if(q_tmp[i] <= 'z' && q_tmp[i] >= 'a'){
				c_stack.push_back(q_tmp[i]);
			}else if(q_tmp[i] <= 'Z' && q_tmp[i] >='A'){
				c_stack.push_back(q_tmp[i]);
			}else if(q_tmp[i] == ' '){
				if(c_stack.size()!=0){
					q_string.assign(c_stack.begin(), c_stack.end());
					q_tmp_string.push_back(q_string);
					c_stack.clear();
					ops.push_back('.');
					q_string.assign(ops.begin(), ops.end());
					q_tmp_string.push_back(q_string);
					ops.clear();
					popcount++;
					if(popcount == 2){
						if(!elements.empty()){
							char c = elements.top();
							ops.push_back(c);
							elements.pop();
							q_string.assign(ops.begin(), ops.end());
							q_tmp_string.push_back(q_string);
							ops.clear();
							popcount = 1;
						}
					}
				}
			}else if(q_tmp[i] == '+' || q_tmp[i] == '-' || q_tmp[i] == '/'){
				if(c_stack.size()!=0){
					q_string.assign(c_stack.begin(), c_stack.end());
					q_tmp_string.push_back(q_string);
					c_stack.clear();
				}
				elements.push(q_tmp[i]);
			}
		}
		if(c_stack.size()!=0){
			q_string.assign(c_stack.begin(), c_stack.end());
			q_tmp_string.push_back(q_string);
			c_stack.clear();
			ops.push_back('.');
			q_string.assign(ops.begin(), ops.end());
			q_tmp_string.push_back(q_string);
			ops.clear();
		}
		while(!elements.empty()){
			char c = elements.top();
			ops.push_back(c);
			elements.pop();
			q_string.assign(ops.begin(), ops.end());
			q_tmp_string.push_back(q_string);
			ops.clear();
		}
		queries.push_back(q_tmp_string);
		q_tmp_string.clear();
	}

	q_fi.close();

	for(int i=0;i<queries.size();i++){
		for(int j=0;j<queries[i].size();j++){
			cout<<queries[i][j]<<" ";
		}
		cout<<endl;
	}
	// Search for different keys
	vector<vector<int>> opstack;
	vector<int> answer;
	vector<int> v1, v2;
	fstream outputfile;
	int count = 0;
	outputfile.open(output.c_str(), ios::out);
	for(int i=0;i<queries.size();i++){
		//cout<<"i = "<<i<<endl;
		for(int j=0;j<queries[i].size();j++){
			//cout<<queries[i][j]<<" "<<queries[i][j+1]<<endl;
			if(queries[i][j][0] >= 'a' && queries[i][j][0] <= 'z'){
				if(queries[i][j+1][0] == '*') v1 = operate(rev_root, queries[i][j].c_str(), queries[i][j+1].c_str());
				else v1 = operate(root, queries[i][j].c_str(), queries[i][j+1].c_str());
				opstack.push_back(v1);
				j++;
				count++;
			}else if(queries[i][j][0] >= 'A' && queries[i][j][0] <= 'Z'){
				if(queries[i][j+1][0] == '*') v2 = operate(rev_root, queries[i][j].c_str(), queries[i][j+1].c_str());
				else v2 = operate(root, queries[i][j].c_str(), queries[i][j+1].c_str());
				opstack.push_back(v2);
				j++;
				count++;
			}else if(queries[i][j][0] == '+'){
				for(int i=0;i<opstack.size();i++){
					cout<<opstack[i].size()<<" ";
					for(int j=0;j<opstack[i].size();j++) cout<<"ans = "<<opstack[i][j]<<" ";
					cout<<endl;
				}
				for(int i=0;i<opstack[0].size();i++){
					auto it = find(opstack[1].begin(), opstack[1].end(), opstack[0][i]);
					if(it == opstack[1].end()) continue;
					else answer.push_back(opstack[0][i]);
				}
				sort(answer.begin(), answer.end());
				//for(int i=0;i<answer.size();i++) cout<<answer[i]<<endl;
				while(!opstack.empty()) opstack.pop_back();
				opstack.push_back(answer);
				cout<<"size = "<<opstack.size()<<endl;
				for(int i=0;i<opstack.size();i++){
					cout<<"size2 = "<<opstack[i].size()<<endl;
					for(int j=0;j<opstack[i].size();j++) cout<<opstack[i][j]<<" ";
					cout<<endl;
				}
				answer.clear();
			}else if(queries[i][j][0] == '/'){
				for(int i=0;i<opstack[0].size();i++){
					answer.push_back(opstack[0][i]);
				}
				for(int i=0;i<opstack[1].size();i++){
					auto it = find(answer.begin(), answer.end(), opstack[1][i]);
					if(it == answer.end()) answer.push_back(opstack[1][i]);
				}
				sort(answer.begin(), answer.begin());
				while(!opstack.empty()) opstack.pop_back();
				opstack.push_back(answer);
				answer.clear();
			}else if(queries[i][j][0] == '-'){
				cout<<"- ans"<<endl;
				for(int i=0;i<opstack[0].size();i++){
					auto it = find(opstack[1].begin(), opstack[1].end(), opstack[0][i]);
					if(it == opstack[1].end()) answer.push_back(opstack[0][i]);
				}
				for(int i=0;i<answer.size();i++) cout<<answer[i]<<" ";
				cout<<endl;
				sort(answer.begin(), answer.end());
				while(!opstack.empty()) opstack.pop_back();
				opstack.push_back(answer);
				for(int i=0;i<opstack[0].size();i++){
					cout<<opstack[0][i]<<" ";
				}
				cout<<endl;
				answer.clear();
			}
			cout<<"round"<<endl;
		}
		sort(opstack[0].begin(), opstack[0].end());
		cout<<"opstack"<<endl;
		for(int i=0;i<opstack[0].size();i++) cout<<opstack[0][i]<<" ";
		cout<<endl;
		if(opstack[0].size() == 0) outputfile<<"Not Found!"<<endl;
		else{
			for(int i=0;i<opstack[0].size();i++){
				int num = opstack[0][i];
				int num2 = 0;
				int sz = title_table.size();
				for(int j=0;j<sz;j++){
					if(title_table[j].second == num){
						num2 = j;
						break;
					}
					else if(title_table[sz-j].second == num){
						num2 = sz-j;
						break;
					}
				}
				for(int j=0;j<title_table[num2].first.size();j++) outputfile<<title_table[num2].first[j];
				outputfile<<endl;
			}
		}
		while(!opstack.empty()) opstack.pop_back();
	}


	outputfile.close();

	del(root);
	del(rev_root);
}




// 1. UPPERCASE CHARACTER & LOWERCASE CHARACTER ARE SEEN AS SAME.
// 2. FOR SPECIAL CHARACTER OR DIGITS IN CONTENT OR TITLE -> PLEASE JUST IGNORE, YOU WONT NEED TO CONSIDER IT.
//    EG : "AB?AB" WILL BE SEEN AS "ABAB", "I AM SO SURPRISE!" WILL BE SEEN AS WORD ARRAY AS ["I", "AM", "SO", "SURPRISE"].
// 3. THE OPERATOR IN "QUERY.TXT" IS LEFT ASSOCIATIVE
//    EG : A + B / C == (A + B) / C

//

//////////////////////////////////////////////////////////
