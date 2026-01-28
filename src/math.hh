#pragma once

#include <cmath>

#include "type.hh"

m3 inverse(const m3& m) {
    /* Cofactors */
    const f32 c00 =  (m.m11 * m.m22 - m.m21 * m.m12);
    const f32 c10 = -(m.m01 * m.m22 - m.m21 * m.m02);  
    const f32 c20 =  (m.m01 * m.m12 - m.m11 * m.m02);
    
    const f32 c01 = -(m.m10 * m.m22 - m.m20 * m.m12);  
    const f32 c11 =  (m.m00 * m.m22 - m.m20 * m.m02);
    const f32 c21 = -(m.m00 * m.m12 - m.m10 * m.m02);  
    
    const f32 c02 =  (m.m10 * m.m21 - m.m20 * m.m11);
    const f32 c12 = -(m.m00 * m.m21 - m.m20 * m.m01);  
    const f32 c22 =  (m.m00 * m.m11 - m.m10 * m.m01);
    
    /* Determinant */
    const f32 det = m.m00 * c00 + m.m10 * c10 + m.m20 * c20;
    const f32 inv_det = 1.0f / det;
    
    /* Adjugate / determinant */
    return m3 {
        c00 * inv_det, c01 * inv_det, c02 * inv_det,
        c10 * inv_det, c11 * inv_det, c12 * inv_det,
        c20 * inv_det, c21 * inv_det, c22 * inv_det
    };
}

m3 mul(const m3& a, const m3& b) {
    return m3 {
        /* Column 0 */
        a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20,
        a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20,
        a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20,
        /* Column 1 */
        a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21,
        a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21,
        a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21,
        /* Column 2 */
        a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22,
        a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22,
        a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22
    };
}

v3 mul(const m3& m, const v3& v) {
    return {
        m.m00 * v.x + m.m01 * v.y + m.m02 * v.z,
        m.m10 * v.x + m.m11 * v.y + m.m12 * v.z,
        m.m20 * v.x + m.m21 * v.y + m.m22 * v.z
    };
}
