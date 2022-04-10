namespace sol2 {
    bool apply[Server_limit];
    int used[Server_limit];

    struct pkg {
        int i, k, size;
        pkg() {}
        pkg(int i, int k, int size) : i(i), k(k), size(size) {}
    };

    bool operator < (pkg A, pkg B) {
        return A.size > B.size;
    }

    bool extend[T_limit][Server_limit];

    inline bool update_t(int t) {
        for (int j=1;j<=N;j++) used[j] = 0;

        vector<pkg> V, VV;
        V.clear();
        VV.clear();

        for (int i=1;i<=M;i++) {
            for (int k=1;k<=F;k++) if (Demand[t][i][k]) {
                V.push_back(pkg(i, k, Demand[t][i][k]));
            }
        }

        sort(V.begin(), V.end());

        for (int j=1;j<=N;j++) if (extend[t][j]) {
            int rest = MaxBandwidth[j];

            bool flag = true;
            while (flag) {
                flag = false;

                for (auto it=V.begin();it!=V.end();++it) if (LinkAble[it->i][j] && it->size <= rest) {
                    rest -= it->size;
                    Answer[t][it->i][it->k] = j;
                    V.erase(it);
                    flag = true;
                    break;
                }
            }
        }

        vector<int> tmp;
        for (int jj=1;jj<=N;jj++) tmp.push_back(jj);

        for (const pkg &now : V) {
            int i = now.i;
            int k = now.k;

            int pos = -1;
            random_shuffle(tmp.begin(), tmp.end());
            for (int jj=1;jj<=N;jj++) {
                int j = tmp[jj];
                if (apply[j] && !extend[t][j] && LinkAble[i][j] && used[j] + now.size <= min(BaseCost, MaxBandwidth[j])) {
                    if (pos == -1 || used[pos] > used[j]) pos = j;
                }
            }

            if (pos == -1) {
                // VV.push_back(now);
                return false;
            }
            used[pos] += now.size;
            Answer[t][i][k] = pos;
        }

        // for (const pkg &now : VV) {
        //     int i = now.i;
        //     int k = now.k;

        //     int pos = -1;
        //     random_shuffle(tmp.begin(), tmp.end());
        //     for (int jj=1;jj<=N;jj++) {
        //         int j = tmp[jj];
        //         if (apply[j] && !extend[t][j] && LinkAble[i][j] && used[j] + now.size <= MaxBandwidth[j]) {
        //             if (pos == -1 || used[pos] > used[j]) pos = j;
        //         }
        //     }

        //     if (pos == -1) {
        //         return false;
        //     }
        //     used[pos] += now.size;
        //     Answer[t][i][k] = pos;
        // }

        return true;
    }

    bool calc() {
        for (int t=1;t<=T;t++) {
            CheckTime();
            if (TLE_flag) return false;
            if (!update_t(t)) return false;
        }

        memset(extend, 0, sizeof(extend));
        // for (int t=1;t<=T05;t++) {
        //     io.check();
        //     for (int j=1;j<=N;j++) if (apply[j]) {
        //         CheckTime();
        //         if (TLE_flag) return false;

        //         int tt = Bandwidth[j][T-t].second;
        //         extend[tt][j] = true;
        //         update_t(tt);
        //     }
        // }

        // V10_ANS.clear();
        // io.check();
        // vector<pair<int,int>> tmp;
        // for (int j=1;j<=N;j++) if (apply[j]) {
        //     tmp.push_back(make_pair(-Bandwidth[j][T95].first, j));
        // }
        // sort(tmp.begin(), tmp.end());
        // for (int j=0;j<10;j++) V10_ANS.push_back(tmp[j].second);

        // for (int t=T05+1;t<=T10;t++) {
        //     io.check();
        //     for (int j : V10_ANS) {
        //         CheckTime();
        //         if (TLE_flag) return false;

        //         int tt = Bandwidth[j][T-t].second;
        //         extend[tt][j] = true;
        //         update_t(tt);
        //     }
        // }
        
        return true;
    }

    inline double random_float_01() {
        return 1.0 * (rand() % 4096) / 4096;
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

        const double RATE = 0.8, IN_LOOP = 30;
        const int T_gen = 100;
        int last = BestAnswer;

        double TEM = -1;
        vector<int> answers;
        
        for (int task_id=0;;) {
            if (TLE_flag) break;

            for (int loop=1;loop<=IN_LOOP;loop++) {
                if (TLE_flag) break;
                random_shuffle(st.begin(), st.end());

                int cnt = rand() % 4 + 1;
                for (int i=0;i<cnt;i++) apply[st[i]] = !apply[st[i]];

                bool sol_valid = calc();
                bool sol_accept = sol_valid;
                int now;

                if (sol_valid) {
                    now = io.check();
                    int delta = now - last;

                    if (task_id < T_gen) {
                        answers.push_back(now);
                        if (delta >= 0) sol_accept = false;
                    }
                    if (task_id == T_gen) {
                        sort(answers.begin(), answers.end());
                        TEM = answers[70] - answers[30];
                    }

                    if (delta >= 0 && task_id >= T_gen) {
                        printf("try to accept a bad sol\n");
                        printf("delta = %d  TEM = %f\n", delta, TEM);
                        printf("f = %f\n", exp(-delta / TEM));
                        printf("\n");

                        if (exp(-delta / TEM) > random_float_01()) {
                            sol_accept = true;
                            printf("ACCEPT\n");
                        } else {
                            sol_accept = false;
                            printf("NOT ACCEPT\n");
                        }                        
                    }

                    // printf("cnt = %d | ", cnt);
                    // for (int i=0;i<cnt;i++) printf("<%d,%d> ", st[i], apply[st[i]]);
                }

                if (!sol_accept) {
                    for (int i=0;i<cnt;i++) apply[st[i]] = !apply[st[i]];
                } else {
                    last = now;
                    printf("last = %d\n", last);
                }
                printf("taskid = %d\n", task_id);
                printf("ans = %d\n\n", BestAnswer);
                ++task_id;
            }

            TEM = TEM * RATE;
        }
    }
}