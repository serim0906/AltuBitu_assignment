#include <iostream>
#include <queue>
#include <vector>

using namespace std;
typedef pair<int, int> ci;

vector<bool> is_valid;

//최대 힙에서 top 데이터가 is_valid 할 때까지 pop
void deleteMax(priority_queue<ci>& heap) {
    while (!heap.empty() && !is_valid[heap.top().second]) {  // 힙이 비어있지 않고, heap의 두 번째 원소가 is_valid 하면
        heap.pop();  // pop
    }
}

//최소 힙에서 top 데이터가 is_valid 할 때까지 pop
void deleteMin(priority_queue<ci, vector<ci>, greater<>>& heap) {
    while (!heap.empty() && !is_valid[heap.top().second]) {  // 힙이 비어있지 않고, heap의 두 번째 원소가 is_valid 하면
        heap.pop();  // pop
    }
}

/**
 * 우선순위 큐 풀이
 * 최대 힙과 최소 힙을 동시에 관리하면서 각각의 힙에서 삭제된 데이터를 다른 힙에 반영하는 방법
 * -> 각 데이터의 유효성을 체크하는 배열을 만들기
 */
int main() {
    int t, k, n;  // 테스트 케이스 t, 적용할 연산 개수 k, 정수 n
    char cmd;  // 연산을 나타내는 문자

    cin >> t;  // t 입력
    while (t--) {  // t번 반복
        priority_queue<ci> max_heap; //최대 힙
        priority_queue<ci, vector<ci>, greater<>> min_heap; //최소 힙
        cin >> k;  // k 입력
        int idx = 0; //I 연산으로 입력된 숫자의 인덱스
        while (k--) {  // k 번 반복
            //입력
            cin >> cmd >> n;  // 연산을 나타내는 문자와 정수 입력

            //연산
            switch (cmd) {
            case 'I': //두가지 힙 모두에 값을 넣은 뒤, is_valid 벡터에도 추가
                max_heap.push(ci(n, idx));  // 최대 힙에 값 삽입
                min_heap.push(ci(n, idx));  // 최소 힙에 값 삽입
                is_valid.push_back(true);  // is_valid 벡터에 추가
                idx++;  // I 연산으로 입력된 숫자의 인덱스 증가
                break;
            case 'D': //n에 따라 최대 힙 또는 최소 힙에서 데이터 삭제
                if (n == 1) {  // n=1일 경우
                    deleteMax(max_heap); //D연산 전처리
                    if (!max_heap.empty()) { //D연산, 최대힙이 비어있지 않다면
                        is_valid[max_heap.top().second] = false;  //  최대heap의 두 번째 원소가 is_valid 하지 않음
                        max_heap.pop();  // 최대힙에서 가장 큰 값의 원소 삭제
                    }
                }
                if (n == -1) {  // n=-1일 경우
                    deleteMin(min_heap); //D연산 전처리
                    if (!min_heap.empty()) { //D연산, 최소힙이 비어있지 않다면
                        is_valid[min_heap.top().second] = false;  //  최소heap의 두 번째 원소가 is_valid 하지 않음
                        min_heap.pop();  // 최소힙에서 가장 작은 값의 원소 삭제
                    }
                }
                break;
            }
        }
        //마지막으로 각 힙의 top이 valid 하도록 처리
        deleteMax(max_heap);
        deleteMin(min_heap);

        //출력
        if (max_heap.empty()) {  // 최대힙이 비어있다면
            cout << "EMPTY\n";  // "EMPTY" 출력
        }
        else {  // 비어있지 않다면 
            cout << max_heap.top().first << ' ' << min_heap.top().first << '\n';
        }
        is_valid.clear(); //새로운 테스트 케이스 전 is_valid 벡터 초기화
    }
}