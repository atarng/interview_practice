    /*
    // Intervals are not necessarily (beg, end), it could be inserted as (end, beg)
    void InsertionHelper(pair<int,int>& to_insert,
                         vector<pair<int,int>>& to_populate) {
        // printf("InsertionHelper] I:(%d,%d)\n", to_insert.first, to_insert.second);
        // First find where to try to insert the new interval
        int beg = 0, end = to_populate.size();
        int mid = 0;
        while(beg < end ) {
            mid = beg + (end - beg) / 2;
            auto right_interval = to_populate[mid];
            // printf("InsertionHelper] R: (%d,%d)\n", right_interval.first, right_interval.second);
            // If they cross, this can never be inserted
            if((to_insert.first <= right_interval.first
             && to_insert.second >= right_interval.second) || 
               (to_insert.first >= right_interval.first
             && to_insert.second <= right_interval.second)) {
                return;
            }// If the Right side interval exceeds the End of our
            //  interval to insert, check the Upper half.
            else if(max(to_insert.first, to_insert.second) >
                      min(right_interval.first, right_interval.second)) {
                beg = mid + 1;
                continue;
            } // Check to see if we even have a left interval
            if (mid - 1 >= beg) {
                auto left_interval = to_populate[--mid];
                printf("InsertionHelper] L: (%d,%d)\n", left_interval.first, left_interval.second);
                if((to_insert.first <= left_interval.first
                 && to_insert.second >= left_interval.second) || 
                   (to_insert.first >= left_interval.first
                 && to_insert.second <= left_interval.second)) {
                    return;
                }// If the left side interval exceeds the beginning of our
                //  interval to insert, check the lower half.
                else if(max(left_interval.first, left_interval.second) >
                        min(to_insert.first, to_insert.second)) {
                    end = mid - 1;
                    continue;
                }
            } // If it made it here we can exit the loop
            break;
        } // Update Mid one last time.
        mid = end + (beg - end) / 2;
        // printf("InsertionHelper] Test Insert: (%d,%d) to: %d\n",
        //         to_insert.first, to_insert.second, mid);
        // Insert at our last valid location.
        to_populate.insert(to_populate.begin() + mid, to_insert);
        for(int i = 0; i < to_populate.size(); ++i) {
            auto p = to_populate[i];
            printf("(%d,%d)%s", p.first, p.second, ((i + 1) % 10 ? " " : "\n"));
        } cout << endl;
    }
    // Associate all the values to their beg (A), end (B) indices.
    /// This might mean we can use a map.
    // Have the map sorted by the distance between the two indices.
    // Iterate through the Map, keeping track of which ranges are no longer valid.
    //
    // Since we are constrained to lengths of 500, it's not too bad to allocate extra
    // bufferspace to keep track?
    //
    // Maybe just push intervals consumed, merging them
    // as needed and comparing against the current interval.
    int myMaxUncrossedLines(vector<int>& A, vector<int>& B) {
        int pc = 0;
#if defined(DEBUG)
        cout << "A:" << endl;
        for(int i = 0; i < A.size(); ++i) {
            printf("%d%s", A[i], ((i + 1) % 10 ? " " : "\n"));
        } cout << endl << "B: " << endl;
        for(int i = 0; i < B.size(); ++i) {
            printf("%d%s", B[i], ((i + 1) % 10 ? " " : "\n"));
        } cout << endl;
#endif
        // Map Key to Indices
        /// vector<int> is populated in ascending order (front - back)
        unordered_map<int, vector<int>> map_A_I;
        unordered_map<int, vector<int>> map_B_I;
        for(int i = 0; i < A.size() || i < B.size(); ++i) {
            if(i < A.size()) {
                map_A_I[A[i]].push_back(i);
            } if (i < B.size()) {
                map_B_I[B[i]].push_back(i);
            }
        }
#if defined(DEBUG)
        printf("myMaxUncrossedLines] AMapI:\n");
        for(auto a_p : map_A_I) {
            printf("(%d,%lu)%s", a_p.first, a_p.second.size(), (++pc % 10 ? " " : "\n"));
        } cout << endl;
        pc = 0;
        printf("myMaxUncrossedLines] BMapI:\n");
        for(auto b_p : map_B_I) {
            printf("(%d,%lu)%s", b_p.first, b_p.second.size(), (++pc % 10 ? " " : "\n"));
        } cout << endl;
#endif
        // Each possible indice connection is unique
        auto custom_cmp = [](pair<int,int> L, pair<int,int> R){
            return (abs(L.second - L.first) < abs(R.second - R.first))
                || min(L.first, L.second) < min(R.first, R.second)
                || L.first < R.first; };
        set<pair<int, int>, decltype(custom_cmp)> set_IndiceConnections(custom_cmp);
        for (auto a_p : map_A_I) {
            if(map_B_I.find(a_p.first) == map_B_I.end()) {
                continue;
            } auto& a_v = a_p.second;
            auto& b_v = map_B_I[a_p.first];
            int a_i = 0, b_i = 0;
            printf("Check for: %d\n", a_p.first);
#if defined(DEBUG)            
            int terminate_check = 1000;
            while(a_i < a_v.size() && b_i < b_v.size()) {
                if(--terminate_check <= 0) {
                    cout << "possible infinite loop" << endl;
                    break;
                } printf("Indices: (%d,%d){%d, %d}\n", a_v[a_i], b_v[b_i], a_i, b_i);
#else
            while(a_i < a_v.size() && b_i < b_v.size()) {
#endif
                set_IndiceConnections.emplace(pair<int,int>(a_v[a_i], b_v[b_i]));
                bool inc_a = (b_i + 1 >= b_v.size() ||
                        (a_i + 1 < a_v.size() && b_v[b_i] >= a_v[a_i]));
                bool inc_b = (a_i + 1 >= a_v.size() ||
                        (b_i + 1 < b_v.size() && a_v[a_i] >= b_v[b_i]));
                if(inc_a || inc_b){
                    a_i += inc_a;
                    b_i += inc_b;
                    printf("new indices: {%d,%d}\n", a_i, b_i);
                    continue;
                } break;
            }
        }
#if defined(DEBUG)
        printf("myMaxUncrossedLines] set_IndiceConnections:\n");
        pc = 0;
        for(auto ic_p : set_IndiceConnections) {
            printf("(%d,%d)%s", ic_p.first, ic_p.second,
                   (++pc % 10 ? " " : "\n"));
        } cout << endl; 
#endif
        // Insert from the now sorted set (by interval size)
        // into an interval collection, checking to see if there is a collision
        // If there is, do not insert
        vector<pair<int,int>> using_intervals;
        for(auto s : set_IndiceConnections) {
            InsertionHelper(s, using_intervals);
        } return using_intervals.size();
    }
    // */