
__global void vector_add(
    __global const int* a,
    __global const int* b,
    __global const int* c,
    const int count)
{
    const int idx = get_global_id(0);
    if (idx < count){
        c[i] = a[i] + b[i];
    }
}