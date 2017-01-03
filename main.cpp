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

/*** DAY20 : WORKS ***/

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

/*** DAY21 : WORKS ***/

enum class day21_type {
    SwapPosition,
    SwapLetter,
    Rotate,
    RotateSpecial,
    Reverse,
    Move
};

class day21_scramble {
private:
    day21_type type;
    int x, y;
public:
    day21_scramble(day21_type t, int X) {
        type = t;
        x = y = X;
    }
    day21_scramble(day21_type t, int X, int Y) {
        type = t;
        x = X;
        y = Y;
    }
    void apply(string& input) const {
        char t = 'a';
        size_t ny = 0;
        size_t nx = 0;
        int f = 0;
        switch (type) {
            case day21_type::SwapPosition:
                t = input[x];
                input[x] = input[y];
                input[y] = t;
                break;
            case day21_type::SwapLetter:
                nx = input.find((char)x);
                ny = input.find((char)y);
                if((nx != string::npos) && (ny != string::npos)) {
                    char t = input[nx];
                    input[nx] = input[ny];
                    input[ny] = t;
                }
                break;
            case day21_type::Rotate:
                if (x < 0) {
                    // Rotate right
                    f = (-x) - ((-x)/input.length())*input.length();
                    rotate(input.rbegin(), input.rbegin() + f, input.rend());
                } else {
                    // Rotate left
                    f = x - (x/input.length())*input.length();
                    rotate(input.begin(), input.begin() + f, input.end());
                }
                break;
            case day21_type::RotateSpecial:
                // Get amount
                nx = input.find((char)x);
                if (nx>=4) nx++;
                nx++;
                // Rotate right
                f = nx - (nx/input.length())*input.length();
                rotate(input.rbegin(), input.rbegin() + f, input.rend());
                break;
            case day21_type::Reverse:
                reverse(input.begin()+x,input.begin()+y+1);
                break;
            case day21_type::Move:
                t = input[x];
                input.erase(input.begin()+x);
                input.insert(input.begin()+y,t);
                break;
        }
    }
    void unapply(string& input) const {
        char t = 'a';
        size_t ny = 0;
        size_t nx = 0;
        int f = 0;
        string str = "";
        string original = input;
        switch (type) {
            case day21_type::SwapPosition:
                // Swap = Same operation
                t = input[x];
                input[x] = input[y];
                input[y] = t;
                break;
            case day21_type::SwapLetter:
                // Swap = Same operation
                nx = input.find((char)x);
                ny = input.find((char)y);
                if((nx != string::npos) && (ny != string::npos)) {
                    char t = input[nx];
                    input[nx] = input[ny];
                    input[ny] = t;
                }
                break;
            case day21_type::Rotate:
                if (x < 0) {
                    // Rotate left
                    f = (-x) - ((-x)/input.length())*input.length();
                    rotate(input.begin(), input.begin() + f, input.end());
                } else {
                    // Rotate right
                    f = x - (x/input.length())*input.length();
                    rotate(input.rbegin(), input.rbegin() + f, input.rend());
                }
                break;
            case day21_type::RotateSpecial:
                // f = 0 initially
                while(true) {
                    // Try inverse operation, rotate with f
                    rotate(input.begin(), input.begin() + f, input.end());
                    str = input;
                    apply(str);
                    if (str == original) {
                        break;
                    }
                    f++;
                }
                break;
            case day21_type::Reverse:
                // Reverse = same
                reverse(input.begin()+x,input.begin()+y+1);
                break;
            case day21_type::Move:
                // Unmove
                t = input[y];
                input.erase(input.begin()+y);
                input.insert(input.begin()+x,t);
                break;
        }
    }
};

class day21_scrambler {
private:
    vector<day21_scramble> i;
public:
    day21_scrambler() {};
    void apply(string& input) {
        for (day21_scramble& s : i) {
            s.apply(input);
        }
    }
    void unapply(string& input) {
        for (int p = i.size()-1; p>=0; p--) {
            string before = input;
            i[p].unapply(input);
            string verify = input;
            i[p].apply(verify);
            if (verify == before) {
                //cout << "Unapply instruction " << p << " verified!\n";
            } else {
                cout << "Unapply instruction " << p << " failed!\n";
            }
        }
    }
    void add(day21_scramble s) {
        i.push_back(s);
    }
};

void day21_test() {
    day21_scrambler s;
    s.add(day21_scramble(day21_type::SwapPosition,4,0));
    s.add(day21_scramble(day21_type::SwapLetter,'d','b'));
    s.add(day21_scramble(day21_type::Reverse,0,4));
    s.add(day21_scramble(day21_type::Rotate,1));
    s.add(day21_scramble(day21_type::Move,1,4));
    s.add(day21_scramble(day21_type::Move,3,0));
    s.add(day21_scramble(day21_type::RotateSpecial,'b'));
    s.add(day21_scramble(day21_type::RotateSpecial,'d'));
    string in = "abcde";
    s.apply(in);
}

void day21(string filename, string password, string scrambled) {
    ifstream input(filename);
    string line;
    day21_scrambler scrambler;
    string i = password;
    cout << "Scrambling: " << i << endl;
    while (getline(input, line)) {
        stringstream ss;
        ss.str(line);
        vector<string> words;
        string word;
        while (getline(ss, word, ' ')) {
            words.push_back(word);
        }
        if (words.size() >= 7 && words[0] == "rotate" && words[1] == "based") {
            scrambler.add(day21_scramble(day21_type::RotateSpecial, words[6][0]));
        } else if (words.size() >= 6 && words[0] == "swap" && words[1] == "position") {
            scrambler.add(day21_scramble(day21_type::SwapPosition, stoi(words[2]), stoi(words[5])));
        } else if  (words.size() >= 6 && words[0] == "swap" && words[1] == "letter") {
            scrambler.add(day21_scramble(day21_type::SwapLetter, words[2][0], words[5][0]));
        } else if  (words.size() >= 6 && words[0] == "move") {
            scrambler.add(day21_scramble(day21_type::Move, stoi(words[2]), stoi(words[5])));
        } else if  (words.size() >= 5 && words[0] == "reverse") {
            scrambler.add(day21_scramble(day21_type::Reverse, stoi(words[2]), stoi(words[4])));
        } else if  (words.size() >= 4 && words[0] == "rotate" && words[1] == "left") {
            scrambler.add(day21_scramble(day21_type::Rotate, stoi(words[2])));
        } else if  (words.size() >= 4 && words[0] == "rotate" && words[1] == "right") {
            scrambler.add(day21_scramble(day21_type::Rotate, -stoi(words[2])));
        }
    }
    scrambler.apply(i);
    cout << "Result: " << i << endl; // Result: fdhbcgea
    cout << "Unscrambling: " << i << endl;
    scrambler.unapply(i);
    cout << "Result: " << i << endl;

    cout << "Unscrambling: " << scrambled << endl;
    scrambler.unapply(scrambled);
    cout << "Result: " << scrambled << endl;
}

/*** DAY22 : WORKS (Part 2 needs counting) ***/

class day22_node {
private:
    size_t x, y, s, u, a;
public:
    day22_node(size_t x, size_t y, size_t size, size_t used, size_t avail) : x(x), y(y), s(size),
                                                                             u(used), a(avail) {};
    void print_info() {
        cout << "Node at (" << x << "," << y << ") free: " << a << ", used: " << u << " (total: " << s << ")\n";
    }
    size_t size() const {
        return s;
    }
    size_t used() const {
        return u;
    }
    size_t avail() const {
        return a;
    }
    size_t getX() const {
        return x;
    }
    size_t getY() const {
        return y;
    }
};

void day22(string filename) {
    ifstream input(filename);
    string line;
    // Skip first line
    getline(input, line);
    vector<day22_node> nodes;
    while (getline(input, line)) {
        stringstream ss;
        ss.str(line);
        vector<string> words;
        string word;
        while (getline(ss, word, ' ')) {
            words.push_back(word);
        }
        // Parse node data (x0 y0 94T 73T 21T 77%)
        // Erase extra chars
        words[0].erase(words[0].begin());
        words[1].erase(words[1].begin());
        words[2].erase(words[2].length()-1);
        words[3].erase(words[3].length()-1);
        words[4].erase(words[4].length()-1);
        nodes.push_back( day22_node(stoi(words[0]),
                                    stoi(words[1]),
                                    stoi(words[2]),
                                    stoi(words[3]),
                                    stoi(words[4])) );
    }
    /*for(day22_node &n : nodes) {
        n.print_info();
    }*/
    size_t pairs = 0;
    for (size_t i=0; i<nodes.size(); i++) {
        for (size_t j=i+1; j<nodes.size(); j++) {
            if(nodes[i].used() <= nodes[j].avail() && nodes[i].used() > 0) pairs++;
            if(nodes[j].used() <= nodes[i].avail() && nodes[j].used() > 0) pairs++;
        }
    }
    cout << "Viable pairs: " << pairs << endl;
    size_t n = 31;
    for (size_t i=0; i<n; i++) {
        for (size_t j=0; j<n; j++) {
            size_t z = j*n+i;//i*n+j;
            size_t u = nodes[z].used();
            size_t s = nodes[z].size();
            size_t x = nodes[z].getX();
            size_t y = nodes[z].getY();
            //cout << "("<<x<<","<<y<<") ";
            if (x==0 && y==0) { cout << "E"; }
            else if ((x==n-1) && y==0) { cout << "G"; }
            else if (s > 99) { cout << "#"; }
            else if (u == 0) { cout << "_"; }
            else if (double(u)/double(s) > 0.6) { cout << "."; }
            else { cout << "?";}
            cout << " ";
        }
        cout << endl;
    }
}

/**
 * After getting the map of the grid:
    D.............................G
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    .....##########################
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ...............................
    ............._.................
    ...............................
    ...............................
    ...............................

    9+26+26 to move before the data
    D............................_G
    ...............................

    +1 to move it left by 1 (sub total 62)
    D............................G_
    ...............................

    Now additional 5 moves it left again
    So from this move:
    D............................G_
    ...............................
    29*5 moves it to destination D
    Total 62+29*5=207
 */

/*** DAY23 : WORKS ***/

enum class day23_t {
    cpy,
    dec,
    inc,
    jnz,
    tgl
};

enum class day23_argt {
    reg,
    val
};

class day23_i {
public:
    day23_t t;
    int a1, a2;
    day23_argt at1, at2;
    day23_i(day23_t t, day23_argt at1, int a1, day23_argt at2, int a2) : t(t), a1(a1), a2(a2), at1(at1), at2(at2) {};
    day23_i(day23_t t, day23_argt at1, int a1) : t(t), a1(a1), at1(at1) {};
};

day23_i day23_parse(vector<string>& words) {
    // Parse assembunny instruction
    if (words[0] == "cpy") {
        if (words[1] == "a" || words[1] == "b" || words[1] == "c" || words[1] == "d") {
            return day23_i(day23_t::cpy, day23_argt::reg, words[1][0], day23_argt::reg, words[2][0]);
        } else {
            return day23_i(day23_t::cpy, day23_argt::val, stoi(words[1]), day23_argt::reg, words[2][0]);
        }
    } else if (words[0] == "inc") {
        return day23_i(day23_t::inc, day23_argt::reg, words[1][0]);
    } else if (words[0] == "dec") {
        return day23_i(day23_t::dec, day23_argt::reg, words[1][0]);
    } else if (words[0] == "jnz") {
        if (words[1] == "a" || words[1] == "b" || words[1] == "c" || words[1] == "d") {
            if (words[2] == "a" || words[2] == "b" || words[2] == "c" || words[2] == "d") {
                return day23_i(day23_t::jnz, day23_argt::reg, words[1][0], day23_argt::reg, words[2][0]);
            } else {
                return day23_i(day23_t::jnz, day23_argt::reg, words[1][0], day23_argt::val, stoi(words[2]));
            }
        } else {
            if (words[2] == "a" || words[2] == "b" || words[2] == "c" || words[2] == "d") {
                return day23_i(day23_t::jnz, day23_argt::val, stoi(words[1]), day23_argt::reg, words[2][0]);
            } else {
                return day23_i(day23_t::jnz, day23_argt::val, stoi(words[1]), day23_argt::val, stoi(words[2]));
            }
        }
    } else if (words[0] == "tgl") {
        if (words[1] == "a" || words[1] == "b" || words[1] == "c" || words[1] == "d") {
            return day23_i(day23_t::tgl, day23_argt::reg, words[1][0]);
        } else {
            return day23_i(day23_t::tgl, day23_argt::val, stoi(words[1]));
        }
    }
}

class day23_program {
private:
    vector<day23_i> instructions;
    vector<day23_i> default_instr;
    int registers[4];
    size_t iptr;
    size_t reg(int regval) {
        // a = 0, b = 1, c = 2, d = 3;
        return regval - 97;
    }
    void toggle_instruction(int n) {
        // If an attempt is made to toggle an instruction outside the program, nothing happens.
        if((n<0) || (n>=instructions.size())) return;
        day23_i i = instructions[n];
        switch(i.t) {
            case day23_t::inc:
                // inc becomes dec
                i.t = day23_t::dec;
                break;
            case day23_t::dec:
            case day23_t::tgl:
                // All other one-argument instructions (dec and tgl) become inc
                i.t = day23_t::inc;
                break;
            case day23_t::jnz:
                // jnz becomes cpy
                i.t = day23_t::cpy;
                break;
            case day23_t::cpy:
                // All other two-instructions (cpy) become jnz.
                i.t = day23_t::jnz;
                break;
        }
        // Save toggled instruction
        // The arguments of a toggled instruction are not affected.
        instructions[n] = i;
    }
    void apply_instruction(day23_i i) {
        switch(i.t) {
            case day23_t::cpy:
                // First arg can be reg or val, Second arg always reg
                // If 2nd arg is val, skip
                if (i.at2 == day23_argt::val) {
                    // Skip
                    iptr++;
                } else {
                    switch (i.at1) {
                        case day23_argt::reg:
                            // Copy reg(a1) to reg(a2)
                            registers[reg(i.a2)] = registers[reg(i.a1)];
                            iptr++;
                            break;
                        case day23_argt::val:
                            // Copy val(a1) to reg(a2)
                            registers[reg(i.a2)] = i.a1;
                            iptr++;
                            break;
                    }
                }
                break;
            case day23_t::dec:
                if (i.at1 == day23_argt::val) {
                    // Skip
                    iptr++;
                } else {
                    // Decrease reg(a1)
                    registers[reg(i.a1)]--;
                    iptr++;
                }
                break;
            case day23_t::inc:
                if (i.at1 == day23_argt::val) {
                    // Skip
                    iptr++;
                } else {
                    // Increase reg(a1)
                    registers[reg(i.a1)]++;
                    iptr++;
                }
                break;
            case day23_t::jnz:
                // Check first arg
                switch (i.at1) {
                    case day23_argt::reg:
                        // Check if first arg is zero
                        if (registers[reg(i.a1)]!=0) {
                            // Check second arg
                            switch (i.at2) {
                                case day23_argt::reg:
                                    iptr += registers[reg(i.a2)];
                                    break;
                                case day23_argt::val:
                                    iptr += i.a2;
                                    break;
                            }
                        } else {
                            iptr++;
                        }
                        break;
                    case day23_argt::val:
                        // Check if first value argument is zero
                        if (i.a1 != 0) {
                            // Check second arg
                            switch (i.at2) {
                                case day23_argt::reg:
                                    iptr += registers[reg(i.a2)];
                                    break;
                                case day23_argt::val:
                                    iptr += i.a2;
                                    break;
                            }
                        } else {
                            iptr++;
                        }
                        break;
                }
                break;
            case day23_t::tgl:
                size_t target = 0;
                switch (i.at1) {
                    case day23_argt::reg:
                        target = iptr + registers[reg(i.a1)];
                        break;
                    case day23_argt::val:
                        target = iptr + i.a1;
                        break;
                }
                // Toggle target
                toggle_instruction(target);
                // Continue execution
                iptr++;
                break;
        }
    }
public:
    day23_program(const vector<day23_i> &i) : instructions(i) {
        iptr = 0;
        default_instr = i;
    }
    void run(int a, int b, int c, int d) {
        // Init registers and reset instructions
        registers[0] = a;
        registers[1] = b;
        registers[2] = c;
        registers[3] = d;
        instructions = default_instr;
        iptr = 0;
        while((iptr >= 0) && (iptr < instructions.size())) {
            apply_instruction(instructions[iptr]);
        }
        cout << "a,b,c,d = " << registers[0] << ", " << registers[1] << ", " << registers[2] << ", " << registers[3] << endl;
    }
};

void day23(string filename,int a, int b, int c, int d) {
    ifstream input(filename);
    string line;
    vector<day23_i> instructions;
    while (getline(input, line)) {
        stringstream ss;
        ss.str(line);
        vector<string> words;
        string word;
        while (getline(ss, word, ' ')) {
            words.push_back(word);
        }
        instructions.push_back(day23_parse(words));
    }
    cout << "Assembunny code compiled, total number of instructions: " << instructions.size() << endl;
    day23_program program(instructions);
    program.run(a,b,c,d);

}

int main() {


    // Day 17 solution
    //day17(day17_input);

    // Day 18 solution
    //day18(day18_input, 40);
    //day18_count_only(day18_input, 400000);

    // Day 20 solution
    //day20("day20_input.txt");

    // Day 21 solution
    //day21("day21_input.txt","abcdefgh","fbgdceah");

    // Day 22 solution
    //day22("day22_input.txt");

    // Day 23 solution
    //day23("day12_input.txt",0,0,0,0);
    //day23("day23_input.txt",7,0,0,0);
    //day23("day23_input.txt",12,0,0,0);

    return 0;

}