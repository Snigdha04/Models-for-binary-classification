#include<bits/stdc++.h>
using namespace std;
#define pb push_back
const int N=4;
vector<vector<double> > c0,c1;
vector<double> mean0,mean1;
// training data ...
int main(){
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
    vector<double> w(4);
    double dr = 0;
    // setting the initial value of w matrix to be along (u0 - u1)
    for(int i=0;i<4;i++) w[i] =(mean0[i] - mean1[i]);
    /*for(int i=0;i<4;i++) dr = dr + w[i]*w[i];
    dr = sqrt(dr);
    for(int i=0;i<4;i++)
        w[i]/=dr;*/
    vector<vector<double> > vv;
    for(int i=0;i<c0.size();i++)
            vv.pb(c0[i]);
    for(int i=0;i<c1.size();i++)
            vv.pb(c1[i]);
    int p = c0.size();
    double eta = 0.05;          // Learning rate
    for(int i=0;i<4;cout<<w[i]<<"  ",i++);
    cout<<"\n";
    for(int epoch=0; epoch<10; epoch++)
    {
      for(int i=0;i<vv.size();i++)
      {
          vector<double> temp(4);
          for(int j=0;j<vv.size();j++)
          {
              double x = 0;
              for(int k=0;k<4;k++) x = x + vv[j][k]*w[k];
              double dd = 1.0 + exp(-x);
              dd = 1.0/dd;
              double tn;
              if(j<p) tn = 0;
              else tn=1;
              dd = dd-tn;
              for(int k=0;k<4;k++) temp[k]+=dd*vv[j][k];

          }
          dr = 0;
          for(int k=0;k<4;k++)
              w[k] -= eta*temp[k];
      }
    }
    for(int i=0;i<4;cout<<w[i]<<"  ",i++);

    //testing data starts ..
    read.open("test.txt");
    double tp=0, fp=0, tn=0, fn=0;
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
        double dd =0;
        for(int j=0;j<4;j++) dd = dd + (vv[j])*w[j];
        dd = 1.0 + exp(-dd);
        dd = 1.0/dd;

        cout<<dd<<"\t"<<s[sz-1]<<"\n";
        if(s[sz-1] == '1' && dd>0.5) tp++;
        if(s[sz-1] == '1' && dd<0.5) fn++;
        if(s[sz-1] == '0' && dd>0.5) fp++;
        if(s[sz-1] == '0' && dd<0.5) tn++;
        vv.clear();
    }
    cout<<"\n";
    cout<<"n\t\t\t: "<<eta<<endl;
    cout<<"epochs\t\t: 10"<<endl;
    cout<<"precision\t: "<<tp/(tp+fp)<<endl;
    cout<<"recall\t\t: "<<tp/(tp+fn)<<endl;
    cout<<"accuarcy\t: "<<(tp+tn)/(tp+tn+fp+fn)<<endl;
    cout<<"f-measure\t: "<<(2.0*tp)/(2.0*tp+fp+fn)<<endl;
    cout<<tp<<"\t\t\t"<<fn<<"\n";
    cout<<fp<<"\t\t\t"<<tn<<"\n";
    return 0;
}
