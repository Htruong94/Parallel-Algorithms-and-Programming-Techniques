// Workshop 6 - Word Count Algorithm
// WordCount.h
// Hung Truong
// 2021-07-17

#pragma once
#include <tbb/tbb.h>

class WordCount {
	const char* str;
	int len;
	int* size;
	int* numb;
    bool (*delimiter)(char);
public:
    WordCount(const char* st, int* sz, int* n, int l, bool(*f)(char d)) :
        str(st), size(sz), numb(n), len(l), delimiter(f) {
	}
    void operator()(const tbb::blocked_range<int>& r) const {
        for (int i = r.begin(); i < r.end(); i++) {
            if (!delimiter(str[i])) {
                int s = 0;
                while (i + s < len && !delimiter(str[i + s])) s++;
                size[i] = s;
                int n = 0;
                for (int j = i + s + 1; j + s < len; j++) {
                    bool bad = false;
                    for (int k = 0;
                        k < s && k + i < len && k + j < len; k++) {
                        if (str[i + k] != str[j + k]) {
                            bad = true;
                            break;
                        }
                    }
                    if (!bad && delimiter(str[j + s])) n++;
                }
                numb[i] = n;
            }
            else {
                size[i] = 0;
                numb[i] = 0;
            }
            i += size[i];
        }
    }
};