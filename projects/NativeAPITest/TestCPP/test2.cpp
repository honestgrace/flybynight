#include "pch.h"

class Dog {
public:
	Dog()
	{
		cout << "Created from Nothing" << endl;
	}
	~Dog()
	{
		cout << "Going back to Nothing" << endl;
		throw 20;
	}
	void Bark()
	{
		cout << "Bark" << endl;
	}
};

TEST(TestDog, test1) {
	cout << "Begin" << endl;
	{
		try
		{
			Dog dog1;
			Dog dog2;

			//throw 20;
			dog1.Bark();
			dog2.Bark();
		}
		catch (int e)
		{
			cout << "Exception error happened: " << e << endl;
		}
		//EXPECT_EQ(obj.bar2, 1);
	}
	cout << "End" << endl;
}