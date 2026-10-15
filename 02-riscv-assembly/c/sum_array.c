/* Compile to RV32 assembly and compare optimisation levels (see README). */
int sum(const int *a, int n)
{
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += a[i];
    }
    return total;
}
