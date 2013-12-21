#ifndef __VTL_RANDOM_HPP__
#define __VTL_RANDOM_HPP__

/*************************************************************************\
Microsoft Research Asia
Copyright (c) 2001 Microsoft Corporation

Module Name:
    Random engine

Abstract:
    An managable random engine providing uniform and guassian random number

Notes:
    Inherit from Numeric Recipes 2nd Edition, pp 280. ran1()

Usage:
    Random<class T>;
    Random::Uniform();
    Random::Gaussian();
    Random::FillUniform();
    Random::FillGaussian();

History:
    Created  on 2003 Feb 13 by liyin@ust.hk
    Modified on 2003 Mar 26 by liyin@ust.hk

\*************************************************************************/

#include <ctime>
#include <cmath>
#include <limits>

namespace vtl 
{
    // anonymous private namespace for randome engine
    namespace
    {
        // ------------------------------------------------------------
        // RandomEngine is the basic main algorithm
        // ------------------------------------------------------------

        class RandomEngine
        {
            const static long NTAB = 32;
            const static long IA = 16807;
            const static long IM = 2147483647;
            const static long IQ = 127773;
            const static long IR = 2836;
            const static long NDIV = 1 + (IM - 1) / NTAB;

        public:
            // initial ramdom engine, 
            // zero seed uses time to initialize
            void Initialize(long seed = 0);

            // Inherit from Numeric Recipes 2nd Edition, pp 280. ran1()
            // Uniform Deviates, range [0, 1]
            double Uniform();

            // Inherit from Numeric Recipes 2nd Edition, pp 289. gasdev()
            // Gaussian Random, with mean = 0 and variance = 1
            double Gaussian();

        private:
            // reverse the bit order of a long number, time_t == long
            long bitreverse(long l);

            int iset;
            double gset;

            long idum;
            long iy;
            long iv[NTAB];
        };

        void RandomEngine::Initialize(long seed)
        {
            if(seed == 0)
            {
                seed = bitreverse(static_cast<long>(time( NULL )));
            }

            idum = seed > 0 ? - seed : seed;
            iy = 0;

            // Initialize
            if(-(idum) < 1) idum = 1; // Be sure to prevent idum = 0
            else idum = -(idum);

            for(int j = NTAB + 7; j >= 0; j --)
            {   // Load the shuffle table (after 8 warm-ups).
                long k = (idum) / IQ;
                idum = IA * (idum - k * IQ) - IR * k;
                if(idum < 0) idum += IM;
                if(j < NTAB) iv[j] = idum;
            }
            iy = iv[0];

            iset = 0;
        }

        long RandomEngine::bitreverse(long l)
        {
            long k = 0;
            for(int i = 0; i < NTAB; i ++)
            {
                long b = l & 1;
                l >>= 1;
                k |= b;
                k <<= 1;
            }
            return k;
        }

        double RandomEngine::Uniform()
        {
            // ""minimal" random number gerator of Park and Miller
            // with Bays-Durham shuffle and added safeguards.
            // Returns a nuiform random deviate between 0.0 and 1.0 
            // (exclusive of the endpoint values).
            // Call with idum a negative integer to initialize;
            // Thereafter, do not alter idum between successive deviates in a sequnce. 
            // RNMX should approximate the largest floating value that is less than 1

            // iy == 0 will fall into zero space
            if(iy == 0) Initialize();

            // Compute idum = (IA*idum)%IM without overflows by Schrage's method
            long k = (idum) / IQ;          // Start here when not initializing
            idum = IA * (idum - k * IQ) - IR * k;  
            if(idum < 0) idum += IM;     // Will be in the range 0.. NTAB-1.

            int j = iy / NDIV;    // Out put previously stored value and refill the shuffle table
            iy = iv[j];
            iv[j] = idum;

            // Because users don't expect endpoint value
            double temp = iy / static_cast<double>(IM);
            return __min(temp, 1-1e-7);
        }

        double RandomEngine::Gaussian()
        {
            // Returns a normally distributed deviate with zero mean and unit variance
            // using rand1(idum) as the surce of uniform deviates.

            double fac,rsq, v1,v2;

            if  (iset == 0) 
            {   // We don't have an extra deviate handy, so 
                do 
                {   // Pick two uniform numbers in the square extending from -1 to +1 in each direction,
                    v1 = 2.0 * Uniform() - 1.0;  
                    v2 = 2.0 * Uniform() - 1.0;
                    rsq= v1 * v1 + v2 * v2;    // see if they are in the unit circle,
                } while (rsq >= 1.0 || rsq == 0.0); // and if they are not, try again.

                fac = sqrt(-2.0 * log(rsq) / rsq);

                // Now make the Box-Muller tansformation to get two normal diates.
                // return on and save the other from next time.

                iset = 1;     // Set flag.
                gset = v1 * fac;
                return v2 * fac;
            } 
            else 
            {   // W have an extra deviate handy, so 
                iset = 0; // unset the flag
                return gset;    //  and return it.
            }
        }

    } // endof random egine private namespace

    ///////////////////////////////////////////////////////////////////////
    // Interface class defined for vtl namespace
    ///////////////////////////////////////////////////////////////////////

    template<class T>
        class Random
    {
    public:
        Random (long seed = 0)
        {
            m_Engin.Initialize (seed);
        }

        // Uniform Deviates, range in [minval, maxval];
        T Uniform(T minval, T maxval)
        {
            AdjustMinValue(minval);
            AdjustMaxValue(maxval);
            return static_cast<T>(m_Engin.Uniform() * (maxval - minval) + minval);
        }

        template<class _It>
            void FillUniform(_It begin, _It end, T minval, T maxval)
        {
            AdjustMinValue(minval);
            AdjustMaxValue(maxval);
            const T diff = maxval - minval;
            for(_It it = begin; it != end; ++it)
            {
                *it = static_cast<T>(m_Engin.Uniform() * diff + minval);
            }
        }

        void FillUniform(T* rgDst, size_t size, T minval, T maxval)
        {
            AdjustMinValue(minval);
            AdjustMaxValue(maxval);
            const T diff = maxval - minval;
            for(size_t i = 0; i < size; i ++)
            {
                rgDst[i] = static_cast<T>(m_Engin.Uniform() * diff + minval);
            }
        }

        // Gaussian Random, given mean and variance
        T Gaussian(T mean, T variance)
        {
            return static_cast<T>(m_Engin.Gaussian() * variance + mean);
        }

        template<class _It>
            void FillGaussian(_It begin, _It end, T minval, T maxval)
        {
            for(_It it = begin; it != end; ++it)
            {
                *it = Gaussian(minval, maxval);
            }
        }

        void FillGaussian(T* rgDst, size_t size, T minval, T maxval)
        {
            for(size_t i = 0; i < size; i ++)
            {
                rgDst[i] = Gaussian(minval, maxval);
            }
        }

    private:
        RandomEngine m_Engin;
        void AdjustMinValue(T& minval) const;
        void AdjustMaxValue(T& maxval) const;
    };


    #pragma push_macro("max")
    #pragma push_macro("min")
    #undef max
    #undef min

    // Adjust the min value for uniform randome number
    // signed nagative integer value should decrease one
    template<class T> 
        void Random<T>::AdjustMinValue(T& minval) const
    {
        if(!std::numeric_limits<T>::is_integer) return;
        if(!std::numeric_limits<T>::is_signed) return;
        #pragma warning(push)
        #pragma warning(disable:4146) //unary minus operator applied to unsigned type

        if(minval < 0 && -minval < std::numeric_limits<T>::max())
        {
            minval --;
        }
        #pragma warning(pop)
    }

    // Adjust the min value for uniform randome number
    // positive integer value should increase one
    template<class T> 
        void Random<T>::AdjustMaxValue(T& maxval) const
    {
        if(!std::numeric_limits<T>::is_integer) return;
        if(maxval > 0 && maxval < std::numeric_limits<T>::max())
        {
            maxval ++;
        }
    }

    #pragma pop_macro("max")
    #pragma pop_macro("min")

} // namespace vtl

#endif//__VTL_RANDOM_HPP__
