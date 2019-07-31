#include <stdlib.h>

//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>
//#include <gsl/gsl_eigen.h>
//#include <gsl/gsl_blas.h>
#include <math.h>
#include "stat.h"

namespace GMapping {


// Draw randomly from a zero-mean Gaussian distribution, with standard
// deviation sigma.
// We use the polar form of the Box-Muller transformation, explained here:
//   http://www.taygeta.com/random/gaussian.html
double pf_ran_gaussian(double sigma)
{
  double x1, x2, w;
  double r;

  do
  {
    do { r = drand48(); } while (r == 0.0);
    x1 = 2.0 * r - 1.0;
    do { r = drand48(); } while (r == 0.0);
    x2 = 2.0 * drand48() - 1.0;
    w = x1*x1 + x2*x2;
  } while(w > 1.0 || w==0.0);

  return(sigma * x2 * sqrt(-2.0*log(w)/w));
}

//////////////////////////////////////////////////////////////////////////////////////////////
/* add for windows */
static unsigned long long seed = 1;
void srand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}
//////////////////////////////////////////////////////////////////////////////////////////////


double sampleGaussian(double sigma, unsigned long int S) 
{
  /*
	static gsl_rng * r = NULL;
	if(r==NULL) {
		gsl_rng_env_setup();
		r = gsl_rng_alloc (gsl_rng_default);
	}
        */
	if (S!=0)
        {
		//gsl_rng_set(r, S);
                srand48(S);
        }
	if (sigma==0)
		return 0;
	//return gsl_ran_gaussian (r,sigma);
	return pf_ran_gaussian (sigma);
}

double evalLogGaussian(double sigmaSquare, double delta){
	if (sigmaSquare<=0)
		sigmaSquare=1e-4;
	return -.5*delta*delta/sigmaSquare-.5*log(2*M_PI*sigmaSquare);
}

double Gaussian3::eval(const OrientedPoint& p) const{
	OrientedPoint q=p-mean;
	q.theta=atan2(sin(p.theta-mean.theta),cos(p.theta-mean.theta));
	double v1,v2,v3;
	v1 = covariance.evec[0][0]*q.x+covariance.evec[1][0]*q.y+covariance.evec[2][0]*q.theta;
	v2 = covariance.evec[0][1]*q.x+covariance.evec[1][1]*q.y+covariance.evec[2][1]*q.theta;
	v3 = covariance.evec[0][2]*q.x+covariance.evec[1][2]*q.y+covariance.evec[2][2]*q.theta;
	return evalLogGaussian(covariance.eval[0], v1)+evalLogGaussian(covariance.eval[1], v2)+evalLogGaussian(covariance.eval[2], v3);
}

}// end namespace

