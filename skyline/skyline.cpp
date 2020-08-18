#include <iostream>
#include <stdlib.h> // realloc, free, exit ...
#include "skyline.h"

using namespace std;

Skyline::Skyline(int dim){
    m_dim = dim;
    m_u_coeff_size = dim;
    m_l_coeff_size = dim;

    m_u_coeff = new double[dim];
    m_l_coeff = new double[dim];

    m_u_index = new int[dim+1];
    m_l_index = new int[dim+1];

    int i=0;
    for(; i<dim; i++){
        m_u_coeff[i] = 0.;
        m_l_coeff[i] = 0.;
    }

    for(i=0; i<dim+1; i++){
        m_u_index[i] = i;
        m_l_index[i] = i;
    }
    cout << endl;
}

void Skyline::set_upper_coeff(int i, int j, double const &val){
    if(((j-i)>(m_l_index[j+1]-m_l_index[j]))&&(val==0)){
        return;
    }

    if((j-i)<(m_u_index[j+1]-m_u_index[j])){
        m_u_coeff[m_u_index[j]+(j-i)] = val;
    }else{
        int n_to_alloc = (j-i)-(m_u_index[j+1]-m_u_index[j]-1);
        m_u_coeff_size += n_to_alloc;

        double *new_coeff = new double[m_u_coeff_size];

        int k, idx;

        for(k=0; k<j; k++){
            for(idx=m_u_index[k]; idx<m_u_index[k+1]; idx++){
                new_coeff[idx] = m_u_coeff[idx];
            }
        }

        for(idx=m_u_index[j]; idx<m_u_index[j+1]+n_to_alloc; idx++){
            if(idx<m_u_index[j+1]){
                new_coeff[idx] = m_u_coeff[idx];
            }else{
                new_coeff[idx] = 0.;
            }
        }
        new_coeff[idx-1] = val;

        for(k=j+1; k<m_dim+1; k++){
            m_u_index[k] = m_u_index[k] + n_to_alloc;
        }

        for(k=j+1; k<m_dim; k++){
            for(idx=m_u_index[k]; idx<m_u_index[k+1]; idx++){
                new_coeff[idx] = m_u_coeff[idx-n_to_alloc];
            }
        }

        m_u_coeff = new_coeff;
    }
}

void Skyline::set_lower_coeff(int i, int j, double const &val){
    if(((j-i)>(m_l_index[j+1]-m_l_index[j]))&&(val==0)){
        return;
    }

    if((j-i)<(m_l_index[j+1]-m_l_index[j])){
        m_l_coeff[m_l_index[j]+(j-i)] = val;
    }else{
        if(i==j){
            m_l_coeff[m_l_index[j]] = val;
        }else{
            int n_to_alloc = (j-i)-(m_l_index[j+1]-m_l_index[j]-1);
            m_l_coeff_size += n_to_alloc;

            double *new_coeff = new double[m_l_coeff_size];

            int k, idx, d=0;

            for(k=0; k<j; k++){
                for(idx=m_l_index[k]; idx<m_l_index[k+1]; idx++){
                    new_coeff[idx] = m_l_coeff[idx];
                }
            }

            for(idx=m_l_index[j]; idx<m_l_index[j+1]+n_to_alloc; idx++){
                if(idx<m_l_index[j+1]){
                    new_coeff[idx] = m_l_coeff[idx];
                }else{
                    new_coeff[idx] = 0.;
                }
            }
            new_coeff[idx-1] = val;

            for(k=j+1; k<m_dim+1; k++){
                m_l_index[k] = m_l_index[k] + n_to_alloc;
            }

            for(k=j+1; k<m_dim; k++){
                for(idx=m_l_index[k]; idx<m_l_index[k+1]; idx++){
                    new_coeff[idx] = m_l_coeff[idx-n_to_alloc];
                }
            }

            m_l_coeff = new_coeff;
        }
    }
}

void Skyline::set_coeff(int i, int j, double const &val){
    if(i==j){
        m_u_coeff[m_u_index[j]] = val;
        m_l_coeff[m_l_index[j]] = val;
    }else if(j>i){
        (*this).set_upper_coeff(i,j,val);
    }else{
        (*this).set_lower_coeff(j,i,val);
    }
}

double Skyline::get_coeff(int i, int j){
    int* m_index = m_u_index;
    double* m_coeff = m_u_coeff;
    int m_coeff_size = m_u_coeff_size;

    if(i>j){
        int tmp = j;
        j = i;
        i = tmp;
        m_index = m_l_index;
        m_coeff = m_l_coeff;
        m_coeff_size = m_l_coeff_size;
    }

    if((j-i)>=(m_index[j+1]-m_index[j])){
        return 0;
    }else{
        return m_coeff[m_index[j]+(j-i)];
    }
}

void Skyline::factoLU(void){
    int i,j,k;
    double piv;

    for(k=0; k<m_dim; k++){
        for(j=k+1; j<m_dim; j++){
            piv = (*this).get_coeff(k,k);
            if(piv==0){
                cout << "pivot nul" << endl;
            }

            (*this).set_coeff(j,k, (*this).get_coeff(j,k)/piv);
            for(i=k+1; i<m_dim; i++){
                (*this).set_coeff(j,i, (*this).get_coeff(j,i)-(*this).get_coeff(j,k)*(*this).get_coeff(k,i));
            }
        }
    }
}

void Skyline::solve(double* x, double* b){
    double* y = new double[m_dim];

    int i,j;
    for(i=0; i<m_dim; i++){
        y[i] = b[i];
        for(j=0; j<i; j++){
            y[i] -= (*this).get_coeff(i,j)*y[j];
        }
    }

    for(i=m_dim-1; i>=0; i--){
        x[i] = y[i];
        for(j=m_dim-1; j>i; j--){
            x[i] -= (*this).get_coeff(i,j)*x[j];
        }
        x[i] /= (*this).get_coeff(i,i);
    }
}

void Skyline::print(void){
    for(int i=0; i<m_dim; i++){
        for(int j=0; j<m_dim; j++){
            cout << (*this).get_coeff(i,j) << " " ;
        }
        cout << "" << endl;
    }
}
