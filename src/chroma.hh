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

/* Calculate a color-space transform matrix. */
m3 cs_transform(const Chromaticities& src, const Chromaticities& dst) {
    return mul(inverse(rgb_to_xyz(dst)), rgb_to_xyz(src));
}
