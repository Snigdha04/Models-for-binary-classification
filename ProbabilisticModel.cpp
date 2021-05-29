#include<bits/stdc++.h>
using namespace std;
#define pb push_back
const int N=4;
vector<vector<double> > c0,c1;
vector<double> mean0,mean1;
double tp,tn,fp,fn;
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
		//Storing the training data in c0 and c1 ...
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
		// finding mean
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

    double sw[4][4], sw1[4][4], sw2[4][4];
		// sw is the within variance matrix

		// Initialising the sw's to 0
    for(int i=0;i<4;i++)
		{
	    for(int j=0;j<4;j++)
	    {
				sw[i][j]=0.0;
				sw1[i][j]=0.0;
				sw2[i][j]=0.0;
			}
		}
		// building the within variance matrix
    for(int i=0;i<c0.size();i++)
    {
        vector<double> t = c0[i];
        for(int i=0;i<4;i++)
            t[i] = t[i] - mean0[i];
        double n1 = c0.size();
				// sw =
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                sw1[i][j] += (t[i]*t[j])/(n1);
            }
        }

    }

    for(int i=0;i<c1.size();i++)
    {
        vector<double> t = c1[i];
        for(int i=0;i<4;i++)
            t[i] = t[i] - mean1[i];
        double n2 = c1.size();
        for(int i=0;i<4;i++)
        {
            for(int j=0;j<4;j++)
            {
                sw2[i][j] += (t[i]*t[j])/(n2);
            }
        }

    }
    double N1 = c0.size(),N2 = c1.size(),N;
    N = N1+N2;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            sw[i][j] = (N1/N)*sw1[i][j] + (N2/N)*(sw2[i][j]);
        }
    }
    double inv[4][4],adj[4][4];
    inverse(sw,inv,adj);
    //for(int i=0;i<4;cout<<endl,i++) for(int j=0;j<4;cout<<"  ",j++) cout<<sw[i][j];
    vector<double> w;
    for(int i=0;i<4;i++)
    {
        double cd = 0;
        for(int j=0;j<4;j++)
        {
            cd = cd + inv[i][j]*(mean0[j]-mean1[j]);
        }
        w.pb(cd);
    }
    vector<double> temp;
    for(int i=0;i<4;i++)
    {
        double cd = 0;
        for(int j=0;j<4;j++)
        {
            cd = cd + inv[i][j]*mean0[j];
        }
        temp.pb(cd);
    }
    double w0 = 0,w1 = 0,w2 = 0;
    for(int i=0;i<4;i++)
        w1 = w1 + temp[i]*mean0[i];
    w1/=2.0;
    temp.clear();
    for(int i=0;i<4;i++)
    {
        double cd = 0;
        for(int j=0;j<4;j++)
        {
            cd = cd + inv[i][j]*mean1[j];
        }
        temp.pb(cd);
    }
    for(int i=0;i<4;i++)
        w2 = w2 - temp[i]*mean1[i];
    w2/=2.0;
    w0 = w2+w1 + log(N1/N2);

    // training data starts.....
cout<<w0<<endl;
    read.open("test.txt");
    cout<<"class\tvalues\n";

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
        double dd = w0;
        for(int i=0;i<4;i++) dd+=vv[i]*w[i];
        double h = exp(-dd);
        dd = 1.0/(1.0+h);
        int ty = s[sz-1]-'0';
        if(ty == 0) dd= 1.0 - dd;
        cout<<s[sz-1]<<"\t"<<dd<<"\n";
        if(s[sz-1] == '1' && dd>0.5) tp++;
        if(s[sz-1] == '1' && dd<0.5) fn++;
        if(s[sz-1] == '0' && dd>0.5) fp++;
        if(s[sz-1] == '0' && dd<0.5) tn++;
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
