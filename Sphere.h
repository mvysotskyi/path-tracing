//
// Created by waylex on 24.03.24.
//

#ifndef PATHTRACER_SPHERE_H
#define PATHTRACER_SPHERE_H

struct Ray { Vec o, d; Ray(Vec o_, Vec d_) : o(o_), d(d_) {} };
enum Refl_t { DIFF, SPEC, REFR };
struct Sphere {
    double rad;
    Vec p, e, c;
    Refl_t refl;
    Sphere(double rad_, Vec p_, Vec e_, Vec c_, Refl_t refl_):
            rad(rad_), p(p_), e(e_), c(c_), refl(refl_) {}
    double intersect(const Ray &r) const {
        Vec op = p-r.o;
        double t, eps=1e-4, b=op.dot(r.d), det=b*b-op.dot(op)+rad*rad;
        if (det<0) return 0; else det=sqrt(det);
        return (t=b-det)>eps ? t : ((t=b+det)>eps ? t : 0);
    }
};
#endif //PATHTRACER_SPHERE_H
