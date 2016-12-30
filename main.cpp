#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include "md5.h"
#include "dexel2.h"

using namespace std;
using namespace vpm;

template <class T>
void bfs(T node, T target, size_t depth_limit = 0, bool verbose = false) {
    list<T> queue;
    queue.push_back(node);
    while (true) {
        T current = queue.front();
        if (current.depth() > depth_limit) {
            cout << "Depth limit reached!\n";
            break;
        }
        if (verbose) cout << "Current node: " << current.info() << endl;
        if (current.equals(target)) {
            cout << "Target found!\n";
            cout << current.info() << endl;
            break;
        } else {
            queue.pop_front();
            list<T> children = current.children();
            for (T& t : children) { queue.push_back(t); }
        }
    }
}

template <class T>
void bfs_longest(T node, T target, size_t depth_limit = 0, bool verbose = false) {
    list<T> queue;
    queue.push_back(node);
    size_t max_depth = 0;
    while (true) {
        if (queue.size() == 0) break;
        T current = queue.front();
        if (current.depth() > depth_limit) {
            cout << "Depth limit reached!\n";
            break;
        }
        if (verbose) cout << "Current node: " << current.info() << endl;
        if (current.equals(target)) {
            // Save max depth
            if (current.depth() > max_depth) max_depth = current.depth();

            queue.pop_front();
        } else {
            // Enqueue children
            queue.pop_front();
            list<T> children = current.children();
            for (T& t : children) { queue.push_back(t); }
        }
    }
    cout << "Max depth: " << max_depth << endl;
}

/*** DAY17 : WORKS ***/

string day17_input = "pvhmgsws";
string day17_test = "ihgpwlah";
string day17_test2 = "kglvqrro";
string day17_test3 = "ulqzkmiv";

bool day17_open(char c) {
    switch (c) {
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            return true;
        default:
            return false;
    }
}

class day17_state {
private:
    size_t x, y;
    string passcode;
    size_t d;
public:
    day17_state(size_t X, size_t Y, string C, size_t D) : x(X), y(Y), passcode(C), d(D) { };
    bool equals(const day17_state state) {
        return  (x == state.getX()) && (y == state.getY());
    }
    list<day17_state> children() {
        // Get md5 of current passcode
        string hash = md5(passcode);
        string keys = hash.substr(0,4);
        list<day17_state> ch;
        // Up
        if (day17_open(keys[0]) && (y-1 >= 1)) {
            ch.push_back(day17_state(x,y-1,passcode+"U",d+1));
        }
        // Down
        if (day17_open(keys[1]) && (y+1 <= 4)) {
            ch.push_back(day17_state(x,y+1,passcode+"D",d+1));
        }
        // Left
        if (day17_open(keys[2]) && (x-1 >= 1)) {
            ch.push_back(day17_state(x-1,y,passcode+"L",d+1));
        }
        // Right
        if (day17_open(keys[3]) && (x+1 <= 4)) {
            ch.push_back(day17_state(x+1,y,passcode+"R",d+1));
        }
        return ch;
    }
    size_t getX() const {
        return x;
    }
    size_t getY() const {
        return y;
    }
    string info() const {
        return "depth: "+to_string(d)+" c = "+to_string(x)+", "+to_string(y)+" "+passcode;
    }
    size_t depth() const {
        return d;
    }
};

void day17(string passcode) {
    // BFS
    day17_state start(1,1,passcode,0);
    day17_state target(4,4,"",0);
    bfs(start, target, 100);
    bfs_longest(start, target, 1000);
}

/*** DAY18 : WORKS ***/

const char trap = '^';
const char safe = '.';
const string day18_test = "..^^.";
const string day18_test2 = ".^^.^.^^^^";
const string day18_input = "^.^^^..^^...^.^..^^^^^.....^...^^^..^^^^.^^.^^^^^^^^.^^.^^^^...^^...^^^^.^.^..^^..^..^.^^.^.^.......";

char day18_tile(const char left, const char center, const char right) {
    if ((left == trap) && (right == safe)) return trap;
    if ((left == safe) && (right == trap)) return trap;
    /*if ((left == trap) && (center == trap) && (right == safe)) return trap;
    if ((left == safe) && (center == trap) && (right == trap)) return trap;
    if ((left == trap) && (center == safe) && (right == safe)) return trap;
    if ((left == safe) && (center == safe) && (right == trap)) return trap;*/

    return safe;
}

string day18_next_row(const string row) {
    string next_row = row;
    char left,center,right;
    for (size_t i=0; i<row.length(); i++) {
        // Look back to previous row
        if (i==0) {
            left = safe;
            center = row[i];
            right = row[i+1];
        } else if (i==(row.length()-1)) {
            left = row[i-1];
            center = row[i];
            right = safe;
        } else {
            left = row[i-1];
            center = row[i];
            right = row[i+1];
        }
        //cout << i << " " << left << center << right << " -> " << day18_tile(left, center, right) << endl;
        next_row[i] = day18_tile(left, center, right);
    }
    return next_row;
}

size_t day18_count_safes(string s) {
    size_t c=0;
    size_t n = s.find(safe, 0);
    while(n != string::npos) {
        c++;
        n = s.find(safe, n+1);
    }
    return c;
}

void day18(const string first_row, size_t rows) {
    list<string> field;
    size_t r = 1;
    string current_row = first_row;
    cout << current_row << endl;
    field.push_back(current_row);
    while (r < rows) {
        current_row = day18_next_row(current_row);
        field.push_back(current_row);
        cout << current_row << endl;
        r++;
    }
    size_t safes = 0;
    for(string r : field) {
        safes += day18_count_safes(r);
    }
    cout << "Total number of safe tiles: " << safes << endl;

}

void day18_count_only(const string first_row, size_t rows) {
    size_t safes = 0;
    size_t r = 1;
    string current_row = first_row;
    safes += day18_count_safes(current_row);
    while (r < rows) {
        current_row = day18_next_row(current_row);
        safes += day18_count_safes(current_row);
        r++;
    }
    cout << "Total number of safe tiles: " << safes << endl;
}

/*** DAY20 : DEV ***/


void day20(string filename) {
    ifstream input(filename);
    string line;

    vector<size_t> full_range;
    full_range.push_back(0);
    full_range.push_back(4294967295);
    Dexel2<size_t> full(full_range);

    size_t subtracts = 0;
    while (getline(input, line)) {
        stringstream ss;
        ss.str(line);
        string n;
        vector<size_t> ban_range;
        while (std::getline(ss, n, '-')) {
            ban_range.push_back(stoul(n));
        }
        Dexel2<size_t> ban(ban_range);
        full = dexelSubtract(full, ban);
        subtracts++;
    }
    cout << "Addressed " << subtracts << " blacklisted IP ranges.\n";

    vector<size_t> points = full.getPoints();
    for (size_t i=0; i<points.size(); i+=2) {
        if (points[i+1] > points[i]+1) {
            cout << "First white IP: " << points[i]+1 << endl;
            break;
        }
    }

    size_t allowed = 0;
    for (size_t i=0; i<points.size(); i+=2) {
        size_t a = points[i+1]-points[i]-1;
        allowed += a;
        if (a>0) cout << "Range " << points[i] << " - " << points[i+1] << " allows: " << a << endl;
    }
    cout << "Total allowed: " << allowed << endl;


}

int main() {


    // Day 17 solution
    //day17(day17_input);

    // Day 18 solution
    //day18(day18_input, 40);
    //day18_count_only(day18_input, 400000);

    // Day 20 solution
    day20("day20_input.txt");


    return 0;

}