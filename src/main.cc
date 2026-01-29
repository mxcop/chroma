#include <cstdlib> /* EXIT_SUCCESS */
#include <cstdio>  /* printf */

#include "chroma.hh"

/* Source: <https://en.wikipedia.org/wiki/Rec._709> */
constexpr Chromaticities SRGB_REC709 { 0.640f, 0.330f, 0.300f, 0.600f, 0.150f, 0.060f, 0.3127f, 0.3290f };
/* Source: <https://en.wikipedia.org/wiki/Rec._2020> */
constexpr Chromaticities SRGB_REC2020 { 0.708f, 0.292f, 0.170f, 0.797f, 0.131f, 0.046f, 0.3127f, 0.3290f };
/* Source: <https://docs.acescentral.com/encodings/acescg/#color-space> */
constexpr Chromaticities ACES_CG { 0.713f, 0.293f, 0.165f, 0.830f, 0.128f, 0.044f, 0.32168f, 0.33767f };
/* Source: <https://docs.acescentral.com/encodings/aces2065-1/#technical-specification> */
constexpr Chromaticities ACES_2065_1 { 0.7347f, 0.2653f, 0.0f, 1.0f, 0.0001f, -0.077f, 0.32168f, 0.33767f };

void log_color(const char* label, const v3& c) {
    printf("%s: r %f, g %f, b %f\n", label, c.x, c.y, c.z);
}

#define COMPACT_PRINTM 1

void printm(const char* label, const m3 m) {
#if COMPACT_PRINTM
    printf("constexpr glm::mat3 %s { %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff, %ff };\n", label,
#else
    printf("constexpr glm::mat3 %s {\n    %ff, %ff, %ff,\n    %ff, %ff, %ff,\n    %ff, %ff, %ff\n};\n", label,
#endif
        m.m00, m.m10, m.m20,
        m.m01, m.m11, m.m21,
        m.m02, m.m12, m.m22
    );
}

int main() {
    const m3 r709_to_acescg = cs_transform(SRGB_REC709, ACES_CG);

    /* Test primary colors */
    log_color("  red", mul(r709_to_acescg, v3 { 1.0f, 0.0f, 0.0f }));
    log_color("green", mul(r709_to_acescg, v3 { 0.0f, 1.0f, 0.0f }));
    log_color(" blue", mul(r709_to_acescg, v3 { 0.0f, 0.0f, 1.0f }));
    
    /* Log color transform matrices */
    printm("R709_TO_ACESCG", cs_transform(SRGB_REC709, ACES_CG));
    printm("ACESCG_TO_R709", cs_transform(ACES_CG, SRGB_REC709));
    printm("ACESCG_TO_ACES2065", cs_transform(ACES_CG, ACES_2065_1));
    printm("ACES2065_TO_ACESCG", cs_transform(ACES_2065_1, ACES_CG));

    printm("XYZ_TO_ACESCG", inverse(rgb_to_xyz(ACES_CG)));    

    return EXIT_SUCCESS;
}
