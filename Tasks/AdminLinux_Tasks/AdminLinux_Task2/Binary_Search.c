#include<stdio.h>
#include<math.h>

int Find_Indix(int *arr , int s , int target);



int main()
{

int arr[12]={1,2,3,4,5,6,7,8,9,10,11,12};


int result=Find_Indix(arr,12,2);

if(result==-1)
{

printf("the target element is not present in the array");
}else
{

printf("the indix is : %d\n",result);
}









}


int Find_Indix(int *arr , int s , int target)
{

int l=0;
int r=s-1;

int mid;


while(l<=r)
{
	mid=(l+r)/2;

	if(arr[mid]==target)
		return mid;
	else if(arr[mid]<target)
		l=mid+1;
	else
		r=mid-1;

}

return -1;


}
