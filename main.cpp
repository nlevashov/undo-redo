//Программа не в realtime (redo вычитается из undo)

#include <iostream>
#include <list>
#include <string>
#include <stack>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

list<string> data;

struct Tcurs {
	int x;
	list<string>::iterator y;
} curs;

struct Toper {
	int type;
	char sym;
	Tcurs pos;
};
stack<Toper> opers;

void check_curs()
{
	if (curs.x < 0 || curs.x > (*(curs.y)).length()) {
		cout << "Cursor out" << endl;
		exit(EXIT_SUCCESS);
	}
}


void ins(const char c, bool flag = true)
{
	if (c == '\n') {
		if (flag) {
			Toper oper;
			oper.type = 0;
			oper.pos = curs;
			opers.push(oper);
		}

		auto where = curs.y;
		++(curs.y);
		if ((*where).length() - curs.x > 0) {
			data.insert(curs.y, (*where).substr(curs.x));
		} else data.insert(curs.y, "");
		(*where).resize(curs.x);
		curs.x = 0;
		curs.y = ++where;
	} else {
		if (curs.x < (*(curs.y)).length()) {
			if (flag) {
				Toper oper;
				oper.type = 1;
				oper.sym = (*(curs.y))[curs.x];
				oper.pos = curs;
				opers.push(oper);
			}

			(*(curs.y))[curs.x] = c;
		} else {
			if (flag) {
				Toper oper;
				oper.type = 0;
				oper.pos = curs;
				opers.push(oper);
			}

			(*(curs.y)).push_back(c);
		}
		curs.x++;
	}
}

void del(const int n, bool flag = true)
{
	int r = n, diff;
	while (r > 0) {
		if (curs.x == (*(curs.y)).length()) {
			auto next = curs.y; next++;
			if (next == data.end()) {
				cout << "no symbols to delete" << endl;
				exit(EXIT_SUCCESS);
			}
			(*(curs.y)).insert(curs.x, *next);
			data.erase(next);
			r--;

			if (flag) {
				Toper oper;
				oper.type = 2;
				oper.pos = curs;
				opers.push(oper);
			}
		} else if (r > (diff = (*(curs.y)).length() - curs.x)) {
			if (flag) {
				for (int i = curs.x; i < curs.x + diff; i++) {
					Toper oper;
					oper.type = 3;
					oper.sym = (*(curs.y))[i];
					oper.pos = curs;
					opers.push(oper);
				}
			}

			(*(curs.y)).erase(curs.x, diff);
			r -= diff;
		} else {
			if (flag) {
				for (int i = curs.x; i < curs.x + r; i++) {
					Toper oper;
					oper.type = 3;
					oper.sym = (*(curs.y))[i];
					oper.pos = curs;
					opers.push(oper);
				}
			}

			(*(curs.y)).erase(curs.x, r);
			r = 0;
		}
	}
}

void undo(const int n)
{
	for (int i = 0; i < n; i++) {
		Toper oper = opers.top();

		switch (oper.type) {
		case 0:
			curs = oper.pos;
			del(1, 0);
			break;
		case 1:
			curs = oper.pos;
			(*(curs.y))[curs.x] = oper.sym;
			break;
		case 2:
			curs = oper.pos;
			ins('\n', 0);
			curs = oper.pos;
			break;
		case 3:
			curs = oper.pos;
			(*(curs.y)).insert(curs.x, 1, oper.sym);
			break;
		}

		opers.pop();
	}
}

void print_ans()
{
	cout << endl << "OUTPUT:" << endl;
	for (auto i = data.begin(); i != data.end(); i++) cout << *i << endl;
}


int main ()
{
	data.push_back("");
	curs.x = 0;
	curs.y = data.begin();

	char c = getchar();

	while (1) {
		//move curs
		bool flag = true;
		while (1) {
			switch (c) {
			case '<': curs.x--; break;
			case '>': curs.x++; break;
			case '^': curs.y--; break;
			case '|': curs.y++; break;
			case EOF: print_ans(); return 0;
			default: check_curs(); flag = false;
			}

			if (! flag) break;
			c = getchar();
		}

		//insert symbols
		flag = true;
		while (1) {
			switch (c) {
			case '<': flag = false; break;
			case '>': flag = false; break;
			case '^': flag = false; break;
			case '|': flag = false; break;
			case '#': flag = false; break;
			case '@': flag = false; break;
			case '$': flag = false; break;
			case EOF: print_ans(); return 0;
			default: ins(c);
			}

			if (! flag) break;
			c = getchar();
		}

		//delete symbols
		flag = true;
		int i = 0;
		while (1) {
			switch (c) {
			case '#': i++; break;
			case EOF: del(i); print_ans(); return 0;
			default: del(i); flag = false;
			}

			if (! flag) break;
			c = getchar();
		}

		//undo redo
		flag = true;
		i = 0;
		while (1) {
			switch (c) {
			case '@': i++; break;
			case '$': i--; break;
			case EOF: undo(i); print_ans(); return 0;
			default: undo(i); flag = false;
			}

			if (! flag) break;
			c = getchar();
		}
	}
}
