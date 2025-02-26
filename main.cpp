#include <iostream>;
#include <fstream>;
#include <memory>;
#include <sstream>;
#include <utility>;
#include <vector>;
#include <algorithm>;

using namespace std;

struct State {
    string state;
    vector<State*> especific_functions;
    bool finalState;

    explicit State(string state, const bool finalState = false) {
        this->state = move(state);
        this->finalState = finalState;
    }
};

vector<pair<State*, State*>> equals;
/*
vector<string> getPaths(State* state, const string &alphabet, vector<string> visitedPaths = {},
    string curPath = "", vector<State*> visitedStates = {})
{
    visitedStates.push_back(state);

    if (state->finalState && find(visitedPaths.begin(), visitedPaths.end(), curPath) == visitedPaths.end()) {
        visitedPaths.push_back(curPath);
    }

    for(int i = 0; i < alphabet.length(); i++) {

        if(state->especific_functions.empty())
            break;

        State* nextState = state->especific_functions.at(i);

        if(nextState == state) {
            curPath += alphabet.at(i) + "*";
            continue;
        }

        if(nextState->state == "_")
            continue;

        if (find(visitedStates.begin(), visitedStates.end(), nextState) != visitedStates.end())
            continue;

        curPath += alphabet.at(i);

        vector<string>futurePaths = getPaths(nextState, alphabet, visitedPaths, curPath, visitedStates);
        visitedPaths.insert(visitedPaths.end(), futurePaths.begin(), futurePaths.end());
    }

    return visitedPaths;
}

bool checkPaths(const vector<string>& paths, State* state, const string& alphabet) {
    for(string path : paths) {
        State* curstate = state;
        for(int i = 0; i < path.length(); i++) {
            int index = alphabet.find(path.at(i));

            if(index == string::npos || index >= curstate->especific_functions.size())
                return false;

            if(i + 1 < path.length() && path.at(i + 1) == '*') {
                i++;
                if(curstate != curstate->especific_functions.at(index)) {
                    vector<State*> visited;
                    while(curstate != curstate->especific_functions.at(index)) {
                        if(find(visited.begin(), visited.end(), curstate) != visited.end())
                            break;

                        if(curstate->especific_functions.empty() || curstate->especific_functions.at(index)->state == "_")
                            return false;

                        visited.push_back(curstate);
                        curstate = curstate->especific_functions.at(index);
                    }
                }
            }

            curstate = curstate->especific_functions.at(index);
        }
        if(!curstate->finalState)
            return false;
    }
    return true;
}*/

vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string token;

    while (getline(ss, token, delim)) {
        result.push_back(token);
    }

    return result;
}

bool checkPair(State* state1, State* state2, const string& alphabet, vector<pair<State*, State*>> visitedStates = {}) {
    if(state1->finalState != state2->finalState)
        return false;

    if(state1->especific_functions.empty() || state2->especific_functions.empty())
        return true;

    for(pair<State*, State*> visited:visitedStates) {
        if((visited.first->state == state1->state && visited.second->state == state2->state) || (visited.first->state == state2->state && visited.second->state == state1->state))
            return true;
    }

    visitedStates.emplace_back(state1, state2);

    for(int i = 0; i < alphabet.length(); i++) {
        if(state1->especific_functions.size() <= i || state2->especific_functions.size() <= i)
            break;

        if((state1->especific_functions.at(i)->state == "_") || (state2->especific_functions.at(i)->state == "_"))
            continue;

        if(!checkPair(state1->especific_functions.at(i), state2->especific_functions.at(i), alphabet, visitedStates))
            return false;
    }

    return true;
}

int main() {
    ifstream file("text.txt", ios::in);
    stringstream buffer;

    if (!file.is_open()) {
        cout << "File could not be opened" << endl;
        return 1;
    }

    string noDFA;
    getline(file, noDFA);
    for(int i = 0; i < stoi(noDFA); i++) {
        string statesStr;
        getline(file, statesStr);
        string alphabet;
        getline(file, alphabet);
        erase_if(alphabet, ::isspace);
        vector<State*> states;
        for(int e = 0; e < stoi(statesStr) + 1; e++) {
            string curFunction;
            getline(file, curFunction);
            vector<string> splited_str = split(curFunction, ' ');
            for(int j = 0; j < splited_str.size(); j++) { //creacion de estados y todas esas mamadas
                const string& sr = splited_str.at(j);
                State* state = nullptr;
                State* parent = nullptr;

                if(e == 0) {
                    state = new State(sr, true);
                    states.push_back(state);
                    continue;
                }

                for(State* s : states) {
                    if(s->state == sr){
                        state = s;
                    }
                    if(j!=0 && s->state == splited_str.at(0)) {
                        parent = s;
                    }
                }

                if(state == nullptr) {
                    state = new State(sr);
                    states.push_back(state);
                }

                if(parent != nullptr) {
                    parent->especific_functions.push_back(state);
                }
            }
        }
        for(int i = 0; i < states.size(); i++) {
            State* s1 = states.at(i);
            for(int e = i; e < states.size(); e++) {
                State* s2 = states.at(e);

                if(s1 == s2)
                    continue;

                if(!checkPair(s1, s2, alphabet))
                    continue;

                cout << '(' << s1->state << ',' << s2->state << ") ";
            }
        }
        cout << endl << endl;
        for(State* s : states) {
            delete s;
        }
        states.clear();
    }

    file.close();

    return 0;
}
