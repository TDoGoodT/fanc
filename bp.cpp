#include "bp.hpp"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;

bool replace(string &str, const string &from, const string &to, const BranchLabelIndex index);

CodeBuffer::CodeBuffer() : buffer(), globalDefs() {}

CodeBuffer &CodeBuffer::instance() {
    static CodeBuffer inst;//only instance
    return inst;
}

string CodeBuffer::newTemp() {
    return "%t" + to_string(tempCounter++);
}

string CodeBuffer::genLabel() {
    string label = newLabel();
    emitLabel(label + ":");
    return label;
}

int CodeBuffer::emit(const string &s, bool isGlobal) {
    string new_s = isGlobal ? s : "\t" + s;
    buffer.push_back(new_s);
    return buffer.size() - 1;
}

void CodeBuffer::bpatch(const vector<pair<int, BranchLabelIndex>> &address_list, const std::string &label) {
    for (vector<pair<int, BranchLabelIndex>>::const_iterator i = address_list.begin(); i != address_list.end(); i++) {
        int address = (*i).first;
        BranchLabelIndex labelIndex = (*i).second;
        replace(buffer[address], "@", "%" + label, labelIndex);
    }
}

void CodeBuffer::printCodeBuffer() {
    for (std::vector<string>::const_iterator it = buffer.begin(); it != buffer.end(); ++it) {
        cout << *it << endl;
    }
}

const vector<pair<int, BranchLabelIndex>> CodeBuffer::makelist(const pair<int, BranchLabelIndex> item) {
    vector<pair<int, BranchLabelIndex>> newList;
    newList.push_back(item);
    return newList;
}

vector<pair<int, BranchLabelIndex>>
CodeBuffer::merge(const vector<pair<int, BranchLabelIndex>> &l1, const vector<pair<int, BranchLabelIndex>> &l2) {
    vector<pair<int, BranchLabelIndex>> newList(l1.begin(), l1.end());
    newList.insert(newList.end(), l2.begin(), l2.end());
    return newList;
}

// ******** Methods to handle the global section ********** //
void CodeBuffer::emitGlobal(const std::string &dataLine) {
    globalDefs.push_back(dataLine);
}

void CodeBuffer::printGlobalBuffer() {
    for (vector<string>::const_iterator it = globalDefs.begin(); it != globalDefs.end(); ++it) {
        cout << *it << endl;
    }
}

int CodeBuffer::emitLabel(const string& str) {
    return emit(str, true);
}

int CodeBuffer::getLineNumber() {
    return buffer.size() - 1;
}

string CodeBuffer::newLabel() {
    return "label_" + to_string(labelCounter++);
}
// ******** Helper Methods ********** //
bool replace(string &str, const string &from, const string &to, const BranchLabelIndex index) {
    size_t pos;
    switch (index) {
        case FIRST:
            pos = str.find_first_of(from);
            break;
        case SECOND:
            pos = str.find_last_of(from);
            break;
    }
    if (pos == string::npos)
        return false;
    str.replace(pos, from.length(), to);
    return true;
}

