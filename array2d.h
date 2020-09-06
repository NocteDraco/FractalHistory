#pragma once

#ifndef ARRAY2D_H_INCLUDED
#define ARRAY2D_H_INCLUDED

//====================================
//  Bounds checking option.  Change this #define to one of the following values:
//  0 = never bounds checking
//  1 = bounds checking in Debug builds only (DEBUG or _DEBUG defined)
//  2 = bounds checking always

#define ARRAY2D_BOUNDCHECK_OPTION       1


//====================================
//  v* DON'T MESS WITH THIS *v
//====================================
#if (ARRAY2D_BOUNDCHECK_OPTION < 0) || (ARRAY2D_BOUNDCHECK_OPTION > 2)
#error "Invalid value for ARRAY2D_BOUNDCHECK_OPTION in array2d.h"
#elif (ARRAY2D_BOUNDCHECK_OPTION == 2) || ( (ARRAY2D_BOUNDCHECK_OPTION == 1) && (defined(DEBUG) || defined(_DEBUG)) )
#define ARRAY2D_BOUNDCHECK_PERFORM      1
#else
#define ARRAY2D_BOUNDCHECK_PERFORM      0
#endif
//====================================
//  ^* DON'T MESS WITH THAT *^
//====================================

// need <algorithm> for std::copy
#include <algorithm>

#if ARRAY2D_BOUNDCHECK_PERFORM == 1
#include <stdexcept>
#endif


// the actual class!
template <typename T>
class array2d
{
private:
    T*          p;
    unsigned    w;
    unsigned    h;

public:
    //===============================
    // ctors / dtors
    array2d() : p(0), w(0), h(0) { }

    array2d(const array2d<T>& r) : p(0), w(r.w), h(r.h)
    {
        if(w && h)
        {
            try
            {
                p = new T[w*h];
                std::copy(  r.p,
                            r.p + w*h,
                            p           );
            }catch(...)
            {
                delete[] p;
                throw;
            }
        }
    }

    array2d(unsigned width,unsigned height) : p(0), w(width), h(height)
    {
        if(w && h)
        {
            p = new T[w*h];
        }
    }

    ~array2d()
    {
        delete[] p;
    }

    //================================
    //  assignment
    array2d& operator = (const array2d<T>& r)
    {
        if(this == &r)
            return *this;

        T* n = 0;

        if(r.w && r.h)
        {
            try
            {
                n = new T[r.w * r.h];
                std::copy(  r.p,
                            r.p + r.w*r.h,
                            n                   );
            }catch(...)
            {
                delete[] n;
                throw;
            }
        }

        delete[] p;
        p = n;
        w = r.w;
        h = r.h;
        return *this;
    }

    //================================
    //  info
    inline unsigned GetWidth() const        { return w; }
    inline unsigned GetHeight() const       { return h; }

    //================================
    //  accessing
#if ARRAY2D_BOUNDCHECK_PERFORM == 0
    inline       T& operator () (unsigned x,unsigned y)         { return p[(y*w)+x]; }
    inline const T& operator () (unsigned x,unsigned y) const   { return p[(y*w)+x]; }
#elif ARRAY2D_BOUNDCHECK_PERFORM == 1
    inline       T& operator () (unsigned x,unsigned y)         { AssertBounds(x,y); return p[(y*w)+x]; }
    inline const T& operator () (unsigned x,unsigned y) const   { AssertBounds(x,y); return p[(y*w)+x]; }
#endif

    //===============================
    //  resizing
    void Resize(unsigned width,unsigned height)
    {
        T* n = 0;
        if(width && height)
        {
            try
            {
                n = new T[width * height];
                unsigned copyx = std::min(w,width);
                unsigned copyy = std::min(h,height);

                for(unsigned y = 0; y < copyy; ++y)
                    std::copy(  p + (y*w),
                                p + (y*w) + copyx,
                                n + (y*width)       );
            }catch(...)
            {
                delete[] n;
                throw;
            }
        }
        delete[] p;
        p = n;
        w = width;
        h = height;
    }

    //===============================
    //  bounds checking
private:
#if ARRAY2D_BOUNDCHECK_PERFORM == 1
    inline void AssertBounds(unsigned x,unsigned y) const
    {
        if((x >= w) || (y >= h))
            throw std::out_of_range("Attempting to access out-of-bounds element in array2d class");
    }
#endif
};

#endif // ARRAY2D_H_INCLUDED 