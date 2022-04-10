namespace sol2 {
    bool apply[Server_limit];
    int used[Server_limit];
    int rest[Server_limit];

    struct pkg {
        int i, k, size;
        pkg() {}
        pkg(int i, int k, int size) : i(i), k(k), size(size) {}
    };

    bool operator < (pkg A, pkg B) {
        return A.size > B.size;
    }

    inline bool update_t(int t) {
        for (int j=1;j<=N;j++) used[j] = 0;

        vector<pkg> V;
        V.clear();

        for (int i=1;i<=M;i++) {
            for (int k=1;k<=F;k++) if (Demand[t][i][k]) {
                V.push_back(pkg(i, k, Demand[t][i][k]));
            }
        }

        sort(V.begin(), V.end());
        
        for (pkg now : V) {
            int i = now.i;
            int k = now.k;

            int pos = -1;
            for (int j=1;j<=N;j++) if (apply[j] && LinkAble[i][j] && used[j] + now.size <= MaxBandwidth[j]) {
                if (pos == -1 || used[pos] > used[j]) pos = j;
            }

            if (pos == -1) {
                return false;
            }
            used[pos] += now.size;
            Answer[t][i][k] = pos;
        }

        return true;
    }

    bool calc() {
        for (int j=1;j<=N;j++) if (apply[j]) rest[j] = T05;

        for (int t=1;t<=T;t++) {
            CheckTime();
            if (TLE_flag) return false;

            if (!update_t(t)) return false;
        }

        return true;
    }

    vector<int> st0, st1, st;
    inline void init_st() {
        st0.clear();
        st1.clear();
        for (int j=1;j<=N;j++) (apply[j] ? st1 : st0).push_back(j);
        random_shuffle(st0.begin(), st0.end());
        random_shuffle(st1.begin(), st1.end());
    }

    void main() {
        for (int j=1;j<=N;j++) apply[j] = true;
        for (int j=1;j<=N;j++) st.push_back(j);
        calc();

        double p = 0.8;
        int last = BestAnswer;
        
        while (1) {
            if (TLE_flag) break;
            random_shuffle(st.begin(), st.end());

            int cnt = rand() % 4 + 1;
            for (int i=0;i<cnt;i++) apply[st[i]] = !apply[st[i]];

            bool flag = calc();
            int now;

            if (flag) {
                now = io.check();
                
                if (rand()%1024 > p*1024) flag = (now < last);
                p *= 0.99;

                // printf("cnt = %d | ", cnt);
                // for (int i=0;i<cnt;i++) printf("<%d,%d> ", st[i], apply[st[i]]);
            }

            if (!flag) {
                for (int i=0;i<cnt;i++) apply[st[i]] = !apply[st[i]];
            } else {
                last = now;
                printf("last = %d\n", last);
                printf("ans = %d\n\n", BestAnswer);
            }
        }
    }
}