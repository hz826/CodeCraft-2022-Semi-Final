class IO {
private:
    const bool debug = false;

    FILE * getfile(const char * filename) {
        const char * data_path = OnlineJudge ? "/data/" : "../data/";
        char path[100];
        sprintf(path, "%s%s", data_path, filename);

        FILE * fp = fopen(path, "r");
        return fp;
    }

    char value[40][100], valuenum;

    void csvgetline(FILE * fp) {
        static char line[4000];
        if (fscanf(fp, "%s", line) == EOF) {valuenum = 0;  return ;}
        int len = strlen(line), j = 0, k = 0;
        line[len] = ',';

        for (int i=0;i<=len;i++) {
            if (line[i] == ',') {
                value[j][k++] = '\0';
                j++, k=0;
            } else {
                value[j][k++] = line[i];
            }
        }
        valuenum = j;
    }

    typedef char Name[10];
    Name Mname[User_limit], Nname[Server_limit], Fname[T_limit][Flow_limit];
    map<string, pair<int,int> > Timeid;

    int getid(Name *a, int n, Name b) {
        for (int i=1;i<=n;i++) if (strcmp(a[i], b) == 0) return i;
        printf("getid ERROR!\n");
        return -1;
    }

public:
    void read() {
        FILE * fp;

        if (debug) printf("reading demand.csv\n");
        fp = getfile("demand.csv");
        csvgetline(fp);
        M = valuenum - 2;
        F = 100;

        for (int i=1;i<=M;i++) strcpy(Mname[i], value[i+1]);
        while (1) {
            csvgetline(fp);
            if (!valuenum) break;
            
            string t = value[0];
            if (Timeid.find(t) == Timeid.end()) Timeid[t] = make_pair(++T, 0);
            auto it = Timeid.find(t);
            it->second.second++;

            strcpy(Fname[it->second.first][it->second.second], value[1]);

            for (int i=1;i<=M;i++) sscanf(value[i+1], "%d", &Demand[T][i][it->second.second]);
        }
        fclose(fp);

        T95 = (T * 95 + 99) / 100;
        T05 = T - T95;

        T90 = (T * 90 + 99) / 100;
        T10 = T - T90;

        if (debug) printf("reading site_bandwidth.csv\n");
        fp = getfile("site_bandwidth.csv");
        csvgetline(fp);
        while (1) {
            csvgetline(fp);
            if (!valuenum) break;
            N++;
            strcpy(Nname[N], value[0]);
            sscanf(value[1], "%d", &MaxBandwidth[N]);
        }
        fclose(fp);

        if (debug) printf("reading qos.csv\n");
        fp = getfile("qos.csv");
        csvgetline(fp);
        int id[User_limit];
        for (int i=1;i<=M;i++) id[i] = getid(Mname, M, value[i]);
        if (debug) for (int i=1;i<=M;i++) printf("%d%c", id[i], " \n"[i==M]);
        for (int i=1;i<=N;i++) {
            csvgetline(fp);
            int ii = getid(Nname, N, value[0]);
            for (int j=1;j<=M;j++) sscanf(value[j], "%d", &LinkAble[id[j]][ii]);
        }
        fclose(fp);

        if (debug) printf("reading config.ini\n");
        fp = getfile("config.ini");
        csvgetline(fp);
        csvgetline(fp);
        int Q;
        sscanf(value[0], "qos_constraint=%d", &Q);
        if (debug) printf("Q = %d\n", Q);
        for (int i=1;i<=M;i++) for (int j=1;j<=N;j++) LinkAble[i][j] = (LinkAble[i][j] < Q) ? 1 : 0;
        csvgetline(fp);
        sscanf(value[0], "base_cost=%d", &BaseCost);
        fclose(fp);
    }

    // void get_big_data() {
    //     N = 135;
    //     M = 35;
    //     T = 8928;
    //     T95 = (T * 95 + 99) / 100;
    //     T05 = T - T95;
    //     for (int t=1;t<=T;t++) for (int i=1;i<=M;i++) Usage[t][i] = rand() % 10000;
    //     for (int i=1;i<=N;i++) C[i] = 1e6;
    //     for (int i=1;i<=M;i++) for (int j=1;j<=N;j++) {
	// 	        LinkAble[i][j] = rand() & 1;
    //     }
    // }

    void show() {
        printf("M = %d\tN = %d\tT = %d\tF = %d\tBaseCost = %d\n", M, N, T, F, BaseCost);
        printf("\n");
        printf("M name :\n");
        for (int i=1;i<=M;i++) printf("%s ", Mname[i]);
        printf("\n\n");
        printf("N name :\n");
        for (int i=1;i<=N;i++) printf("%s ", Nname[i]);
        printf("\n\n");

        printf("Demand : %d * %d * %d\n", T, M, F);
        for (int i=1;i<=T;i++) {
            for (int j=1;j<=M;j++) {
                printf("%d %d | ", i, j);
                for (int k=1;k<=F;k++) printf("%d ", Demand[i][j][k]);
                printf("\n");
            }
        }
        printf("\n");

        printf("MaxBandwidth : %d\n", N);
        for (int i=1;i<=N;i++) printf("%d ", MaxBandwidth[i]);
        printf("\n\n");

        printf("LinkAble : %d * %d\n", M, N);
        for (int i=1;i<=M;i++) {
            for (int j=1;j<=N;j++) printf("%d ", LinkAble[i][j]);
            printf("\n");
        }
    }

    int check() { // 检查输出合法性，并计算答案
        static int sum[Server_limit];

        for (int t=1;t<=T;t++) {
            for (int j=1;j<=N;j++) sum[j] = 0;

            for (int i=1;i<=M;i++) {
                for (int k=1;k<=F;k++) if (Demand[t][i][k]) {
                    if (Answer[t][i][k] < 1 || Answer[t][i][k] > N) {
                        printf("%d %d %d\n", t, i, k);
                        printf("%d %d\n", Demand[t][i][k], Answer[t][i][k]);
                        printf("ERROR : answer not in [1,N]\n");
                        return -1;
                    }
                    sum[Answer[t][i][k]] += Demand[t][i][k];
                }
            }

            for (int j=1;j<=N;j++) {
                if (sum[j] > MaxBandwidth[j]) {
                    printf("ERROR : sum > MaxBandwidth\n");
                    return -1;
                }

                Bandwidth[j][t] = make_pair(sum[j], t);
            }
        }

        double score_d = 0;
        for (int j=1;j<=N;j++) {
            sort(Bandwidth[j]+1, Bandwidth[j]+T+1);
            if (sum[j] > 0) {
                int w = Bandwidth[j][T95].first;
                if (w <= BaseCost) score_d += BaseCost;
                else score_d += 1.0  * (w - BaseCost) * (w - BaseCost) / MaxBandwidth[j] + w;
            }
        }
        int score = (int) (score_d + 0.5);

        if (score < BestAnswer) {
            BestAnswer = score;
            NewRecord = true;
            memcpy(AnswerSave, Answer, sizeof(Answer));
            V10_SAVE = V10_ANS;
        } else {
            NewRecord = false;
        }
        
        return score;
    }

    void write() {
        static vector<int> Solution[Server_limit];

        const char * data_path = OnlineJudge ? "/output/" : "../";
        char path[100];
        sprintf(path, "%s%s", data_path, "solution.txt");

        FILE * fp = fopen(path, "w");
        memcpy(Answer, AnswerSave, sizeof(Answer));
        V10_ANS = V10_SAVE;

        bool fflag = false;
        for (int j : V10_ANS) {
            if (fflag) fprintf(fp, ",");
            fflag = true;
            fprintf(fp, "%s", Nname[j]);
        }
        fprintf(fp, "\n");
        
        for (int t=1;t<=T;t++) {
            for (int i=1;i<=M;i++) {
                fprintf(fp, "%s:", Mname[i]);

                for (int j=1;j<=N;j++) Solution[j].clear();
                for (int k=1;k<=F;k++) if (Demand[t][i][k]) Solution[Answer[t][i][k]].push_back(k);

                bool flag = false;
                for (int j=1;j<=N;j++) if (!Solution[j].empty()) {
                    if (flag) fprintf(fp, ",");
                    flag = true;

                    fprintf(fp, "<%s", Nname[j]);
                    for (int k : Solution[j]) fprintf(fp, ",%s", Fname[t][k]);
                    fprintf(fp, ">");
                }
                fprintf(fp, "\n");
            }
        }
        fclose(fp);
    }
} io;