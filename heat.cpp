#include <iostream>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <fstream>
#include "skyline/skyline.h"
#include "heat.hpp"

using namespace std;

#define PI 3.1418

double u0(double const &x){
    if(fabs(x-0.5) < 1./8.){
        return 1.;
    }
    else{
        return 0.;
    }
}

double* uu0(double const &xmin, double const &xmax, int const &N){
    double* res = new double[N+1];
    double dx = (xmax-xmin)/N;

    for(int i=0; i<N+1; i++){
        res[i] = u0(xmin + i*dx);
    }

    return res;
}

// double** uu0(double const &xmin, double const &xmax, int const &N){
//     double** res = new double*[2];
//     res[0] = new double[N];
//     res[1] = new double[N];
//
//     double dx = (xmax-xmin)/N;
//
//     for(int i=0; i<N; i++){
//         res[0][i] = u0(xmin + i*dx);
//         res[1][i] = u0(xmin + i*dx);
//     }
//
//     return res;
// }

double alpha(int const &k)
{
    if(k==0){
        return 1/4.;
    }
    return 2/(k*PI) * (sin(k*PI*5./8.)- sin(k*PI*3./8.));
}

double uN(int const &N, double const &x, double const &t){
    double s = alpha(0);

    for(int j=1; j<N; j++){
        s+= alpha(j)*exp(-(j*j)*PI*PI*t)*cos(j*PI*x);
    }

    return s;
}

double* solve_heat(double const &xmin, double const &xmax, int const &Nx, double const &tmax, int const &Nt){
    Skyline A(Nx+1);

    double dx = (xmax-xmin)/Nx;
    double dt = tmax/Nt;
    double beta = dt/(dx*dx);
    int i;

    for(i=0; i<Nx+1; i++){
        A.set_coeff(i,i, 1+2*beta);
        if(i<Nx){
            A.set_coeff(i+1,i, -beta);
            A.set_coeff(i,i+1, -beta);
        }
    }

    double* u = new double[Nx+1];
    for(i=0; i<Nx+1; i++){
        if(((i*dx)>=3./8.)&&((i*dx)<=5./8.)){
            u[i] = 1.;
        }else{
            u[i] = 0.;
        }
    }

    A.factoLU();

    for(i=0; i<Nt+1; i++){
        cout << "t = " << i*dt << '\r';
        A.solve(u,u);
        u[Nx] = 0.;
        u[0] = 0.;
    }
    cout << "t = " << i*dt << endl;

    return u;
}

// int main(void){
//
//     double xmin = 0.;
//     double xmax = 1.;
//     int Nx = 100;
//     double* u = heat(xmin, xmax, Nx, 0.01, 100);
//
//     ofstream myfile;
//     myfile.open ("init.txt");
//     for(int i=0; i<Nx+1; i++){
//         myfile << i*(xmax-xmin)/Nx << " " << u0(i*(xmax-xmin)/Nx) << endl;
//     }
//     myfile.close();
//
//     myfile.open ("num.txt");
//     for(int i=0; i<Nx+1; i++){
//         myfile << i*(xmax-xmin)/Nx << " " << u[i] << endl;
//     }
//     myfile.close();
//
//     return 0;
// }
