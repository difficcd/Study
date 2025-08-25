// 16953번 그리디 풀이
#include <iostream> 
using namespace std;

int main(){
  int A, B, cnt=1;
  cin >> A; cin >> B;
  

  while(true){

    if (B<A) {
      cout << "-1";
      break;
    };
    
    
    if (B == A) {
      cout << cnt ;
      break;
    }
    else if(B % 10 == 1) B /= 10, cnt++;
    else if (B % 2 == 0) B /= 2, cnt++;
    else {
      cout << "-1";
      break;
    }
  }

}
