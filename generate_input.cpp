#include <bits/stdc++.h>
using namespace std;

int main(){
  srand(2);
  freopen("input3.txt", "w", stdout);
  int n = 100;
  cout << n << '\n';
  for(int i = 0; i < n; i++){
  	int x = rand() % 300;
  	int y = rand() % 300;
  	cout << x << " " << y << "\n"; 
  }
}
