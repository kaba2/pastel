// Description: Testing for Matrix
// DocumentationOf: matrix.h

#include "test/test_init.h"

#include "pastel/math/matrix/matrix_view.h"
#include "pastel/sys/math/divide_and_round_up.h"

#include <algorithm>

using namespace Pastel;

TEST_CASE("transpose (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[3][3] = {
        {1, 4, 7},
        {2, 5, 8}, 
        {3, 6, 9}
    };

    auto a = view(data).transpose();
    auto b = view(correct);

    REQUIRE(a.isContinuous());
    REQUIRE(a.transpose().isContinuous());
    REQUIRE(a.canCopyBySpan(a));
    REQUIRE(a.canCopyBySpan(b.transpose()));
    REQUIRE(!a.canCopyBySpan(b));
    REQUIRE(!a.transpose().canCopyBySpan(b.transpose()));

    REQUIRE(a.equals(b));
}

TEST_CASE("flipx (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[3][3] = {
        {3, 2, 1},
        {6, 5, 4}, 
        {9, 8, 7}
    };

    auto a = view(data).flipx();
    auto b = view(correct);

    REQUIRE(a.isContinuous());
    REQUIRE(a.flipx().isContinuous());
    REQUIRE(a.canCopyBySpan(a));
    REQUIRE(a.canCopyBySpan(b.flipx()));
    REQUIRE(!a.canCopyBySpan(b));
    REQUIRE(!a.flipx().canCopyBySpan(b.flipx()));

    REQUIRE(a.equals(b));
}

TEST_CASE("flipy (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[3][3] = {
        {7, 8, 9},
        {4, 5, 6}, 
        {1, 2, 3}
    };

    auto a = view(data).flipy();
    auto b = view(correct);

    REQUIRE(a.equals(b));
}

TEST_CASE("slicex (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[3][2] = {
        {2, 3},
        {5, 6}, 
        {8, 9}
    };

    auto a = view(data).slicex(1, 3);
    auto b = view(correct);
    auto c = view(data).slicex<1, 3>();

    REQUIRE(!a.isContinuous());
    REQUIRE(!a.canCopyBySpan(a));
    REQUIRE(!a.canCopyBySpan(b));

    REQUIRE(a.equals(b));
    REQUIRE(c.equals(b));
}

TEST_CASE("slicey (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[2][3] = {
        {4, 5, 6}, 
        {7, 8, 9}
    };

    auto a = view(data).slicey(1, 3);
    auto b = view(correct);
    auto c = view(data).slicey<1, 3>();

    REQUIRE(a.isContinuous());
    REQUIRE(a.canCopyBySpan(a));
    REQUIRE(a.canCopyBySpan(b));

    REQUIRE(a.equals(b));
    REQUIRE(c.equals(b));
}

TEST_CASE("sparsex (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[3][2] = {
        {1, 3},
        {4, 6},
        {7, 9}
    };

    auto a = view(data).sparsex(2);
    auto b = view(correct);
    auto c = view(data).sparsex<2>();

    REQUIRE(!a.isContinuous());
    REQUIRE(!a.canCopyBySpan(a));
    REQUIRE(!a.canCopyBySpan(b));

    REQUIRE(a.equals(b));
    REQUIRE(c.equals(b));
}

TEST_CASE("sparsey (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[2][3] = {
        {1, 2, 3},
        {7, 8, 9}
    };

    auto a = view(data).sparsey(2);
    auto b = view(correct);
    auto c = view(data).sparsey<2>();

    REQUIRE(!a.isContinuous());
    REQUIRE(!a.canCopyBySpan(a));
    REQUIRE(!a.canCopyBySpan(b));

    REQUIRE(a.equals(b));
    REQUIRE(c.equals(b));
}

TEST_CASE("repeatColumn (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    {
        int correct[3][4] = {
            {2, 2, 2, 2},
            {5, 5, 5, 5},
            {8, 8, 8, 8}
        };
        auto a = view(data).repeatColumn(1, 4);
        REQUIRE(a.equals(view(correct)));
        REQUIRE(!a.hasUniqueAddresses());
    }
    {
        int correct[3][4] = {
            {3, 3, 3, 3},
            {6, 6, 6, 6},
            {9, 9, 9, 9}
        };
        auto a = view(data).repeatColumn<4>(2);
        REQUIRE(a.equals(view(correct)));
        REQUIRE(!a.hasUniqueAddresses());
    }
    {
        int correct[3][1] = {
            {2},
            {5},
            {8}
        };

        auto a = view(data).repeatColumn(1, 1);
        REQUIRE(a.equals(view(correct)));
        REQUIRE(a.hasUniqueAddresses());
    }
    {
        int correct[3][1] = {
            {3},
            {6},
            {9}
        };

        auto a = view(data).repeatColumn<1>(2);
        REQUIRE(a.equals(view(correct)));
        REQUIRE(a.hasUniqueAddresses());
    }
}

TEST_CASE("repeatRow (MatrixView)")
{
    int data[3][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };

    int correct[4][3] = {
        {4, 5, 6},
        {4, 5, 6},
        {4, 5, 6},
        {4, 5, 6},
    };

    auto a = view(data).repeatRow(1, 4);
    auto b = view(correct);
    auto c = view(data).repeatRow<4>(1);

    REQUIRE(a.equals(b));
    REQUIRE(c.equals(b));
}
