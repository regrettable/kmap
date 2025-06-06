#ifndef KMATH_H
#define KMATH_H

#include <math.h>

#define Tau 6.28318530717958647692f

typedef float GLfloat;
typedef int GLint;

typedef struct
{
    GLfloat x, y;
} v2;

typedef struct
{
	GLint x, y;
} iv2;

typedef union
{
	struct
	{
		GLfloat x, y, z;
	};
    struct
    {
        GLfloat r, g, b;
    };
	struct
	{
		v2 xy;
		GLfloat z_;
	};
	struct
	{
		GLfloat x_;
		v2 yz;
	};
} v3;

typedef union
{
    struct
    {
        f32 x, y, z, w;
    };
    struct
    {
        f32 r, g, b, a;
    };
    struct
    {
        v3 xyz;
        f32 Ignored0_;
    };
} v4;

typedef struct
{
    v2 BottomLeft;
    v2 TopRight;
} rect;

internal iv2
IV2(int x, int y)
{
	iv2 Result =
	{
		.x = x,
		.y = y
	};
	return Result;
}

internal f32
Sqrt(f32 a)
{
	return sqrtf(a);
}

internal f32
SafeDiv0(f32 x, f32 y)
{
	f32 Result = 0;
	if(fabsf(y) > .00001f)
	{
		Result = x/y;
	}
	return Result;
}

internal f32
Atan2(f32 y, f32 x)
{
    return atan2f(y, x);
}

internal int
Round(f32 x)
{
	return (int)roundf(x);
}

internal f32
Cos(f32 x)
{
    return cosf(x);
}

internal f32
Sin(f32 x)
{
    return sinf(x);
}

internal f32
Clamp(f32 x, f32 LowerBound, f32 UpperBound)
{
    f32 Result = x;
    if(Result < LowerBound)
    {
        Result = LowerBound;
    }
    if(Result > UpperBound)
    {
        Result = UpperBound;
    }
    return Result;
}

internal f32
Floor(f32 x)
{
    return floorf(x);
}

internal f32
Abs(f32 x)
{
    return fabs(x);
}

internal u32
IAbs(i32 x)
{
    return (x >= 0) ? x : -x;
}

#define Square(x) (x)*(x)

internal f32
Lerp(f32 a, f32 b, f32 t)
{
    f32 Result = (1.f - t)*a + t*b;
    return Result;
}

internal v2
V2(f32 x, f32 y)
{
    v2 Result = {.x = x, .y = y};
    return Result;
}

internal v3
V3(f32 x, f32 y, f32 z)
{
    v3 Result = {.x = x, .y = y, .z = z};
    return Result;
}

internal v4
V4(f32 x, f32 y, f32 z, f32 w)
{
    v4 Result = {.x = x, .y = y, .z = z, .w = w};
    return Result;
}

internal v2
V2Inv(v2 V)
{
    v2 Result = 
    {
        .x = SafeDiv0(1.f, V.x),
        .y = SafeDiv0(1.f, V.y)
    };
    return Result;
}

internal v2
V2TimesScalar(v2 V, f32 s)
{
	v2 Result = V;
	Result.x *= s;
	Result.y *= s;
	return Result;
}

internal v2
V2Add(v2 A, v2 B)
{
	v2 Result =
	{
		.x = A.x + B.x,
		.y = A.y + B.y
	};
	return Result;
}

internal v2
V2Hadamard(v2 A, v2 B)
{
	v2 Result =
	{
		.x = A.x*B.x,
		.y = A.y*B.y
	};
	return Result;
}

internal v2
V2Sub(v2 A, v2 B)
{
	v2 Result =
	{
		.x = A.x - B.x,
		.y = A.y - B.y
	};
	return Result;
}

internal f32
V2Length(v2 V)
{
	f32 Result = Sqrt(V.x*V.x + V.y*V.y);
	return Result;
}

internal v2
V2Neg(v2 V)
{
    v2 Result = 
    {
        .x = -V.x,
        .y = -V.y
    };
    return Result;
}

internal v2
V2Lerp(v2 A, v2 B, f32 t)
{
    v2 Result =
    {
        .x = Lerp(A.x, B.x, t),
        .y = Lerp(A.y, B.y, t)
    };
    return Result;
}

internal iv2
IV2Add(iv2 a, iv2 b)
{
    iv2 Result =
    {
        .x = a.x + b.x,
        .y = a.y + b.y
    };
    return Result;
}

internal v2
IV2ToV2(iv2 a)
{
    v2 Result =
    {
        .x = a.x,
        .y = a.y
    };
    return Result;
}

internal b32
RectanglesIntersect(rect R1, rect R2)
{
    b32 Intersect = 
        !((R1.TopRight.x < R2.BottomLeft.x) || (R1.BottomLeft.x > R2.TopRight.x) ||
          (R1.TopRight.y < R2.BottomLeft.y) || (R1.BottomLeft.y > R2.TopRight.y));
    return Intersect;
}

internal rect
RectangleFromCenterAndHalfDim(v2 Center, v2 HalfDim)
{
    rect Result =
    {
        .BottomLeft = V2Sub(Center, HalfDim),
        .TopRight = V2Add(Center, HalfDim)
    };
    return Result;
}

internal rect
RectangleFromCenterAndDim(v2 Center, v2 Dim)
{
    v2 BottomLeft = V2Sub(Center, V2TimesScalar(Dim, .5f));
    rect Result =
    {
        .BottomLeft = BottomLeft,
        .TopRight = V2Add(BottomLeft, Dim)
    };
    return Result;
}

internal rect
RectangleFromBottomLeftTopRight(v2 BottomLeft, v2 TopRight)
{
    rect Result = 
    {
        .BottomLeft = BottomLeft,
        .TopRight = TopRight
    };
    return Result;
}

internal rect
RectangleFromBottomLeftAndDim(v2 BottomLeft, v2 Dim)
{
    rect Result =
    {
        .BottomLeft = BottomLeft,
        .TopRight = V2Add(BottomLeft, Dim)
    };
    return Result;
}

internal v2
RectangleGetCenter(rect Rectangle)
{
    v2 Result = V2TimesScalar(V2Add(Rectangle.BottomLeft, Rectangle.TopRight), .5f);
    return Result;
}

internal v2
RectangleGetDim(rect Rectangle)
{
    v2 Result =
    {
        Abs(Rectangle.BottomLeft.x - Rectangle.TopRight.x),
        Abs(Rectangle.BottomLeft.y - Rectangle.TopRight.y),
    };
    return Result;
}


#endif // ifndef KMATH_H
