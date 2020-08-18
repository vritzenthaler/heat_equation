#ifndef SKYLINE_H_INCLUDED
#define SKYLINE_H_INCLUDED

class Skyline
{
public:
    Skyline(int);

    void set_coeff(int, int, double const &);
    void set_upper_coeff(int, int, double const &);
    void set_lower_coeff(int, int, double const &);
    double get_coeff(int, int);
    void factoLU(void);
    void solve(double*, double*);
    void print(void);

private:
    int *m_u_index;
    int *m_l_index;

    double *m_u_coeff;
    double *m_l_coeff;

    int m_dim;
    int m_u_coeff_size;
    int m_l_coeff_size;
};


#endif
