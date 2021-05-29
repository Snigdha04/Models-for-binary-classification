#include<bits/stdc++.h>
using namespace std;
#define pb push_back
#define mp make_pair
const int N=4;
vector<vector<double> > c0,c1;
vector<double> mean0,mean1;
vector<pair<double,int> > vp;
double tp,tn,fp,fn;
double entropy(double f)
{
    double cl1p=0, cl2p=0, cl1n=0, cl2n=0;
    for(int i=0; i<vp.size(); i++)
    {
        if(vp[i].first > f)
            if(vp[i].second == 1)  cl1p++;
            else cl1n++;
        else
            if(vp[i].second == 1)   cl2p++;
            else        cl2n++;
    }
    double etr1 = cl1p/(cl1p + cl1n),etr2 = cl2p/(cl2p + cl2n),etr3 = cl1n/(cl1p + cl1n),etr4 = cl2n/(cl2p + cl2n);
    etr1 = etr1*log(etr1);
    etr2 = etr2*log(etr2);
    etr3 = etr3*log(etr3);
    etr4 = etr4*log(etr4);
    return -(etr1+etr2+etr3+etr4);
}

void gc(double A[4][4], double temp[4][4], int a, int b, int n)
{
    int i1 = 0, j1 = 0;
    for (int i=0;i<n;i++)
    for (int j=0;j<n;j++)
    if (i != a && j != b)
    {
        temp[i1][j1++] = A[i][j];
        if (j1 == n - 1)
            j1 = 0,++i1;
    }
}


double determinant(double A[4][4], int n)
{
    double sum = 0.0;
    if (n == 1)   return A[0][0];
    double A1[4][4];
    int s = 1;
    for (int f = 0; f < n; s=-1*s,f++)
    {
        gc(A, A1, 0, f, n);
        sum=sum+ s*A[0][f]*determinant(A1, n - 1);
    }
    return sum;
}

void adjoint(double A[4][4],double adj[4][4])
{

    int s = 1;
    double A1[4][4];
    for (int i=0;i<4;i++)
    for (int j=0;j<4;j++)
    {
        gc(A, A1, i, j, 4);
        if((i+j)%2) s = -1;
        else s = 1;
        adj[j][i] = (s)*(determinant(A1, 4-1));
    }
}

void inverse(double A[4][4], double inv[4][4],double adj[4][4])
{

    double det = determinant(A, 4);
    adjoint(A, adj);
    /*
            a inverse if adjoint(a)/determinent(a);
    */
    for (int i=0; i<4; i++)
    for (int j=0; j<4; j++)
        inv[i][j] = adj[i][j]/double(det);
}

int main()
{
    ifstream read;
    read.open("train.txt");
    string s;
    while(read>>s)
    {
        int sz = s.size();
        string temp;
        vector<double> vv;
        for(int i=0;i<s.size()-1;i++)
        {
            if(s[i] == ',')
            {
                double f = atof(temp.c_str());
                temp.clear();
                vv.pb(f);
            }
            else
            {
                temp = temp + s[i];
            }
        }
        if(s[sz-1] == '0') c0.pb(vv);
        else c1.pb(vv);
        vv.clear();
    }
    read.close();
    double f0=0,f1=0,f2=0,f3=0;
    for(int i=0;i<c0.size();i++)
    {
        f0+=c0[i][0];
        f1+=c0[i][1];
        f2+=c0[i][2];
        f3+=c0[i][3];
    }
    double abc = c0.size();
    mean0.pb(f0/abc);
    mean0.pb(f1/abc);
    mean0.pb(f2/abc);
    mean0.pb(f3/abc);
    f1=0;f2=0;f3=0;f0=0;
    for(int i=0;i<c1.size();i++)
    {
        f0+=c1[i][0];
        f1+=c1[i][1];
        f2+=c1[i][2];
        f3+=c1[i][3];
    }
    abc = c1.size();
    mean1.pb(f0/abc);
    mean1.pb(f1/abc);
    mean1.pb(f2/abc);
    mean1.pb(f3/abc);
    double sw[4][4],temp[4][4];
    for(int i=0;i<4;i++)
    for(int j=0;j<4;j++)
    sw[i][j]=temp[i][j]=0.0;
    for(int i=0;i<c0.size();i++)
    {
        vector<double> t = c0[i];
        for(int i=0;i<4;i++)
            t[i] = t[i] - mean0[i];

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                sw[i][j] += t[i]*t[j];
            }
        }

    }

    for(int i=0;i<c1.size();i++)
    {
        vector<double> t = c1[i];
        for(int i=0;i<4;i++)
            t[i] = t[i] - mean1[i];

        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                sw[i][j] += t[i]*t[j];
            }
        }

    }
    //display(sw);
    double adj[4][4];
    double inv[4][4];
    inverse(sw,inv,adj);
    //cout<<"***********************************\n";
    //display(inv);

    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            for(int k=0;k<4;k++)
            {
                temp[i][j]+=sw[i][k]*inv[k][j];
            }
        }
    }
    //cout<<"***********************************\n";
    //display(temp);
    vector<double> wt;
    for(int i=0;i<4;i++)
    {
        double x=0;
        for(int j=0;j<4;j++)
        {
            x = x + inv[i][j]*(mean0[j]-mean1[j]);
        }
        wt.pb(x);
    }

    read.open("train.txt");
    while(read>>s)
    {
        int sz = s.size();
        string temp;
        vector<double> vv;
        for(int i=0;i<s.size()-1;i++)
        {
            if(s[i] == ',')
            {
                double f = atof(temp.c_str());
                temp.clear();
                vv.pb(f);
            }
            else
            {
                temp = temp + s[i];
            }
        }
        int t = s[sz-1]-'0';
        double dd=0;
        for(int i=0;i<4;i++) dd+= wt[i]*vv[i];
        vp.pb(mp(dd,t));
    }
    read.close();
    sort(vp.begin(),vp.end());
    reverse(vp.begin(),vp.end());
    double y0 = 1e6;
    for(int i=0;i<vp.size()-1;i++)
    {
            double f = (vp[i].first + vp[i+1].first)/2.0;
            double fi = entropy(f);
            if(fi < y0) y0 = f;
    }
    cout<<"\nvalue of threshold is:\n "<<y0<<endl;
    read.open("test.txt");
    //cout<<"class\tvalues\n";
    while(read>>s)
    {
        int sz = s.size();
        string temp;
        vector<double> vv;
        for(int i=0;i<s.size()-1;i++)
        {
            if(s[i] == ',')
            {
                double f = atof(temp.c_str());
                temp.clear();
                vv.pb(f);
            }
            else
            {
                temp = temp + s[i];
            }
        }
        double dd=0.0;
        for(int i=0;i<4;i++) dd+=vv[i]*wt[i];
        //cout<<s[sz-1]<<"\t"<<dd<<"\n";
        if(s[sz-1] == '1' && dd<y0) tp++;
        if(s[sz-1] == '1' && dd>y0) fn++;
        if(s[sz-1] == '0' && dd<y0) fp++;
        if(s[sz-1] == '0' && dd>y0) tn++;
        vv.clear();
    }
		cout<<"\n";
    cout<<"precision\t: "<<tp/(tp+fp)<<endl;
    cout<<"recall\t\t: "<<tp/(tp+fn)<<endl;
    cout<<"accuarcy\t: "<<(tp+tn)/(tp+tn+fp+fn)<<endl;
    cout<<"f-measure\t: "<<(2.0*tp)/(2.0*tp+fp+fn)<<endl;
    cout<<tp<<"\t\t\t"<<fn<<"\n";
    cout<<fp<<"\t\t\t"<<tn<<"\n";
    return 0;
}
