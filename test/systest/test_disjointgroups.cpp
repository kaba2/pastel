#include "pastelsystest.h"

#include <pastel/sys/disjointgroups.h>
#include <iostream>
#include <string>

using namespace std;
using namespace Pastel;

namespace
{

	template <typename Type>
	void print(const DisjointGroups<Type>& group)
	{
		typedef DisjointGroups<Type> Disjoint;
		typedef Disjoint::GroupIterator GroupIterator;
		typedef Disjoint::ConstGroupIterator ConstGroupIterator;
		typedef Disjoint::ElementIterator ElementIterator;
		typedef Disjoint::ConstElementIterator ConstElementIterator;

		cout << "Contents (" << group.elements() << " elements in "
			<< group.groups() << " groups):" << endl;

		ConstGroupIterator groupIter = group.groupBegin();
		ConstGroupIterator groupEnd = group.groupEnd();

		integer groupIndex = 0;

		while(groupIter != groupEnd)
		{
			cout << "Group " << groupIndex << ": ";

			ConstElementIterator elementIter = groupIter->begin();
			ConstElementIterator elementEnd = groupIter->end();

			while(elementIter != elementEnd)
			{
				cout << elementIter->data() << ", ";
				++elementIter;
			}
			cout << "end (size " << groupIter->size() << ")." << endl;

			++groupIndex;
			++groupIter;
		}

		cout << endl;
	}

	void testDisjoint()
	{
		typedef DisjointGroups<int> Disjoint;
		typedef Disjoint::GroupIterator GroupIterator;
		typedef Disjoint::ElementIterator ElementIterator;

		Disjoint a;
		GroupIterator aGroup = a.insertGroup();

		print(a);

		GroupIterator bGroup = a.insertGroup();

		print(a);

		a.insert(aGroup, 2);
		a.insert(aGroup, 3);
		a.insert(aGroup, 5);
		a.insert(aGroup, 7);

		print(a);

		a.insert(bGroup, 1);
		a.insert(bGroup, 4);
		a.insert(bGroup, 6);
		a.insert(bGroup, 8);
		a.insert(bGroup, 9);

		print(a);

		GroupIterator cGroup = a.merge(aGroup, bGroup);

		print(a);
	}

	void testDisjointEmpty()
	{
		typedef DisjointGroups<EmptyClass> Disjoint;
		typedef Disjoint::GroupIterator GroupIterator;
		typedef Disjoint::ElementIterator ElementIterator;

		Disjoint a;
		GroupIterator aGroup = a.insertGroup();
		GroupIterator bGroup = a.insertGroup();

		a.insert(aGroup);
		a.insert(aGroup);
		a.insert(aGroup);
		a.insert(aGroup);

		a.insert(bGroup);
		a.insert(bGroup);
		a.insert(bGroup);
		a.insert(bGroup);
		a.insert(bGroup);

		GroupIterator cGroup = a.merge(aGroup, bGroup);
	}

	void testBegin()
	{
		testDisjoint();
		testDisjointEmpty();
	}

	void testAdd()
	{
		sysTestList().add("DisjointGroups", testBegin);
	}

	CallFunction run(testAdd);

}
