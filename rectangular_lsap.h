/*
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above
   copyright notice, this list of conditions and the following
   disclaimer in the documentation and/or other materials provided
   with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef RECTANGULAR_LSAP_H
#define RECTANGULAR_LSAP_H



#include <stdint.h>

#include <algorithm>
#include <vector>
#include <limits>

template <typename T>
static intptr_t
augmenting_path(intptr_t nc, std::vector<T>& cost, std::vector<T>& u,
    std::vector<T>& v, std::vector<intptr_t>& path,
    std::vector<intptr_t>& row4col,
    std::vector<T>& shortestPathCosts, intptr_t i,
    std::vector<bool>& SR, std::vector<bool>& SC, T* p_minVal)
{
    constexpr T infinity = std::numeric_limits<T>::has_infinity ? std::numeric_limits<T>::infinity() : std::numeric_limits<T>::max();

    T minVal = 0;

    // Crouse's pseudocode uses set complements to keep track of remaining
    // nodes.  Here we use a vector, as it is more efficient in C++.
    intptr_t num_remaining = nc;
    std::vector<intptr_t> remaining(nc);
    for (intptr_t it = 0; it < nc; it++) {
        // Filling this up in reverse order ensures that the solution of a
        // constant cost matrix is the identity matrix (c.f. #11602).
        remaining[it] = nc - it - 1;
    }

    std::fill(SR.begin(), SR.end(), false);
    std::fill(SC.begin(), SC.end(), false);
    std::fill(shortestPathCosts.begin(), shortestPathCosts.end(), infinity);

    // find shortest augmenting path
    intptr_t sink = -1;
    while (sink == -1) {

        intptr_t index = -1;
        T lowest = infinity;
        SR[i] = true;

        for (intptr_t it = 0; it < num_remaining; it++) {
            intptr_t j = remaining[it];

            T r = minVal + cost[i * nc + j] - u[i] - v[j];
            if (r < shortestPathCosts[j]) {
                path[j] = i;
                shortestPathCosts[j] = r;
            }

            // When multiple nodes have the minimum cost, we select one which
            // gives us a new sink node. This is particularly important for
            // integer cost matrices with small co-efficients.
            if (shortestPathCosts[j] < lowest ||
                (shortestPathCosts[j] == lowest && row4col[j] == -1)) {
                lowest = shortestPathCosts[j];
                index = it;
            }
        }

        minVal = lowest;
        intptr_t j = remaining[index];
        if (minVal == infinity) { // infeasible cost matrix
            return -1;
        }

        if (row4col[j] == -1) {
            sink = j;
        }
        else {
            i = row4col[j];
        }

        SC[j] = true;
        remaining[index] = remaining[--num_remaining];
        remaining.resize(num_remaining);
    }

    *p_minVal = minVal;
    return sink;
}

template <typename T>
static int
solve(intptr_t nr, intptr_t nc, T* input_cost, int64_t* output_col4row)
{

    // build a non-negative cost matrix
    std::vector<T> cost(nr * nc);
    T minval = *std::min_element(input_cost, input_cost + nr * nc);
    for (intptr_t i = 0; i < nr * nc; i++) {
        cost[i] = input_cost[i] - minval;
    }

    // initialize variables
    std::vector<T> u(nr, 0);
    std::vector<T> v(nc, 0);
    std::vector<T> shortestPathCosts(nc);
    std::vector<intptr_t> path(nc, -1);
    std::vector<intptr_t> col4row(nr, -1);
    std::vector<intptr_t> row4col(nc, -1);
    std::vector<bool> SR(nr);
    std::vector<bool> SC(nc);

    // iteratively build the solution
    for (intptr_t curRow = 0; curRow < nr; curRow++) {

        T minVal;
        intptr_t sink = augmenting_path(nc, cost, u, v, path, row4col,
            shortestPathCosts, curRow, SR, SC, &minVal);
        if (sink < 0) {
            return -1;
        }

        // update dual variables
        u[curRow] += minVal;
        for (intptr_t i = 0; i < nr; i++) {
            if (SR[i] && i != curRow) {
                u[i] += minVal - shortestPathCosts[col4row[i]];
            }
        }

        for (intptr_t j = 0; j < nc; j++) {
            if (SC[j]) {
                v[j] -= minVal - shortestPathCosts[j];
            }
        }

        // augment previous solution
        intptr_t j = sink;
        while (1) {
            intptr_t i = path[j];
            row4col[j] = i;
            std::swap(col4row[i], j);
            if (i == curRow) {
                break;
            }
        }
    }

    for (intptr_t i = 0; i < nr; i++) {
        output_col4row[i] = col4row[i];
    }

    return 0;
}


template <typename T>
int solve_rectangular_linear_sum_assignment(intptr_t nr, intptr_t nc, T* input_cost,
    int64_t* col4row)
{
    return solve(nr, nc, input_cost, col4row);
}





#endif