#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
#include <string.h>
#include <iomanip> 

using namespace std;
typedef long long int ll;
#define fill(a,val) memset(a,val,sizeof(a))
#define mp make_pair
#define pb push_back
#define ss second
#define ff first
void fast() { ios_base::sync_with_stdio(false); cin.tie(NULL);cout.tie(NULL); }
#define all(v) v.begin(),v.end()
#define gcd(a,b) __gcd((a), (b))
#define lcm(a,b) ((a)*(b))/gcd((a),(b))


/*
	avg -> stores the final avg value of all different algorithms used
		-> 1st Column : Average Waiting Time
		-> 2nd Column : Average Turnaround Time
		-> Each row for different algortihms used
	inp -> stores the all input value
*/


//Shortest Job first (Non Preemptive) 
int inp[100000][6];
int out[100000][6];
double avg[100000][6];
int flag;
double throughput;
vector<vector<int>> inpv;

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

bool sortcol( const vector<int>& v1, const vector<int>& v2 ) 
{
 	return v1[1] < v2[1];
}

void sortArrival(int num)
{
	sort(inpv.begin(), inpv.end(), sortcol);
}

void FCFS(int num)
{
	int val=inp[0][1];
	for (int i = 0; i < num; ++i)
	{
		if(inp[i][2]==0) val+=(max(0,inp[i][3]-(val-inp[i][1]))+inp[i][4]);
		else val+=(inp[i][2]+inp[i][3]+inp[i][4]);
		out[i][1]=val-inp[i][1];
		out[i][0]=out[i][1]-inp[i][2]-inp[i][4];
	}
	throughput=val;
}

void SJF(int num)
{
	int val=inpv[0][1];
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> v;
	while(!inpv.empty() || v.size()!=0)
	{
		int tem=inpv[0][1];
		while(inpv.size()!=0 && val>=inpv[0][1])
		{
			v.push(mp(inpv[0][2],inpv[0][0]));
			inpv.erase(inpv.begin());
		}
		if(v.size()==0)
		{
			val=inpv[0][1];
			continue;
		}
		val+=v.top().ff;
		// cout<<v.top().ss<<" "<<inp[v.top().ss][0]<<" "<<v.top().ff<<" "<<out[v.top().ss][2]<<"\n";
		if(v.top().ff==0)
		{
			v.pop();
			continue;
		}
		if(out[v.top().ss][2]==1)
		{
			out[v.top().ss][1]=val-inp[v.top().ss][1];
			out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2]-inp[v.top().ss][4];
		}
		else
		{
			out[v.top().ss][2]=1;
			out[v.top().ss][1]=val+inp[v.top().ss][3]-inp[v.top().ss][1];
			out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2];
			vector<int> temp;
			temp.pb(v.top().ss);
			temp.pb(inp[v.top().ss][3]+val);
			temp.pb(inp[v.top().ss][4]);
			inpv.pb(temp);
			sortArrival(inpv.size());
		}
		v.pop();
	}
	throughput=val;
}

void SJFPreemptive(int num)
{
	int val=inpv[0][1];
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> v;
	while(inpv.size()!=0 || v.size()!=0)
	{
		while(inpv.size()!=0 && val>=inpv[0][1])
		{
			v.push(mp(inpv[0][2],inpv[0][0]));
			inpv.erase(inpv.begin());
		}
		if(v.size()==0)
		{
			val=inpv[0][1];
			continue;
		}
		if(inpv.size()!=0 && (val+v.top().ff>inpv[0][1]))
		{
			pair<int,int> temp1=v.top();
			temp1.ff=val+v.top().ff-inpv[0][1];
			v.pop();
			v.push(temp1);
			val=inpv[0][1];
		}
		else
		{
			val+=v.top().ff;
			if(v.top().ff==0)
			{
				v.pop();
				continue;
			}
			if(out[v.top().ss][2]==1)
			{
				out[v.top().ss][1]=val-inp[v.top().ss][1];
				out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2]-inp[v.top().ss][4];
			}
			else
			{
				out[v.top().ss][2]=1;
				out[v.top().ss][1]=val+inp[v.top().ss][3]-inp[v.top().ss][1];
				out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2];
				vector<int> temp;
				temp.pb(v.top().ss);
				temp.pb(inp[v.top().ss][3]+val);
				temp.pb(inp[v.top().ss][4]);
				inpv.pb(temp);
				sortArrival(inpv.size());
			}
			v.pop();
		}	
	}
	throughput=val;
}

void RoundRobin(int num, int q)
{
	int val=inpv[0][1];
	queue<pair<int,int>> v,vtemp;
	while(inpv.size()!=0 || v.size()!=0 || vtemp.size()!=0)
	{
		while(inpv.size()!=0 && val>=inpv[0][1])
		{
			v.push(mp(inpv[0][2],inpv[0][0]));
			inpv.erase(inpv.begin());
		}
		if(v.size()==0 && vtemp.size()!=0)
		{
			v=vtemp;
			while(!vtemp.empty()) vtemp.pop();
			continue;
		}
		if(v.size()==0)
		{
			val=inpv[0][1];
			continue;
		}
		// cout<<v.front().ss<<" "<<inp[v.front().ss][0]<<" "<<v.front().ff<<"\n";
		if(v.front().ff<=q)
		{
			if(v.front().ff==0)
			{
				v.pop();
				continue;
			}
			val+=v.front().ff;
			if(out[v.front().ss][2]==1)
			{
				out[v.front().ss][1]=val-inp[v.front().ss][1];
				out[v.front().ss][0]=out[v.front().ss][1]-inp[v.front().ss][2]-inp[v.front().ss][4];
			}
			else
			{
				out[v.front().ss][2]=1;
				out[v.front().ss][1]=val+inp[v.front().ss][3]-inp[v.front().ss][1];
				out[v.front().ss][0]=out[v.front().ss][1]-inp[v.front().ss][2];
				vector<int> temp;
				temp.pb(v.front().ss);
				temp.pb(inp[v.front().ss][3]+val);
				temp.pb(inp[v.front().ss][4]);
				inpv.pb(temp);
				sortArrival(inpv.size());
			}
			v.pop();
		}
		else
		{
			val+=q;
			pair<int,int> temp=v.front();
			temp.ff-=q;
			v.pop();
			while(inpv.size()!=0 && val>=inpv[0][1])
			{
				v.push(mp(inpv[0][2],inpv[0][0]));
				inpv.erase(inpv.begin());
			}
			vtemp.push(temp);
		}
	}
	throughput=val;
}


void algo(int num)
{
	int val=inpv[0][1];
	int q_sum=0;
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> v,vtemp;
	while(inpv.size()!=0 && val>=inpv[0][1])
	{
		v.push(mp(inpv[0][2],inpv[0][0]));
		q_sum+=inpv[0][2];
		inpv.erase(inpv.begin());
	}
	int q=q_sum/v.size();
	while(inpv.size()!=0 || v.size()!=0 || vtemp.size()!=0)
	{
		while(inpv.size()!=0 && val>=inpv[0][1])
		{
			v.push(mp(inpv[0][2],inpv[0][0]));
			inpv.erase(inpv.begin());
		}
		if(v.size()==0 && vtemp.size()==0)
		{
			val=inpv[0][1];
			q_sum=0;
			while(inpv.size()!=0 && val>=inpv[0][1])
			{
			    v.push(mp(inpv[0][2],inpv[0][0]));
			    q_sum+=inpv[0][2];
			    inpv.erase(inpv.begin());
			}
			q=q_sum/v.size();
		}
		if(v.size()==0)
		{
			// cout<<val<<"\n";
			q_sum=0;
			// cout<<vtemp.size()<<" ";
			while(vtemp.size()!=0)
			{
				v.push(vtemp.top());
				q_sum+=vtemp.top().ff;
				vtemp.pop();
			}
			q=q_sum/v.size();
			// cout<<vtemp.size()<<" "<<q_sum<<" "<<q<<"\n";
		}
		// cout<<val<<" "<<v.top().ff<<" "<<v.top().ss<<" "<<inp[v.top().ss][0]<<" "<<q<<"\n";
		if(v.top().ff<=q)
		{
			// if(v.top().ff==0)
			// {
			// 	v.pop();
			// 	continue;
			// }
			// cout<<v.top().ss<<" "<<inp[v.top().ss][0]<<" "<<v.top().ff<<"\n";
			val+=v.top().ff;
			if(out[v.top().ss][2]==1)
			{
				out[v.top().ss][1]=val-inp[v.top().ss][1];
				out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2]-inp[v.top().ss][4];
			}
			else
			{
				out[v.top().ss][2]=1;
				out[v.top().ss][1]=val+inp[v.top().ss][3]-inp[v.top().ss][1];
				out[v.top().ss][0]=out[v.top().ss][1]-inp[v.top().ss][2];
				if(inp[v.top().ss][4]==0)
				{
					v.pop();
					continue;
				}
				vector<int> temp;
				temp.pb(v.top().ss);
				temp.pb(inp[v.top().ss][3]+val);
				temp.pb(inp[v.top().ss][4]);
				inpv.pb(temp);
				sortArrival(inpv.size());
			}
			v.pop();
		}
		else
		{
			if(flag==1 && v.top().ff>=3*q)
			{
				val+=(2*q);
				pair<int,int> temp1=v.top();
				temp1.ff-=(2*q);
				v.pop();
				vtemp.push(temp1);	
			}
			else
			{
				val+=q;
				pair<int,int> temp1=v.top();
				temp1.ff-=q;
				v.pop();
				vtemp.push(temp1);
			}
		}
	}
	throughput=val;
}


int main()
{
	int q;
	fill(avg,0);
	fill(out,0);
	
	string filename("trial.txt");
    int num;

    cout << "Enter the number of processes:";
    cin >> num;
    cout << endl;

    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
             << filename << "'" << endl;
        return EXIT_FAILURE;
    }
    int i = 0, j = 0;
    int number;
    while (input_file >> number) {
        inp[i][j] = number;
        j++;
        if(j % 5 == 0){
            i++;
            j = 0; 
        }
    }

    for(int i = 0; i < num; i++){
        cout << "PID: " << inp[i][0] << endl;
        cout << "Arrival Time: " << inp[i][1] << endl;
        cout << "Burst_1: " << inp[i][2]<< endl;
        cout << "IO: " << inp[i][3] << endl;
        cout << "Burst_2: " << inp[i][4] << endl;
        cout << endl;
    }
    
    cout << endl;
    input_file.close();
	
	cout<<"\n";
	cout<<"Enter quantum size: ";
	cin>>q;

	char inpu;
	cout<<"\n";
	cout<<"Enter Y to decrease startvation on priority else Enter N: ";
	cin>>inpu;

	if(inpu=='y' || inpu=='Y') flag=1;
	else flag=0;

	cout<<"\n";
	cout<<"\n";
	cout<<"Before Arrange...\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\n";
	for(int i=0; i<num; i++) cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\n";
	
	for(int i=0; i<num; i++) 
		for(int j=0; j<num-i-1; j++) 
			if(inp[j][1] > inp[j+1][1]) for(int k=0; k<5; k++) swap(inp[j][k], inp[j+1][k]);

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);

	//FCFS.........Computation...........start
	sortArrival(num);
	FCFS(num);
	cout<<"\n";
	cout<<"\n";
	cout<<"Final Result... of FCFS\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\tWaiting Time\tTurnaround Time\n";
	for(int i=0; i<num; i++) 
	{
		cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\t\t"<<out[i][0]<<"\t\t"<<out[i][1]<<"\n";
		avg[0][0]+=out[i][0];
		avg[0][1]+=out[i][1];
	}
	avg[0][0]/=num;
	avg[0][1]/=num;
	avg[0][2]=num/throughput;
	cout<<"\n";
	cout<<"Average Waiting Time    :\t"<<avg[0][0]<<"\n";
	cout<<"Average Turnaround Time :\t"<<avg[0][1]<<"\n";
	cout<<"Throughput :\t"<<avg[0][2]<<"\n";
	//FCFS.........Computation...........end

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);


	// SJF.........Computation...........start
	sortArrival(num);
	SJF(num);
	cout<<"\n";
	cout<<"\n";
	cout<<"Final Result... of SJF\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\tWaiting Time\tTurnaround Time\n";
	for(int i=0; i<num; i++) 
	{
		cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\t\t"<<out[i][0]<<"\t\t"<<out[i][1]<<"\n";
		avg[1][0]+=out[i][0];
		avg[1][1]+=out[i][1];
	}
	avg[1][0]/=num;
	avg[1][1]/=num;
	avg[1][2]=num/throughput;
	cout<<"\n";
	cout<<"Average Waiting Time    :\t"<<avg[1][0]<<"\n";
	cout<<"Average Turnaround Time :\t"<<avg[1][1]<<"\n";
	cout<<"Throughput :\t"<<avg[1][2]<<"\n";
	//SJF.........Computation...........end

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);

	//SJF Preemptive.........Computation...........start
    sortArrival(num);
	SJFPreemptive(num);
	cout<<"\n";
	cout<<"\n";
	cout<<"Final Result... of SJF Preemptive\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\tWaiting Time\tTurnaround Time\n";
	for(int i=0; i<num; i++) 
	{
		cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\t\t"<<out[i][0]<<"\t\t"<<out[i][1]<<"\n";
		avg[2][0]+=out[i][0];
		avg[2][1]+=out[i][1];
	}
	avg[2][0]/=num;
	avg[2][1]/=num;
	avg[2][2]=num/throughput;
	cout<<"\n";
	cout<<"Average Waiting Time    :\t"<<avg[2][0]<<"\n";
	cout<<"Average Turnaround Time :\t"<<avg[2][1]<<"\n";
	cout<<"Throughput :\t"<<avg[2][2]<<"\n";
	//SJF Preemptive.........Computation...........end

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);

	// //Round Robin.........Computation...........start
    sortArrival(num);
	RoundRobin(num,q);
	cout<<"\n";
	cout<<"\n";
	cout<<"Final Result... of Round Robin\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\tWaiting Time\tTurnaround Time\n";
	for(int i=0; i<num; i++) 
	{
		cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\t\t"<<out[i][0]<<"\t\t"<<out[i][1]<<"\n";
		avg[3][0]+=out[i][0];
		avg[3][1]+=out[i][1];
	}
	avg[3][0]/=num;
	avg[3][1]/=num;
	avg[3][2]=num/throughput;
	cout<<"\n";
	cout<<"Average Waiting Time    :\t"<<avg[3][0]<<"\n";
	cout<<"Average Turnaround Time :\t"<<avg[3][1]<<"\n";
	cout<<"Throughput :\t"<<avg[3][2]<<"\n";
	// //Round Robin.........Computation...........end

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);

	//Our Algorithm.........Computation...........start
    sortArrival(num);
	algo(num);
	cout<<"\n";
	cout<<"\n";
	cout<<"Final Result... of Our Algorithm\n";
	cout<<"Process ID\tArrival Time\tCPU Burst Time\tI/O Burst Time\tCPU Burst Time\tWaiting Time\tTurnaround Time\n";
	for(int i=0; i<num; i++) 
	{
		cout<<inp[i][0]<<"\t\t"<<inp[i][1]<<"\t\t"<<inp[i][2]<<"\t\t"<<inp[i][3]<<"\t\t"<<inp[i][4]<<"\t\t"<<out[i][0]<<"\t\t"<<out[i][1]<<"\n";
		avg[4][0]+=out[i][0];
		avg[4][1]+=out[i][1];
	}
	avg[4][0]/=num;
	avg[4][1]/=num;
	avg[4][2]=num/throughput;
	cout<<"\n";
	cout<<"Average Waiting Time    :\t"<<avg[4][0]<<"\n";
	cout<<"Average Turnaround Time :\t"<<avg[4][1]<<"\n";
	cout<<"Throughput :\t"<<avg[4][2]<<"\n";
	//Our Algorithm.........Computation...........end

	inpv.clear();
	for(int i=0; i<num; i++)
	{
		vector<int> temp;
		temp.pb(i);
		temp.pb(inp[i][1]);
		temp.pb(inp[i][2]);
		inpv.pb(temp);
	}
	fill(out,0);





	//Final Table........start
	cout<<"\n";
	cout<<"\n";
	cout<<"Comparision Table\n";
	cout<<"\t\t\tFCFS\t\tSJF\t\tSJFPre\t\tRR\t\tOurAlgo\t\tDiff b/w OurAlgo & RR\n";
	cout<<"Avg Waiting Time:\t";
	for (int i = 0; i < 5; ++i) cout<<avg[i][0]<<"\t\t";
	cout<<avg[3][0]-avg[4][0];
	cout<<"\n";
	cout<<"Avg Turnaround Time:\t";
	for (int i = 0; i < 5; ++i) cout<<avg[i][1]<<"\t\t";
	cout<<avg[3][1]-avg[4][1];
	cout<<"\n";
	cout<<"Throughput:\t\t";
	for (int i = 0; i < 5; ++i) cout<<fixed<<setprecision(8)<<avg[i][2]<<"\t";
	cout<<avg[3][2]-avg[4][2];
	cout<<"\n";
	//Final Table........end
}