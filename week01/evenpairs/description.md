# Even Pairs

You are part of a team to develop a new kind of pseudorandom number generator (PRNG). To gauge how good your algorithm is at producing random sequences of bits, you are running several different statistical tests.

For example, if $x_0, ..., x_{n-1}$ was a truly random sequence of bits, then it would have the property that the sum $x_i + ... + x_j$ is even for about half of the pairs $0 \leq i \leq j < n$ (and odd for the other half).

To check whether this is the case, if $x_0, ..., x_{n-1}$ are generated by your PRNG, you need to be able to count the number of pairs $0 \leq i \leq j < n$ for which the sum is even.

### Input

The first line of the input contains the number $t \leq 30$ of test cases. Each of the  test cases is described as follows.

- It starts with a line that contains an integer $n$, such that $1 \leq n \leq 5 \cdot 10^4$.

The following line contains $n$ integers $x_0, ..., x_{n-1}$, separated by a space, such that $x_i \in \{0, 1\}$, for all  $i \in \{0, ..., n-1\}$.

### Output

For each test case output a single line containing the number of pairs $0 \leq i \leq j < n$ such that the sum $x_i + ... + x_j$ is even.

### Points

There are three groups of test sets, worth 100 points in total.

1. For the first group of test sets, worth 40 points, you may assume that $1 \leq n \leq 200$.

2. For the second group of test sets, worth 40 points, you may assume that $1 \leq n \leq 5000$.

3. For the third group of test sets, worth 20 points, there are no additional assumptions.