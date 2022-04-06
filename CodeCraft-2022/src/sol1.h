namespace sol1 {
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

    bool calc() {
        vector<pkg> V;

        for (int t=1;t<=T;t++) {
            for (int j=1;j<=N;j++) used[j] = 0;

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
                for (int j=1;j<=N;j++) if (LinkAble[i][j] && used[j] + now.size <= MaxBandwidth[j]) {
                    if (pos == -1 || used[pos] > used[j]) pos = j;
                }

                if (pos == -1) {
                    return false;
                }
                used[pos] += now.size;
                Answer[t][i][k] = pos;
            }
        }

        return true;
    }

    void main() {
        calc();
        printf("IO : answer = %d\n", io.check());
    }
}