
// Copyright 2020 XGSTeam, NaCaTeam
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "f2c.h"

#ifdef __cplusplus
extern "C" {
#endif
/* Common Block Declarations */

int getabscut_(doublereal *);


struct {
    doublereal radius, rmax, rsqmax, r_initial__, xbound;
} _BLNK__;

#define _BLNK__1 _BLNK__

union {
    struct {
	doublereal temsta;
	integer istep, interval;
	doublereal qv, qa, a_fs__, c_fs__, e0_fs__;
	integer m_fs__, nn_fs__;
    } _1;
    struct {
	doublereal temsta;
	integer istep, interval;
	doublereal qv, qa, a, c__, e0;
	integer m, nn;
    } _2;
} x_data__;

#define x_data__1 (x_data__._1)
#define x_data__2 (x_data__._2)

struct {
    logical trace;
} tracecom_;

#define tracecom_1 tracecom_

struct {
    integer natom;
} use_func__;

#define use_func__1 use_func__

/* Table of constant values */

static doublereal c_b2 = .33333333333333331;
static integer c__1 = 1;
static integer c__9 = 9;
static integer c__5 = 5;
static doublereal c_b29 = 10.;
static integer c__3 = 3;
static integer c_b176 = -123457;

/* Subroutine */ int calcu_initial__(integer *n)
{
    /* Format strings */
    static char fmt_100[] = "(1x,\002xbound=\002,f6.2,10x,\002radius=\002,f6"
	    ".2,10x,\002Rmax=\002,f6.2)";

    /* System generated locals */
    doublereal d__1;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void),
	     s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);

    /* Local variables */
    static doublereal v, v_atom__, dimeter;

    /* Fortran I/O blocks */
    static cilist io___4 = { 0, 6, 0, fmt_100, 0 };
    static cilist io___5 = { 0, 6, 0, 0, 0 };
    static cilist io___6 = { 0, 6, 0, 0, 0 };


/* double precision function calcu_initial(n) */
/* Computing 3rd power */
    d__1 = x_data__1.a_fs__;
    v_atom__ = d__1 * (d__1 * d__1) / 4.;
    v = v_atom__;
    d__1 = (doublereal) (*n) * v * 3. / 12.5663704;
    _BLNK__1.radius = pow_dd(&d__1, &c_b2);
    _BLNK__1.rmax = _BLNK__1.radius * 1.1;
/* r_initial=0.5d0*(radius+Rmax) */
    _BLNK__1.r_initial__ = _BLNK__1.radius * 1.3;
/* r_initial is half of xbound. */
    _BLNK__1.rsqmax = _BLNK__1.rmax * _BLNK__1.rmax;
    dimeter = _BLNK__1.radius * 2.;
/* xbound=dimeter*1.1d0 */
/* xbound=dimeter*1.2d0 */
    _BLNK__1.xbound = dimeter * 1.3;
    return 0;
} /* calcu_initial__ */

/* Subroutine */ int zuixi_(integer *n, doublereal *etotm, doublereal *ezuixi,
	 doublereal *xmini, doublereal *xzuixi)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    xzuixi -= 251;
    xmini -= 251;

    /* Function Body */
    *ezuixi = *etotm;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    xzuixi[i__ + j * 250] = xmini[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* zuixi_ */

/* Subroutine */ int fsaveone_(integer *n, doublereal *x, doublereal *ffx, 
	doublereal *xmini, doublereal *ffxmini)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    ffxmini -= 251;
    xmini -= 251;
    ffx -= 251;
    x -= 251;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    ffx[i__ + j * 250] = x[i__ + j * 250];
	    ffxmini[i__ + j * 250] = xmini[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* fsaveone_ */

/* Subroutine */ int fsavetwo_(integer *n, doublereal *x, doublereal *ffx, 
	doublereal *xmini, doublereal *ffxmini)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    ffxmini -= 251;
    xmini -= 251;
    ffx -= 251;
    x -= 251;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    x[i__ + j * 250] = ffx[i__ + j * 250];
	    xmini[i__ + j * 250] = ffxmini[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* fsavetwo_ */

/* subroutine Tran_energy(Itime,n,x,xmini,Emini,idum,runCG) */
/* Subroutine */ int tran_energy__(integer *n, doublereal *x, doublereal *
	xmini, doublereal *emini, integer *idum, logical *runcg)
{
    /* Format strings */
    static char fmt_900[] = "(1x,\002timeSA1(1)= \002,f15.8)";
    static char fmt_901[] = "(1x,\002timeSA2(1)= \002,f15.8)";
    static char fmt_140[] = "(1x,\002Emini=\002,f22.16)";

    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1;

    /* Builtin functions */
    integer s_wsfe(cilist *), do_fio(integer *, char *, ftnlen), e_wsfe(void);
    double pow_dd(doublereal *, doublereal *);
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void);
    double log(doublereal), exp(doublereal), d_mod(doublereal *, doublereal *)
	    ;

    /* Local variables */
    static integer ix_xmini__;
    static doublereal rminimum;
    extern /* Subroutine */ int xtoxmini_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal d__, e, f[62500]	/* was [250][250] */;
    static integer i__, j;
    static doublereal r__, s;
    extern /* Subroutine */ int evenergy1_(integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    static doublereal diff_etot__;
    extern /* Subroutine */ int fsavemini_(integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *);
    static doublereal s1, etot_func__, t1, t2;
    extern /* Subroutine */ int tran_x_rr__(integer *, doublereal *, 
	    doublereal *, doublereal *, integer *), potential_(integer *, 
	    doublereal *, doublereal *);
    static doublereal ff[62500]	/* was [250][250] */, twopotent[62500]	/* 
	    was [250][250] */;
    static integer kk, it;
    static doublereal fx[750]	/* was [250][3] */, pp[750], rr[62500]	/* 
	    was [250][250] */;
    static integer n_3__;
    static doublereal ftwopotent[62500]	/* was [250][250] */, pqa, tem, frr[
	    62500]	/* was [250][250] */;
    extern doublereal ran2_(integer *);
    static doublereal pqa1;
    extern doublereal func_(doublereal *);
    static doublereal ftol, xmem[750]	/* was [250][3] */, epot, etot;
    static integer ixxx;
    static doublereal epot0, etot0, timcg[3], temqa;
    static integer itdev, jcoor, itnew;
    static doublereal etotm;
    extern /* Subroutine */ int xtopp_(integer *, doublereal *, doublereal *);
    static doublereal delta1, delta2;
    extern /* Subroutine */ int fsave1_(integer *, integer *, doublereal *, 
	    doublereal *, doublereal *, doublereal *, doublereal *, 
	    doublereal *, doublereal *, doublereal *), fsave2_(integer *, 
	    integer *, doublereal *, doublereal *, doublereal *, doublereal *,
	     doublereal *, doublereal *, doublereal *, doublereal *);
    static doublereal timsa1[3], timsa2[3], timecg, record[60], timesa;
    extern doublereal visita_(doublereal *, doublereal *, integer *);
    extern twobden_(integer *, doublereal *, doublereal *, doublereal *);
    static doublereal density[250];

    /* Fortran I/O blocks */
    static cilist io___14 = { 0, 6, 0, fmt_900, 0 };
    static cilist io___28 = { 0, 6, 0, 0, 0 };
    static cilist io___32 = { 0, 6, 0, 0, 0 };
    static cilist io___33 = { 0, 6, 0, 0, 0 };
    static cilist io___34 = { 0, 6, 0, 0, 0 };
    static cilist io___35 = { 0, 6, 0, 0, 0 };
    static cilist io___36 = { 0, 6, 0, 0, 0 };
    static cilist io___57 = { 0, 6, 0, 0, 0 };
    static cilist io___61 = { 0, 6, 0, 0, 0 };
    static cilist io___63 = { 0, 6, 0, fmt_901, 0 };
    static cilist io___65 = { 0, 6, 0, 0, 0 };
    static cilist io___66 = { 0, 6, 0, 0, 0 };
    static cilist io___67 = { 0, 6, 0, 0, 0 };
    static cilist io___72 = { 0, 6, 0, 0, 0 };
    static cilist io___73 = { 0, 6, 0, fmt_140, 0 };
    static cilist io___74 = { 0, 6, 0, 0, 0 };
    static cilist io___77 = { 0, 6, 0, 0, 0 };


    /* Parameter adjustments */
    xmini -= 251;
    x -= 251;

    /* Function Body */
    getabscut_(timsa1);
/* idum=-91103477 !call of Tran_energy has a different idum. */
    kk = 0;
    d__1 = 8. / (doublereal) x_data__2.nn;
    rminimum = x_data__2.a / pow_dd(&c_b29, &d__1);
/*        tem_qamin=0.03d0/793.d0 */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    xmem[i__ + j * 250 - 251] = x[i__ + j * 250];
/* L1200: */
	}
/* L1100: */
    }
    tran_x_rr__(n, &x[251], rr, &rminimum, idum);
    twobden_(n, rr, f, density);
    potential_(n, rr, twopotent);
    evenergy1_(n, &etot, &epot, twopotent, density);
    epot0 = epot;
    etot0 = etot;
    use_func__1.natom = *n;
    xtopp_(n, pp, &x[251]);
    etot_func__ = func_(pp);
    diff_etot__ = (d__1 = etot_func__ - etot, abs(d__1));
    if (diff_etot__ > 1e-6) {
	if (tracecom_1.trace) {
	    s_wsle(&io___32);
	    do_lio(&c__9, &c__1, "diff_etot > 1.d-6", (ftnlen)17);
	    e_wsle();
	    s_wsle(&io___33);
	    do_lio(&c__9, &c__1, "idum = ", (ftnlen)7);
	    do_lio(&c__3, &c__1, (char *)&(*idum), (ftnlen)sizeof(integer));
	    e_wsle();
	    s_wsle(&io___34);
	    do_lio(&c__9, &c__1, "x coordinate is:", (ftnlen)16);
	    e_wsle();
	    s_wsle(&io___35);
	    i__1 = *n;
	    for (i__ = 1; i__ <= i__1; ++i__) {
		for (j = 1; j <= 3; ++j) {
		    do_lio(&c__5, &c__1, (char *)&x[i__ + j * 250], (ftnlen)
			    sizeof(doublereal));
		}
	    }
	    e_wsle();
	    s_wsle(&io___36);
	    do_lio(&c__9, &c__1, "etot=", (ftnlen)5);
	    do_lio(&c__5, &c__1, (char *)&etot, (ftnlen)sizeof(doublereal));
	    do_lio(&c__9, &c__1, ", etot_func =", (ftnlen)13);
	    do_lio(&c__5, &c__1, (char *)&etot_func__, (ftnlen)sizeof(
		    doublereal));
	    do_lio(&c__9, &c__1, ", diff_etot:", (ftnlen)12);
	    do_lio(&c__5, &c__1, (char *)&diff_etot__, (ftnlen)sizeof(
		    doublereal));
	    e_wsle();
	}
/* stop 'diff_etot > 1.d-6' */
    }
    fsavemini_(n, &etot, emini, &x[251], &xmini[251]);
/* go to 3000 */
    i__1 = x_data__2.istep;
    for (it = 1; it <= i__1; ++it) {
	itnew = it;
	s1 = (doublereal) itnew;
	s = s1 + 1.;
	t1 = exp((x_data__2.qv - 1.) * log(2.)) - 1.;
	t2 = exp((x_data__2.qv - 1.) * log(s)) - 1.;
	tem = x_data__2.temsta * t1 / t2;
	temqa = tem / (doublereal) itnew;
	i__2 = *n;
	for (i__ = 1; i__ <= i__2; ++i__) {
/* write(*,*)'i= ', i */
L2200:
	    ixxx = 1;
	    fsave1_(&i__, n, &x[251], fx, rr, frr, f, ff, twopotent, 
		    ftwopotent);
/* delta=visita(qv,tem,idum) */
	    for (jcoor = 1; jcoor <= 3; ++jcoor) {
		e = visita_(&x_data__2.qv, &tem, idum) + x[i__ + jcoor * 250];
		if (abs(e) > 1e7) {
		    x[i__ + jcoor * 250] = ran2_(idum) * _BLNK__1.xbound;
		} else {
/* e=delta+x(i,jcoor) */
		    d__ = d_mod(&e, &_BLNK__1.xbound) + _BLNK__1.xbound;
		    x[i__ + jcoor * 250] = d_mod(&d__, &_BLNK__1.xbound);
/* x(i,jcoor)=dmod(dmod(d,xbound)+xbound,xbound) */
		}
/* L60: */
	    }
	    xtopp_(n, pp, &x[251]);
	    etot = func_(pp);
	    if (etot > 1e8) {
		fsave2_(&i__, n, &x[251], fx, rr, frr, f, ff, twopotent, 
			ftwopotent);
		goto L2200;
	    }
	    if (etot < etot0) {
		epot0 = epot;
		etot0 = etot;
		if (etot < *emini) {
		    fsavemini_(n, &etot, emini, &x[251], &xmini[251]);
		}
	    } else {
		r__ = ran2_(idum);
		pqa1 = (x_data__2.qa - 1.) * (etot - etot0) / temqa + 1.;
		if (pqa1 < 0.) {
		    pqa = 0.;
		} else {
		    pqa = exp(log(pqa1) / (1. - x_data__2.qa));
		}
		if (r__ > pqa) {
		    fsave2_(&i__, n, &x[251], fx, rr, frr, f, ff, twopotent, 
			    ftwopotent);
		} else {
		    epot0 = epot;
		    etot0 = etot;
		}
	    }
/* L50: */
	}
	itdev = itnew % x_data__2.interval;
	if (tem < .032) {
	    goto L1000;
	}
	if (itdev == 0) {
	    if (kk <= 3) {
		++kk;
		record[kk - 1] = etot0;
	    }
	    if (kk >= 4) {
		record[0] = record[1];
		record[1] = record[2];
		record[2] = etot0;
		delta1 = (d__1 = record[1] - record[0], abs(d__1));
		delta2 = (d__1 = record[2] - record[0], abs(d__1));
		if (delta1 < 1e-4 && delta2 < 1e-4 && tem < .05) {
		    goto L1000;
		}
	    }
	}
/* L40: */
    }
L1000:
/*       call dostim(ih2,im2,is2,il2) */
    getabscut_(timsa2);
    timesa = timsa2[0] - timsa1[0];
/* L3000: */
    ftol = .001;
    n_3__ = *n * 3;
    use_func__1.natom = *n;
    etotm = *emini;
    ix_xmini__ = 1;
    xtoxmini_(n, &ix_xmini__, &x[251], &xmini[251], xmem);
/* L120: */
/* L110: */
/* L130: */
    getabscut_(timcg);
    timecg = timcg[0] - timsa2[0];
    return 0;
} /* tran_energy__ */

/* Subroutine */ int xtoxmini_(integer *n, integer *ix_xmini__, doublereal *x,
	 doublereal *xmini, doublereal *xmem)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    xmem -= 251;
    xmini -= 251;
    x -= 251;

    /* Function Body */
    if (*ix_xmini__ == 1) {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    for (j = 1; j <= 3; ++j) {
		x[i__ + j * 250] = xmini[i__ + j * 250];
/* L3100: */
	    }
/* L3000: */
	}
    } else {
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    for (j = 1; j <= 3; ++j) {
		x[i__ + j * 250] = xmem[i__ + j * 250];
/* L3300: */
	    }
/* L3200: */
	}
    }
    return 0;
} /* xtoxmini_ */

/* Subroutine */ int fsavemini_(integer *n, doublereal *etot, doublereal *
	emini, doublereal *x, doublereal *xmini)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    xmini -= 251;
    x -= 251;

    /* Function Body */
    *emini = *etot;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    xmini[i__ + j * 250] = x[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* fsavemini_ */

/* Subroutine */ int coordin_(integer *n, integer *idum, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal rsqscale;
    static integer i__, j;
    static doublereal sum;
    extern doublereal ran2_(integer *);

    /* Parameter adjustments */
    x -= 251;

    /* Function Body */
    rsqscale = _BLNK__1.r_initial__ * _BLNK__1.r_initial__;
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
L15:
	sum = 0.;
	for (j = 1; j <= 3; ++j) {
	    x[i__ + j * 250] = (ran2_(idum) * 2. - 1.) * _BLNK__1.r_initial__;
	    sum += x[i__ + j * 250] * x[i__ + j * 250];
/* L20: */
	}
	if (sum > rsqscale) {
	    goto L15;
	}
/* L10: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    x[i__ + j * 250] += _BLNK__1.r_initial__;
/* L50: */
	}
/* L40: */
    }
    return 0;
} /* coordin_ */

/* Subroutine */ int coordin_sg__(integer *n, integer *idum, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    x -= 251;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
/* x(i,j)=x(i,j)+r_initial */
	    x[i__ + j * 250] = x[i__ + j * 250] * 2. * _BLNK__1.r_initial__;
/* L50: */
	}
/* L40: */
    }
    return 0;
} /* coordin_sg__ */

/* Subroutine */ int tran_x_rr__(integer *n, doublereal *x, doublereal *rr, 
	doublereal *rminimum, integer *idum)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Local variables */
    extern /* Subroutine */ int distance_(integer *, doublereal *, doublereal 
	    *);
    static integer i__, j, k;
    extern doublereal ran2_(integer *);
    static doublereal xlanbda;

    /* Parameter adjustments */
    rr -= 251;
    x -= 251;

    /* Function Body */
L200:
    xlanbda = .5;
    distance_(n, &x[251], &rr[251]);
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    if (rr[i__ + j * 250] < *rminimum) {
		for (k = 1; k <= 3; ++k) {
		    x[i__ + k * 250] = ran2_(idum) * 2. * 
			    _BLNK__1.r_initial__;
		    x[j + k * 250] = ran2_(idum) * 2. * _BLNK__1.r_initial__;
/* L50: */
		}
		goto L200;
	    }
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* tran_x_rr__ */

/* Subroutine */ int tran_x_rr_mem__(integer *n, doublereal *x, doublereal *
	rr, doublereal *rminimum)
{
    /* Format strings */
    static char fmt_110[] = "(1x,3f17.9)";

    /* System generated locals */
    integer i__1, i__2, i__3;

    /* Builtin functions */
    integer s_wsle(cilist *), do_lio(integer *, integer *, char *, ftnlen), 
	    e_wsle(void), s_wsfe(cilist *), do_fio(integer *, char *, ftnlen),
	     e_wsfe(void);

    /* Local variables */
    extern /* Subroutine */ int distance_(integer *, doublereal *, doublereal 
	    *);
    static integer i__, j, k, ii, jj;
    extern doublereal ran2_(integer *);
    static doublereal xlanbda;

    /* Fortran I/O blocks */
    static cilist io___95 = { 0, 6, 0, 0, 0 };
    static cilist io___96 = { 0, 6, 0, fmt_110, 0 };
    static cilist io___100 = { 0, 6, 0, 0, 0 };
    static cilist io___101 = { 0, 6, 0, fmt_110, 0 };
    static cilist io___102 = { 0, 6, 0, 0, 0 };
    static cilist io___103 = { 0, 6, 0, fmt_110, 0 };


    /* Parameter adjustments */
    rr -= 251;
    x -= 251;

    /* Function Body */
L200:
    xlanbda = .5;
    distance_(n, &x[251], &rr[251]);
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    if (rr[i__ + j * 250] < *rminimum) {
		if (rr[i__ + j * 250] < 1e-30) {
		    for (k = 1; k <= 3; ++k) {
			x[i__ + k * 250] += (ran2_(&c_b176) * 2. - 1.) * 2.;
		    }
		    goto L200;
		}
		for (k = 1; k <= 3; ++k) {
		    x[i__ + k * 250] += xlanbda * (x[i__ + k * 250] - x[j + k 
			    * 250]);
		    x[j + k * 250] += xlanbda * (x[j + k * 250] - x[i__ + k * 
			    250]);
		}
		goto L200;
	    }
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* tran_x_rr_mem__ */

/* Subroutine */ int fangnewcoord_(integer *n, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j;
    static doublereal sum1, center[3]	/* was [1][3] */;

    /* Parameter adjustments */
    x -= 251;

    /* Function Body */
    for (j = 1; j <= 3; ++j) {
	sum1 = 0.;
	i__1 = *n;
	for (i__ = 1; i__ <= i__1; ++i__) {
	    sum1 += x[i__ + j * 250];
/* L20: */
	}
	center[j - 1] = sum1 / (doublereal) (*n);
/* L10: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    x[i__ + j * 250] -= center[j - 1];
	    x[i__ + j * 250] += _BLNK__1.xbound * .5;
/* L40: */
	}
/* L30: */
    }
    return 0;
} /* fangnewcoord_ */

/* Subroutine */ int distance_(integer *n, doublereal *x, doublereal *rr)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    static doublereal p, s1, s2, s3;

    /* Parameter adjustments */
    rr -= 251;
    x -= 251;

    /* Function Body */
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    s1 = (x[i__ + 250] - x[j + 250]) * (x[i__ + 250] - x[j + 250]);
	    s2 = (x[i__ + 500] - x[j + 500]) * (x[i__ + 500] - x[j + 500]);
	    s3 = (x[i__ + 750] - x[j + 750]) * (x[i__ + 750] - x[j + 750]);
	    p = sqrt(s1 + s2 + s3);
	    rr[i__ + j * 250] = p;
	    rr[j + i__ * 250] = p;
/* L30: */
	}
/* L20: */
    }
    return 0;
} /* distance_ */

/* Subroutine */ int twobden_(integer *n, doublereal *rr, doublereal *f, 
	doublereal *density)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j;
    static doublereal q;
    static integer jj;

    /* Parameter adjustments */
    --density;
    f -= 251;
    rr -= 251;

    /* Function Body */
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    d__1 = x_data__2.a / rr[i__ + j * 250];
	    d__2 = (doublereal) x_data__2.m;
	    f[i__ + j * 250] = pow_dd(&d__1, &d__2);
	    f[j + i__ * 250] = f[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q = 0.;
	i__2 = *n;
	for (jj = 1; jj <= i__2; ++jj) {
	    if (jj == i__) {
		goto L40;
	    }
	    q += f[i__ + jj * 250];
L40:
	    ;
	}
	density[i__] = q;
/* L35: */
    }
    return 0;
} /* twobden_ */

/* Subroutine */ int potential_(integer *n, doublereal *rr, doublereal *
	twopotent)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    twopotent -= 251;
    rr -= 251;

    /* Function Body */
    i__1 = *n - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *n;
	for (j = i__ + 1; j <= i__2; ++j) {
	    d__1 = x_data__2.a / rr[i__ + j * 250];
	    d__2 = (doublereal) x_data__2.nn;
	    twopotent[i__ + j * 250] = x_data__2.e0 * pow_dd(&d__1, &d__2);
	    twopotent[j + i__ * 250] = twopotent[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* potential_ */

/* Subroutine */ int evenergy1_(integer *n, doublereal *etot, doublereal *
	epot, doublereal *twopotent, doublereal *density)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer j;
    static doublereal p;
    static integer ii, jj;
    static doublereal sum, ebed[250], einbed;

    /* Parameter adjustments */
    --density;
    twopotent -= 251;

    /* Function Body */
    sum = 0.;
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	ebed[j - 1] = -x_data__2.e0 * x_data__2.c__ * sqrt(density[j]);
	sum += ebed[j - 1];
/* L5: */
    }
    einbed = sum;
    p = 0.;
    i__1 = *n - 1;
    for (ii = 1; ii <= i__1; ++ii) {
	i__2 = *n;
	for (jj = ii + 1; jj <= i__2; ++jj) {
	    p += twopotent[ii + jj * 250];
/* L30: */
	}
/* L20: */
    }
    *epot = p;
    *etot = einbed + *epot;
    return 0;
} /* evenergy1_ */

/* Subroutine */ int fsave1_(integer *i__, integer *n, doublereal *x, 
	doublereal *fx, doublereal *rr, doublereal *frr, doublereal *f, 
	doublereal *ff, doublereal *twopotent, doublereal *ftwopotent)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer j, k;

    /* Parameter adjustments */
    ftwopotent -= 251;
    twopotent -= 251;
    ff -= 251;
    f -= 251;
    frr -= 251;
    rr -= 251;
    fx -= 251;
    x -= 251;

    /* Function Body */
    for (k = 1; k <= 3; ++k) {
	fx[*i__ + k * 250] = x[*i__ + k * 250];
/* L10: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (j == *i__) {
	    goto L20;
	}
	frr[*i__ + j * 250] = rr[*i__ + j * 250];
	frr[j + *i__ * 250] = rr[j + *i__ * 250];
	ff[*i__ + j * 250] = f[*i__ + j * 250];
	ff[j + *i__ * 250] = f[j + *i__ * 250];
	ftwopotent[*i__ + j * 250] = twopotent[*i__ + j * 250];
	ftwopotent[j + *i__ * 250] = twopotent[j + *i__ * 250];
L20:
	;
    }
    return 0;
} /* fsave1_ */

/* Subroutine */ int fsave2_(integer *i__, integer *n, doublereal *x, 
	doublereal *fx, doublereal *rr, doublereal *frr, doublereal *f, 
	doublereal *ff, doublereal *twopotent, doublereal *ftwopotent)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer j, k;

    /* Parameter adjustments */
    ftwopotent -= 251;
    twopotent -= 251;
    ff -= 251;
    f -= 251;
    frr -= 251;
    rr -= 251;
    fx -= 251;
    x -= 251;

    /* Function Body */
    for (k = 1; k <= 3; ++k) {
	x[*i__ + k * 250] = fx[*i__ + k * 250];
/* L10: */
    }
    i__1 = *n;
    for (j = 1; j <= i__1; ++j) {
	if (j == *i__) {
	    goto L20;
	}
	rr[*i__ + j * 250] = frr[*i__ + j * 250];
	rr[j + *i__ * 250] = frr[j + *i__ * 250];
	f[*i__ + j * 250] = ff[*i__ + j * 250];
	f[j + *i__ * 250] = ff[j + *i__ * 250];
	twopotent[*i__ + j * 250] = ftwopotent[*i__ + j * 250];
	twopotent[j + *i__ * 250] = ftwopotent[j + *i__ * 250];
L20:
	;
    }
    return 0;
} /* fsave2_ */

doublereal visita_(doublereal *q, doublereal *temp, integer *idum)
{
    /* System generated locals */
    doublereal ret_val, d__1;

    /* Builtin functions */
    double asin(doublereal), log(doublereal), exp(doublereal), sqrt(
	    doublereal), sin(doublereal);

    /* Local variables */
    static doublereal x, y, pi, den, fator1, fator2, fator3, fator4, fator5, 
	    fator6;
    extern doublereal gasdev_(integer *), gammln_(doublereal *);
    static doublereal sigmax;

    pi = asin(1.) * 2.;
/*         idum=int(fx) */
    fator1 = exp(log(*temp) / (*q - 1.));
    fator2 = exp((4. - *q) * log(*q - 1.));
    fator3 = exp((2. - *q) * log(2.) / (*q - 1.));
    fator4 = sqrt(pi) * fator1 * fator2 / (fator3 * (3. - *q));
    fator5 = 1. / (*q - 1.) - .5;
    d__1 = 2. - fator5;
    fator6 = pi * (1. - fator5) / sin(pi * (1. - fator5)) / exp(gammln_(&d__1)
	    );
    sigmax = exp(-(*q - 1.) * log(fator6 / fator4) / (3. - *q));
    x = sigmax * gasdev_(idum);
    y = gasdev_(idum);
    den = exp((*q - 1.) * log((abs(y))) / (3. - *q));
    ret_val = x / den;
    return ret_val;
} /* visita_ */

doublereal gasdev_(integer *idum)
{
    /* Initialized data */

    static integer iset = 0;

    /* System generated locals */
    doublereal ret_val, d__1, d__2;

    /* Builtin functions */
    double log(doublereal), sqrt(doublereal);

    /* Local variables */
    static doublereal v1, v2, fac, rsq;
    extern doublereal ran2_(integer *);
    static doublereal gset;

    if (iset == 0) {
L1:
	v1 = ran2_(idum) * 2. - 1.;
	v2 = ran2_(idum) * 2. - 1.;
/* Computing 2nd power */
	d__1 = v1;
/* Computing 2nd power */
	d__2 = v2;
	rsq = d__1 * d__1 + d__2 * d__2;
	if (rsq >= 1. || rsq == 0.) {
	    goto L1;
	}
	fac = sqrt(log(rsq) * -2. / rsq);
	gset = v1 * fac;
	ret_val = v2 * fac;
	iset = 1;
    } else {
	ret_val = gset;
	iset = 0;
    }
    return ret_val;
} /* gasdev_ */

doublereal gammln_(doublereal *xx)
{
    /* Initialized data */

    static doublereal cof[6] = { 76.18009172947146,-86.50532032941677,
	    24.01409824083091,-1.231739572450155,.001208650973866179,
	    -5.395239384953e-6 };
    static doublereal stp = 2.5066282746310005;

    /* System generated locals */
    doublereal ret_val;

    /* Builtin functions */
    double log(doublereal);

    /* Local variables */
    static integer j;
    static doublereal x, y, ser, tmp;

    x = *xx;
    y = x;
    tmp = x + 5.5;
    tmp = (x + .5) * log(tmp) - tmp;
    ser = 1.000000000190015;
    for (j = 1; j <= 6; ++j) {
	y += 1.;
	ser += cof[j - 1] / y;
/* L11: */
    }
    ret_val = tmp + log(stp * ser / x);
    return ret_val;
} /* gammln_ */

doublereal ran2_(integer *idum)
{
    /* Initialized data */

    static integer idum2 = 123456789;
    static integer iv[32] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	    0,0,0,0,0,0,0,0 };
    static integer iy = 0;

    /* System generated locals */
    integer i__1;
    doublereal ret_val, d__1;

    /* Local variables */
    static integer j, k;

    if (*idum <= 0) {
/* Computing MAX */
	i__1 = -(*idum);
	*idum = max(i__1,1);
/* be sure to prevent idum=0 */
	idum2 = *idum;
	for (j = 40; j >= 1; --j) {
/* load the shuffle table (after 8 warm-up */
	    k = *idum / 53668;
	    *idum = (*idum - k * 53668) * 40014 - k * 12211;
	    if (*idum < 0) {
		*idum += 2147483563;
	    }
	    if (j <= 32) {
		iv[j - 1] = *idum;
	    }
/* L11: */
	}
	iy = iv[0];
    }

    k = *idum / 53668;
    *idum = (*idum - k * 53668) * 40014 - k * 12211;


    if (*idum < 0) {
	*idum += 2147483563;
    }
    k = idum2 / 52774;
    idum2 = (idum2 - k * 52774) * 40692 - k * 3791;


    if (idum2 < 0) {
	idum2 += 2147483399;
    }
    j = iy / 67108862 + 1;
/* will be in the range 1:NTAB */
    iy = iv[j - 1] - idum2;


    iv[j - 1] = *idum;
    if (iy < 1) {
	iy += 2147483562;
    }
/* Computing MIN */
    d__1 = iy * 4.6566130573917691e-10;
    ret_val = min(d__1,.99999987999999995);

    return ret_val;
} /* ran2_ */

/* Subroutine */ int xtopp_(integer *n, doublereal *pp, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, number_pp__;

    /* Parameter adjustments */
    x -= 251;
    --pp;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    number_pp__ = (i__ - 1) * 3 + j;
	    pp[number_pp__] = x[i__ + j * 250];
/* L1600: */
	}
/* L1500: */
    }
    return 0;
} /* xtopp_ */

/* Subroutine */ int pptox_(integer *n, doublereal *pp, doublereal *x)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static integer i__, j, number_pp__;

    /* Parameter adjustments */
    x -= 251;
    --pp;

    /* Function Body */
    i__1 = *n;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    number_pp__ = (i__ - 1) * 3 + j;
	    x[i__ + j * 250] = pp[number_pp__];
/* L1600: */
	}
/* L1500: */
    }
    return 0;
} /* pptox_ */

doublereal func_(doublereal *p)
{
    /* System generated locals */
    integer i__1;
    doublereal ret_val;

    /* Local variables */
    static integer i__, j;
    static doublereal fc[750]	/* was [3][250] */, tau[750]	/* was [3][
	    250] */, etot, delta_energy__;
    static integer num_p__;
    static doublereal spring;
    extern /* Subroutine */ int xy_fore__(integer *, doublereal *, doublereal 
	    *, doublereal *);
    static doublereal penalty;

    /* Parameter adjustments */
    --p;

    /* Function Body */
    i__1 = use_func__1.natom;
    for (j = 1; j <= i__1; ++j) {
	for (i__ = 1; i__ <= 3; ++i__) {
	    num_p__ = (j - 1) * 3 + i__;
	    tau[i__ + j * 3 - 4] = p[num_p__];
/* L20: */
	}
/* L10: */
    }
    xy_fore__(&use_func__1.natom, tau, fc, &etot);
    spring = 100.;
    penalty = 0.;
/* do i=1,num_x */
    i__1 = use_func__1.natom;
    for (j = 1; j <= i__1; ++j) {
	for (i__ = 1; i__ <= 3; ++i__) {
	    if (tau[i__ + j * 3 - 4] >= 0. && tau[i__ + j * 3 - 4] <= 
		    _BLNK__1.xbound) {
		delta_energy__ = 0.;
	    } else {
		if (tau[i__ + j * 3 - 4] < 0.) {
		    delta_energy__ = spring * .5 * (tau[i__ + j * 3 - 4] * 
			    tau[i__ + j * 3 - 4]);
		}
		if (tau[i__ + j * 3 - 4] > _BLNK__1.xbound) {
		    delta_energy__ = spring * .5 * (tau[i__ + j * 3 - 4] - 
			    _BLNK__1.xbound) * (tau[i__ + j * 3 - 4] - 
			    _BLNK__1.xbound);
		}
	    }
	    penalty += delta_energy__;
	}
    }
    ret_val = etot + penalty;
    return ret_val;
} /* func_ */

/* Subroutine */ int xy_fore__(integer *natom, doublereal *tau, doublereal *
	fc, doublereal *etot)
{
    /* System generated locals */
    integer i__1;

    /* Local variables */
    static doublereal f[62500]	/* was [250][250] */;
    static integer i__, j;
    static doublereal x[750]	/* was [250][3] */;
    extern /* Subroutine */ int c_twobden__(integer *, doublereal *, 
	    doublereal *, doublereal *);
    static doublereal twopotent[62500]	/* was [250][250] */, rr[62500]	/* 
	    was [250][250] */;
    extern /* Subroutine */ int c_distance__(integer *, doublereal *, 
	    doublereal *), c_evenergy1__(integer *, doublereal *, doublereal *
	    , doublereal *), c_potential__(integer *, doublereal *, 
	    doublereal *);
    static doublereal density[250];

    /* Parameter adjustments */
    fc -= 4;
    tau -= 4;

    /* Function Body */
    i__1 = *natom;
    for (i__ = 1; i__ <= i__1; ++i__) {
	for (j = 1; j <= 3; ++j) {
	    x[i__ + j * 250 - 251] = tau[j + i__ * 3];
/* L20: */
	}
/* L10: */
    }
    c_distance__(natom, x, rr);
    c_twobden__(natom, rr, f, density);
    c_potential__(natom, rr, twopotent);
    c_evenergy1__(natom, etot, twopotent, density);
    return 0;
} /* xy_fore__ */

/* Subroutine */ int c_distance__(integer *natom, doublereal *x, doublereal *
	rr)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer i__, j;
    static doublereal p, s1, s2, s3;

    /* Parameter adjustments */
    rr -= 251;
    x -= 251;

    /* Function Body */
    i__1 = *natom - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *natom;
	for (j = i__ + 1; j <= i__2; ++j) {
	    s1 = (x[i__ + 250] - x[j + 250]) * (x[i__ + 250] - x[j + 250]);
	    s2 = (x[i__ + 500] - x[j + 500]) * (x[i__ + 500] - x[j + 500]);
	    s3 = (x[i__ + 750] - x[j + 750]) * (x[i__ + 750] - x[j + 750]);
	    p = sqrt(s1 + s2 + s3);
	    rr[i__ + j * 250] = p;
	    rr[j + i__ * 250] = p;
/* L30: */
	}
/* L20: */
    }
    return 0;
} /* c_distance__ */

/* Subroutine */ int c_twobden__(integer *natom, doublereal *rr, doublereal *
	f, doublereal *density)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j;
    static doublereal q;
    static integer jj;

    /* Parameter adjustments */
    --density;
    f -= 251;
    rr -= 251;

    /* Function Body */
    i__1 = *natom - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *natom;
	for (j = i__ + 1; j <= i__2; ++j) {
	    d__1 = x_data__2.a / rr[i__ + j * 250];
	    d__2 = (doublereal) x_data__2.m;
	    f[i__ + j * 250] = pow_dd(&d__1, &d__2);
	    f[j + i__ * 250] = f[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    i__1 = *natom;
    for (i__ = 1; i__ <= i__1; ++i__) {
	q = 0.;
	i__2 = *natom;
	for (jj = 1; jj <= i__2; ++jj) {
	    if (jj == i__) {
		goto L40;
	    }
	    q += f[i__ + jj * 250];
L40:
	    ;
	}
	density[i__] = q;
/* L35: */
    }
    return 0;
} /* c_twobden__ */

/* Subroutine */ int c_potential__(integer *natom, doublereal *rr, doublereal 
	*twopotent)
{
    /* System generated locals */
    integer i__1, i__2;
    doublereal d__1, d__2;

    /* Builtin functions */
    double pow_dd(doublereal *, doublereal *);

    /* Local variables */
    static integer i__, j;

    /* Parameter adjustments */
    twopotent -= 251;
    rr -= 251;

    /* Function Body */
    i__1 = *natom - 1;
    for (i__ = 1; i__ <= i__1; ++i__) {
	i__2 = *natom;
	for (j = i__ + 1; j <= i__2; ++j) {
	    d__1 = x_data__2.a / rr[i__ + j * 250];
	    d__2 = (doublereal) x_data__2.nn;
	    twopotent[i__ + j * 250] = x_data__2.e0 * pow_dd(&d__1, &d__2);
	    twopotent[j + i__ * 250] = twopotent[i__ + j * 250];
/* L20: */
	}
/* L10: */
    }
    return 0;
} /* c_potential__ */

/* Subroutine */ int c_evenergy1__(integer *natom, doublereal *etot, 
	doublereal *twopotent, doublereal *density)
{
    /* System generated locals */
    integer i__1, i__2;

    /* Builtin functions */
    double sqrt(doublereal);

    /* Local variables */
    static integer j;
    static doublereal p;
    static integer ii, jj;
    static doublereal sum, ebed, epot, einbed;

    /* Parameter adjustments */
    --density;
    twopotent -= 251;

    /* Function Body */
    sum = 0.;
    i__1 = *natom;
    for (j = 1; j <= i__1; ++j) {
	ebed = -x_data__2.e0 * x_data__2.c__ * sqrt(density[j]);
	sum += ebed;
/* L5: */
    }
    einbed = sum;
    p = 0.;
    i__1 = *natom - 1;
    for (ii = 1; ii <= i__1; ++ii) {
	i__2 = *natom;
	for (jj = ii + 1; jj <= i__2; ++jj) {
	    p += twopotent[ii + jj * 250];
/* L30: */
	}
/* L20: */
    }
    epot = p;
    *etot = einbed + epot;
    return 0;
} /* c_evenergy1__ */

#ifdef __cplusplus
}
#endif