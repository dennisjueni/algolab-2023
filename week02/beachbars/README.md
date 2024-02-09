## Solution

The solution to the problem is a straightforward Sliding Window approach.
We first sort the beachbars, according to their position along the beach.
Then we initialize 2 pointers $i$ and $j$ (initially pointing to the beach bar on the very left), and start iterating as follows:
If the distance between the beachbar $i$ and $j$ is within the allowed distance, we check if this solution is better than the previous best solution.
If it is better, we remove all previous optimal solutions and add the current solution to the set of optimal solutions. Then we increase the right pointer, since this could lead to an even better solution.
If the current solution is just as good as the previous best solution, we include the current solution to the set of optimal points. Then again, we increase the right pointer, since this could lead to an even better solution.
If the current solution is not within the allowed distance, we increase the left pointer.

Note that we use a set here which in C++ is sorted by default. Therefore we can simply output the optimal solutions one by one.