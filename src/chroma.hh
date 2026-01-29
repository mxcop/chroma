#pragma once

/* Source: <https://github.com/aces-aswf/aces-core> */

#include "type.hh"
#include "math.hh"

/* Chromaticities defining a color-space. */
struct Chromaticities {
    f32 rx = 0.0f, ry = 0.0f; /* CIE xy coordinates of red primary. */
    f32 gx = 0.0f, gy = 0.0f; /* CIE xy coordinates of green primary. */
    f32 bx = 0.0f, by = 0.0f; /* CIE xy coordinates of blue primary. */
    f32 wx = 0.0f, wy = 0.0f; /* CIE xy coordinates of white point. */
};

/* Convert xy chromaticity to XYZ. (assuming Y = 1) */
inline v3 xy_to_xyz(f32 x, f32 y) {
    return v3 { x / y, 1.0f, (1.0f - x - y) / y };
}

/* Calculate matrix that transforms linear RGB values to CIE XYZ. */
m3 rgb_to_xyz(const Chromaticities& c) {
    const v3 rp = xy_to_xyz(c.rx, c.ry), gp = xy_to_xyz(c.gx, c.gy);
    const v3 bp = xy_to_xyz(c.bx, c.by), wp = xy_to_xyz(c.wx, c.wy);
    
    /* Primaries matrix (each column is a primary) */
    const m3 primaries_m3 {
        rp.x, rp.y, rp.z,
        gp.x, gp.y, gp.z,
        bp.x, bp.y, bp.z
    };
    
    /* Solve for `Sr * RedXYZ + Sg * GreenXYZ + Sb * BlueXYZ = WhiteXYZ` */
    /* "How much of each primary do we need so that (1, 1, 1) produces the white point?" */
    const v3 s = mul(inverse(primaries_m3), wp);
    
    /* Finally scale each column by `s` */
    return m3 {
        rp.x * s.x, rp.y * s.x, rp.z * s.x, 
        gp.x * s.y, gp.y * s.y, gp.z * s.y, 
        bp.x * s.z, bp.y * s.z, bp.z * s.z, 
    };
}

/* Bradford matrix (XYZ → "cone response" LMS-like space) */
constexpr m3 XYZ_TO_BRADFORD = { 0.8951f, 0.2664f, -0.1614f, -0.7502f, 1.7135f, 0.0367f, 0.0389f, -0.0685f, 1.0296f };
constexpr m3 BRADFORD_TO_XYZ = { 0.9869929f, -0.1470543f, 0.1599627f, 0.4323053f, 0.5183603f, 0.0492912f, -0.0085287f, 0.0400428f, 0.9684867f };

/* Calculate chromatic adaptation matrix. */
m3 chromatic_adaptation(const Chromaticities& src, const Chromaticities& dst) {
    /* Convert white points to cone response */
    const v3 src_lms = mul(XYZ_TO_BRADFORD, v3 { src.wx, 1.0f, src.wy });
    const v3 dst_lms = mul(XYZ_TO_BRADFORD, v3 { dst.wx, 1.0f, dst.wy });

    /* Scaling matrix in lms space */
    const m3 scale = {
        dst_lms.x / src_lms.x, 0.0f, 0.0f,
        0.0f, dst_lms.y / src_lms.y, 0.0f,
        0.0f, 0.0f, dst_lms.z / src_lms.z
    };

    /* Full adaptation: XYZ -> LMS -> scale -> XYZ */
    return mul(BRADFORD_TO_XYZ, mul(scale, XYZ_TO_BRADFORD));
}

/* Calculate a color-space transform matrix. */
m3 cs_transform(const Chromaticities& src, const Chromaticities& dst) {
    /* Combine matrices (from src rgb to CIE xyz) and (from CIE xyz to dst rgb) */
    return mul(inverse(rgb_to_xyz(dst)), mul(chromatic_adaptation(src, dst), rgb_to_xyz(src)));
}
