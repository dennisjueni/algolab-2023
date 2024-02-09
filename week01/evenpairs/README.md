## Solution

The main insight is that we can use prefix sums to solve this problem in linear time. Assume $S_k = \sum_{i=0}^k a_i$. Then we can check if a given sequence starting at index $k_1$ and ending at index $k_2$ is even, by checking if $S_{k_2} - S_{k_1}$ is even. This can only be even if either $S_{k_1}$ and $S_{k_2}$ are both odd or both even.

Therefore, we do not even have to explictitly calculate the prefix sums but can instead keep track of the number of even prefix sums and the number of odd prefix sums. After we have calculated the number of even and odd prefix sums we can calculate how many ways there are to pick 2 indices from the even prefix sums or from the odd prefix sums. This is given by:
$$\binom{\text{Num even}}{2} + \binom{\text{Num odd}}{2}$$