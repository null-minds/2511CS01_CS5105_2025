// Name: Samyaka
// Roll No.: 2511AI46

#include<bits/stdc++.h>
using namespace std;

// TC: O(n) SC: O(1)
void bruteforce(vector<int> &v)
{
    int a=v[0];
    for(int i=1;i<v.size();i++)
        v[i-1]=v[i];
    v[v.size()-1]=a;
    return;
}

// TC: O(n) SC: O(1)
void optimal(vector<int> &v)
{
    reverse(v.begin()+1,v.end());
    reverse(v.begin(),v.end());
    return;
}
int main()
{
    int n;
    cin>>n;
    vector<int> v(n);
    for(int i=0;i<n;i++)
        cin>>v[i];
    // bruteforce(v);
    optimal(v);
    for(int i=0;i<n;i++)
        cout<<v[i]<<"  ";
    return 0;
}
