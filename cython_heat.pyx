cdef convert_to_python(double *ptr, int n):
    cdef int i
    lst=[]
    for i in range(n):
        lst.append(ptr[i])
    return lst

cdef extern from "heat.hpp":
    # float cppmult(int int_param, float float_param)
    double u0(float float_param)
    double* uu0(double xmin, double xmax, int N)
    double* solve_heat(double xmin, double xmax, int Nx, double tmax, int Nt)


def pyu0(float_param):
    return u0(float_param )

def pyuu0(xmin, xmax, N):
    return convert_to_python(uu0(xmin, xmax, N), N+1)

def PySolveHeat(xmin, xmax, Nx ,tmax, Nt):
    return convert_to_python(solve_heat(xmin, xmax, Nx, tmax, Nt), Nx+1)
