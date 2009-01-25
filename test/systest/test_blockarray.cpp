#include "pastelsystest.h"

#include "pastel/sys/blockarray.h"
#include "pastel/sys/log.h"

#include <iostream>

using namespace Pastel;
using namespace std;

namespace
{

    void testSimple()
    {
        log() << "- simple" << logNewLine;

        BlockArray<integer> a;
        BlockArray<integer> b;
        
        a.swap(b);
        
        BlockArray<integer> c(a);

        c.resize(100);

        a.swap(c);
        
        a.resize(10);
        a.resize(100);

        b = a;

        b.resize(0);
    }

    void testWrite()
    {
        log() << "- write" << logNewLine;
        
        BlockArray<integer> a(100);
        for (integer i = 0;i < 100;++i)
        {
            a[i] = i;
        }

        a.resize(2560);
        for (integer i = 0;i < 2560;++i)
        {
            a[i] = i;
        }
    }

	void testBegin()
	{
		testSimple();
        testWrite();
	}

	void testAdd()
	{
		sysTestList().add("BlockArray", testBegin);
	}

	CallFunction run(testAdd);

}
