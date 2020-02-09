//optimal storage on tapes

#include<stdio.h>
struct tape
{	
	int jobno;
	int jobsize;
}tape;
int m=0,arr[10],n;

struct tape tapes[10][10];

void merge(int arr[],int l,int m,int r);
void mrt();

void mergesort(int arr[],int l,int r)
{
	if(l<r)
	{
		int m;
		m=(l+r)/2;
		mergesort(arr,l,m);
		mergesort(arr,m+1,r);
		merge(arr,l,m,r);
	}
}

void merge(int arr[],int l,int m,int r)
{
	int i,j,k;
	i=0;
	j=0;
	k=l;
	int n1=m-l+1;
	int n2=r-m;
	int L[n1],R[n2];
	for(int i=0;i<n1;i++)
		L[i]=arr[l+i];
	for(int j=0;j<n2;j++)
		R[j]=arr[m+1+j];
	while(i<n1 && j<n2)
	{
		if(L[i]<=R[j])
		{
			arr[k]=L[i];
			i++;
		}
		else
		{
			arr[k]=R[j];
			j++;
		}
		k++;
	}
	while(i<n1)
	{
		arr[k]=L[i];
		i++;
		k++;
	}
	while(j<n2)
	{
		arr[k]=R[j];
		j++;
		k++;
	}

}

void store(int m,int n)
{
	int j=0,k,k1;
	k=1;
	k1=0;
	for(int i=0;i<n;i++)
	{
		tapes[k][j].jobno=i;
		tapes[k][j].jobsize=arr[i];
		tapes[0][j].jobno++;
		if(k1==m-1)
		{
			k1=0;
			k++;	
		}
		else
			k1++;
		j=(j+1)%m;
	}
	//printf("%d %d",tapes[0][0].jobno,tapes[0][1].jobno);
}

void print_tapes()
{
	printf("\n\tTapes -");
	for(int i=0;i<m;i++)
	{
		printf("\n\n\n\tTape %d - ",i+1);
		for(int j=1;j<=tapes[0][i].jobno;j++)
			printf("\n\t\t(Job Number -%d || Job Size -%d)",tapes[j][i].jobno,tapes[j][i].jobsize);
	}	
	mrt();
}

void mrt()
{
	float mr[20];
	float sum;
	printf("\n");
	for(int i=0;i<m;i++)
	{
		sum=0;
		mr[i]=0;
		for(int j=1;j<=tapes[0][i].jobno;j++)
		{
			mr[i]+=tapes[j][i].jobsize+sum;
			sum=mr[i];
		}
		mr[i]=mr[i]/(float)tapes[0][i].jobno;
		printf("\n\n\tMean Retreival Time For Tape %d -%f",i+1,mr[i]);	
	}
	sum=0;
	for(int i=0;i<m;i++)
		sum+=mr[i];
	sum=sum/m;	
	printf("\n\n\tFinal Mean Retreival Time All Tapes %f",sum);	
}

int main()
{
	printf("\n\tEnter The Number Of Jobs-");
	scanf("%d",&n);
	printf("\n\tEnter The Number Of Tapes -");
	scanf("%d",&m);
	printf("\n\tEnter The Size Of Programs\n");
	for(int i=0;i<n;i++)
	{
		printf("\n\tEnter Size Of Job[%d] -",i);
		scanf("%d",&arr[i]);
	}
	mergesort(arr,0,n-1);	
	for(int i=0;i<n;i++)
		printf("\t%d",arr[i]);	
	store(m,n);	
	print_tapes();
	printf("\n");
}



