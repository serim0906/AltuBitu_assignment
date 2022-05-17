#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;
const int INF = 401;
typedef pair<int, int> ci;

pair<int, ci> nextPos(int n, int shark_size, ci& shark, vector<vector<int>>& board) {
    int dr[4] = { -1, 1, 0, 0 };  // 상, 하
    int dc[4] = { 0, 0, -1, 1 };  // 좌, 우

    int min_dist = INF;  // 최소값 401
    queue<ci> q; //상어가 갈 수 있는 곳
    vector<vector<int>> dist(n, vector<int>(n, 0)); //상어의 방문 여부 + 거리
    vector<ci> list; //상어가 먹을 수 있는 물고기들의 위치

    dist[shark.first][shark.second] = 1;
    q.push(shark);  // 상어 위치 큐에 push
    while (!q.empty()) {  // 큐가 비어있지 않을 동안 반복
        int row = q.front().first;  // 아기 상어 행 위치 저장
        int col = q.front().second;  // 아기 상어 열 위치 저장
        q.pop();  // 큐에서 pop

        if (dist[row][col] >= min_dist) { //최단거리 이상은 탐색할 필요 없음
            continue;
        }
        for (int i = 0; i < 4; i++) {  // 상, 하, 좌, 우 4번 반복
            int nr = row + dr[i];  // 새로운 행 
            int nc = col + dc[i];  // 새로운 열
            if (nr < 0 || nr >= n || nc < 0 || nc >= n || dist[nr][nc] || board[nr][nc] > shark_size) {
                // 새로운 행과 열이 0보다 작거나 n보다 클 경우, 이미 방문 했을 경우, 아기 상어 크기보다 클 경우
                continue;
            }

            dist[nr][nc] = dist[row][col] + 1;  // 거리 + 1로 갱신
            if (board[nr][nc] && board[nr][nc] < shark_size) { //먹을 수 있는 물고기 발견
                list.emplace_back(nr, nc);   // 물고기 좌표 삽입
                min_dist = dist[nr][nc];  // 최소 거리 
                continue;
            }
            q.push({ nr, nc });  // 큐에 새로운 행, 열 좌표 push
        }
    }

    if (list.empty()) { //상어가 갈 수 있는 곳이 없음
        return { min_dist, {-1, -1} };  // 최소 거리 출력
    }

    sort(list.begin(), list.end(), [](const ci& p1, const ci& p2) { //정렬 (compare 함수를 정의하지 않아도 됨)
        if (p1.first != p2.first) {  // 최소 거리 다를 경우
            return p1.first < p2.first;
        }
        return p1.second < p2.second;  // 같을 경우
        });
    return { min_dist - 1, list[0] };  // 최소 거리 출력
}

int simulation(int n, pair<int, int>& shark, vector<vector<int>>& board) {
    int ans = 0, size = 2, cnt = 0;  // 물고기 먹을 수 있는 시간, 상어 크기, 먹은 물고기 개수
    while (true) {  // 물고기 먹는 것 반복
        pair<int, ci> result = nextPos(n, size, shark, board);  // 먹을 수 있는 물고기 탐색
        if (result.first == INF) { //더 이상 먹을 수 있는 물고기가 공간에 없음
            break;
        }
        ans += result.first;  // 시간 증가
        cnt++;  // 먹은 물고기 개수 증가
        if (cnt == size) {  //상어 크기 증가(먹은 물고기 수와 상어 크기가 같을 경우)
            cnt = 0;  // 물고기 수 초기화
            size++;
        }

        //상어 이동
        board[shark.first][shark.second] = 0;  // 원래 상어 자리 
        shark = result.second;  // 상어가 물고기 자리로 
    }
    return ans;
}

/**
 * [아기 상어]
 *
 * 1. 상어로부터 가장 가까운 거리에 있는 모든 물고기 탐색 (BFS)
 * 2. 우선순위 조건에 맞추어 먹으러 갈 물고기 확정
 *    탐색하는 방향에 우선순위를 두는 걸로 해결되지 않음! (예제 입력 4) 정렬 필요
 * 3. 상어가 이동할 수 있는 곳이 없을 때까지 BFS 탐색 반복
 *
 * 입력 범위가 작기 때문에 매번 BFS 탐색을 반복해도 시간 초과 X
 * 가능한 물고기의 최대 마리 수 : 399마리
 * 최대 BFS 탐색 횟수 : 399회, 1회 탐색마다 while 문은 최대 400회 미만으로 순회
 * 총 연산 횟수 약 160000번으로 충분히 가능
 *
 * 해설 : https://myunji.tistory.com/378
 * *글 자체는 별로 도움이 안되고...그냥 리팩토링하면 코드가 이렇게 되는구나 정도만 봐주세요
 */

int main() {
    int n;  // 공간의 크기 n
    pair<int, int> shark_pos;  // 아기 상어 위치

    //입력
    cin >> n;
    vector<vector<int>> board(n, vector<int>(n));  // 공간 상태 저장
    for (int i = 0; i < n; i++) {  // 공간의 행 크기 n만큼 반복
        for (int j = 0; j < n; j++) {  // 공간의 열 크기 n만큼 반복
            cin >> board[i][j];  // 공간 상태 입력
            if (board[i][j] == 9) { //상어의 초기 위치
                shark_pos = make_pair(i, j);  // 아기 상어 위치 저장
            }
        }
    }

    //연산 & 출력
    cout << simulation(n, shark_pos, board);
    return 0;
}