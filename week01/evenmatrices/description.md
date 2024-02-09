# Even Matrices

You are still part of a team to develop a pseudorandom number generator. Your generator has already passed some simple statistical tests, but now it is time to get serious. You arrange a list of bits produced by the generator in an $n$ by $n$ matrix. If $M$ was a truly random matrix of bits, then it would have the property that the sum $\sum_{i'=i_1}^{i_2}\sum_{j'=j_1}^{j_2} x_{i',j'}$ is even for about half of the quadruples $(i_1, i_2, j_1, j_2)$, $1 \leq i_1 \leq i_2 \leq n$ and $1 \leq j_1 \leq j_w \leq n$.

To check whether this is the case for your generator, you need to be able to count the number of such quadruples.

### Input

The first line of the input contains the number $t \leq 15$ of test cases. Each of the $t$ test cases is described as follows.

- It starts with a line that contains an integer $n$ such that $1 \leq n \leq 200$.

- This is followed by $n$ lines, where the $i$-th line contains the integers $x_{i,1}, ..., x_{i,n}$, separated by a space, such that $x_{i,j} \in \{0, 1\}$, for all  $i \in \{1, ..., n\}$ and $j \in \{1, ..., n\}$.

### Output

For each test case output a single line that contains the number of quadruples $(i_1, i_2, j_1, j_2)$ where $1 \leq i_1 \leq i_2 \leq n$ and $1 \leq j_1 \leq j_w \leq n$ and for which the sum $\sum_{i'=i_1}^{i_2}\sum_{j'=j_1}^{j_2} x_{i',j'}$ is even.

### Points

There are three groups of test sets, worth $100$ points in total.

1. For the first group of test sets, worth $40$ points, you may assume that $1 \leq n \leq 10$.

2. For the second group of test sets, worth $40$ points, you may assume that $1 \leq n \leq 50$.

3. For the third group of test set, worth $20$ points, there are no additional assumptions.