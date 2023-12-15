#include <vector>
#include <string>
#include <map>
#include <unordered_map>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <cstdint>
#include <sys/stat.h>

#define CLR_CODE 256
#define NEW_CODE 257

using namespace std;

unordered_map<string, uint16_t> get_dict() {
    unordered_map<string, uint16_t> dict;
    for (uint16_t i = 0; i < 256; ++i) dict[string(1, char(i))] = i;
    return dict;
}

unordered_map<uint16_t, string> get_inv_dict() {
    unordered_map<uint16_t, string> dict;
    for (uint16_t i = 0; i < 256; ++i) dict[i] = string(1, char(i));
    return dict;
}

int encode(int in_fd, int out_fd) {
    auto dict = get_dict();
    uint16_t code = NEW_CODE;

    int rc = 0;
    char ch;
    if ((rc = read(in_fd, &ch, sizeof(char))) < 0) {
        perror("read char error");
        return -1;
    }
    if(rc == 0) return 0;
    string p(1, ch);

    while ((rc = read(in_fd, &ch, sizeof(char))) > 0) {
        if (dict.contains(p + ch)) p += ch;
        else {
            if (write(out_fd, &dict[p], sizeof(uint16_t)) < 0) {
                perror("write code error");
                return -1;
            }
            dict[p + ch] = code++;
            p = ch;
        }
        if(code == INT16_MAX){
            if (!p.empty()) {
                if (write(out_fd, &dict[p], sizeof(uint16_t)) < 0) {
                    perror("write code error");
                    return -1;
                }
            }

            dict = get_dict();
            code = NEW_CODE;

            if ((rc = read(in_fd, &ch, sizeof(char))) < 0) {
                perror("read char error");
                return -1;
            }
            if(rc == 0) return 0;
            p = ch;

            uint16_t tmp = CLR_CODE;
            if (write(out_fd, &tmp, sizeof(uint16_t)) < 0) {
                perror("write clr code error");
                return -1;
            }
        }
    }
    if (rc < 0) {
        perror("read char error");
        return -1;
    }
    if (!p.empty()) {
        if (write(out_fd, &dict[p], sizeof(uint16_t)) < 0) {
            perror("write code error");
            return -1;
        }
    }

    return 0;
}

int write_str(int out_fd, const string &str) {
    for (auto ch: str)
        if (write(out_fd, &ch, sizeof(char)) < 0)
            return -1;
    return 0;
}

int decode(int in_fd, int out_fd) {
    auto dict = get_inv_dict();
    uint16_t code = NEW_CODE;

    string s, entry;
    uint16_t k;
    int rc = 0;
    if ((rc = read(in_fd, &k, sizeof(uint16_t))) < 0){
        perror("read code error");
        return -1;
    }
    if(rc == 0) return 0;
    s = dict[k];
    if(write_str(out_fd, s) < 0){
        perror("write str error");
        return -1;
    }

    while ((rc = read(in_fd, &k, sizeof(uint16_t))) > 0) {
        if(k == CLR_CODE){
            dict = get_inv_dict();
            code = NEW_CODE;

            if ((rc = read(in_fd, &k, sizeof(uint16_t))) < 0){
                perror("read code error");
                return -1;
            }
            if(rc == 0) return 0;
            s = dict[k];
            if(write_str(out_fd, s) < 0){
                perror("write str error");
                return -1;
            }
            continue;
        }

        if (dict.contains(k))
            entry = dict[k];
        else if (k == code)
            entry = s + s[0];
        else
            throw exception();

        if(write_str(out_fd, entry) < 0){
            perror("write str error");
            return -1;
        }
        dict[code++] = s + entry[0];
        s = entry;
    }
    if (rc < 0) {
        perror("read code error");
        return -1;
    }

    return 0;
}

int compress(char *in, char *out) {
    int in_fd = open(in, O_RDONLY);
    if (in_fd < 0) {
        perror("open inp file error");
        return -1;
    }

    int out_fd = open(out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out_fd < 0) {
        perror("open out file error");
        close(in_fd);
        return -1;
    }

    int rc = encode(in_fd, out_fd);
    close(in_fd);
    close(out_fd);

    struct stat st_in{}, st_out{};
    stat(in, &st_in);
    stat(out, &st_out);
    printf("Compression ratio %.4f\n", float (st_in.st_size) / float (st_out.st_size));

    return rc;
}

int decompress(char *in, char *out) {
    int in_fd = open(in, O_RDONLY);
    if (in_fd < 0) {
        perror("open inp file error");
        return -1;
    }

    int out_fd = open(out, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (out_fd < 0) {
        perror("open out file error");
        close(in_fd);
        return -1;
    }

    int rc = decode(in_fd, out_fd);
    close(in_fd);
    close(out_fd);
    return rc;
}

int main(const int argc, char *argv[]) {
    if (argc < 4) {
        printf("not enough args");
        return -1;
    }

    if (strcmp(argv[1], "C") == 0) {
        return compress(argv[2], argv[3]);
    } else if (strcmp(argv[1], "D") == 0) {
        return decompress(argv[2], argv[3]);
    }
    printf("wrong mode");
    return 0;

//    string s = "abababab";
//    auto d = en(s);
//    auto e = dec(d);
//    return 0;
}

string dec(const vector<int> &v) {
    map<int, string> inv_dict;
    int dictSize = 256;
    for (int i = 0; i < dictSize; ++i)
        inv_dict[i] = string(1, i);

    string s, entry, res;
    s = res = inv_dict[v[0]];
    for (size_t i = 1; i < v.size(); ++i) {
        int k = v[i];

        if (inv_dict.count(k))
            entry = inv_dict[k];
        else if (k == dictSize)
            entry = s + s[0];
        else throw "error";

        res += entry;
        inv_dict[dictSize++] = s + entry[0];
        s = entry;
    }
    return res;
}

vector<int> en(const string &ori) {
    map<string, int> dict;
    int dictSize = 256;
    for (int i = 0; i < dictSize; ++i)
        dict[string(1, i)] = i;
    vector<int> res;
    string s;
    for (char z: ori) {
        if (dict.count(s + z)) s += z;
        else {
            res.emplace_back(dict[s]);
            dict[s + z] = dictSize++;
            s = z;
        }
    }
    if (s.size()) res.emplace_back(dict[s]);
    return res;
}

