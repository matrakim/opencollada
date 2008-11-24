/*
    Copyright (c) 2008 NetAllied Systems GmbH

    This file is part of COLLADAFramework.

    Licensed under the MIT Open Source License, 
    for details please see LICENSE file or the website
    http://www.opensource.org/licenses/mit-license.php
*/

#ifndef __COLLADAFW_BASE_ARRAY_H__
#define __COLLADAFW_BASE_ARRAY_H__

#include "COLLADAFWPrerequisites.h"

namespace COLLADAFW
{

    /** Base class for the storage for a homogenous array of reference values. */
    template < class Type >
    class BaseArrayElement
    {

    private:

        /**
         * The number of values in the array. Required.
         */
        unsigned int mCount;

        /**
         * A text string containing the unique identifier of this element. 
         * This value must be unique within the instance document. Optional.
         */
        String mId; 

        /**
         * xs:NCName The text string name of this element. Optional.
         */
        String mName; 

        /**
         * The arrayValues element stores values of the templates data type 
         * that reference values within the instance document.
         */
        Type* mValues;

        /**
         * The size of the arrayValues array.
         */
        size_t mValuesSize;

    public:

        /** Constructor. */
        BaseArrayElement () 
            : mCount (0) 
            , mValues (0)
            , mValuesSize (0)
        {}

        /** Destructor. */
        virtual ~BaseArrayElement () {}

        /**
        * The number of values in the array. Required.
        */
        const unsigned int getCount () const { return mCount; }
        void setCount ( const unsigned int val ) { mCount = val; }

        /**
        * A text string containing the unique identifier of this element. 
        * This value must be unique within the instance document. Optional.
        */
        const String getId () const { return mId; }
        void setId ( const String val ) { mId = val; }

        /**
        * xs:NCName The text string name of this element. Optional.
        */
        const String getName () const { return mName; }
        void setName ( const String val ) { mName = val; }

        /**
         * The arrayValues element stores values of the templates data type 
         * that reference values within the instance document.
         * @param arrayValuesSize Parameter to get the size of the array.
         * @return const Type* Pointer to the array of the templates datatype.
         */
        const Type* getValues ( size_t& valuesSize ) const 
        { 
            valuesSize = mValuesSize;
            return mValues; 
        }

        /**
         * The arrayValues element stores values of the templates data type 
         * that reference values within the instance document.
         * @param arrayValues Pointer to the array of values.
         * @param arrayValuesSize Size of the array.
         */
        void setValues ( Type* arrayValues, const size_t valuesSize ) 
        { 
            mValuesSize = valuesSize;
            mValues = arrayValues; 
        }

    };

    /**
     * Declares the storage for a homogenous array of floating-point values.
     * The <float_array> element stores the data values for generic use within 
     * the COLLADA schema. The arrays themselves are strongly typed but without 
     * semantics. They simply describe a sequence of floatingpoint values.
     */
    class FloatArrayElement : public BaseArrayElement < float >
    {

    private:

        /**
         * xs:short The number of significant decimal digits of the float 
         * values that can be contained in the array. The default is 6. Optional.
         */
        short digits;

        /**
         * xs:short The largest exponent of the float values that can be 
         * contained in the array. The default is 38. Optional.
         */
         short magnitude;

    public:

        /**
         * Constructor.
         */
        FloatArrayElement () : BaseArrayElement () {};

        /**
         * Destructor.
         */
        virtual ~FloatArrayElement () {};

        /**
        * xs:short The number of significant decimal digits of the float 
        * values that can be contained in the array. The default is 6. Optional.
        */
        const short getDigits () const { return digits; }
        void setDigits ( const short val ) { digits = val; }

        /**
        * xs:short The largest exponent of the float values that can be 
        * contained in the array. The default is 38. Optional.
        */
        const short getMagnitude () const { return magnitude; }
        void setMagnitude ( const short val ) { magnitude = val; }

    };

    /**
    * Declares the storage for a homogenous array of floating-point values.
    * The <double_array> element stores the data values for generic use within 
    * the COLLADA schema. The arrays themselves are strongly typed but without 
    * semantics. They simply describe a sequence of floatingpoint values.
    */
    class DoubleArrayElement : public BaseArrayElement < double >
    {

    private:

        /**
        * xs:short The number of significant decimal digits of the float 
        * values that can be contained in the array. The default is 6. Optional.
        */
        short digits;

        /**
        * xs:short The largest exponent of the float values that can be 
        * contained in the array. The default is 38. Optional.
        */
        short magnitude;

    public:

        /**
        * Constructor.
        */
        DoubleArrayElement () : BaseArrayElement () {};

        /**
        * Destructor.
        */
        virtual ~DoubleArrayElement () {};

        /**
        * xs:short The number of significant decimal digits of the float 
        * values that can be contained in the array. The default is 6. Optional.
        */
        const short getDigits () const { return digits; }
        void setDigits ( const short val ) { digits = val; }

        /**
        * xs:short The largest exponent of the float values that can be 
        * contained in the array. The default is 38. Optional.
        */
        const short getMagnitude () const { return magnitude; }
        void setMagnitude ( const short val ) { magnitude = val; }

    };

    /**
     * Stores a homogenous array of integer values.
     * The <int_array> element stores the data values for generic use within 
     * the COLLADA schema. The arrays themselves are strongly typed but without 
     * semantics. They simply describe a sequence of integer values.
     */
    class IntArrayElement : public BaseArrayElement < int > 
    {

    private:

        /**
         * xs:integer The smallest integer value that can be contained in the 
         * array. The default is -2147483648. Optional.
         */
        int minInclusive;

        /**
         * xs:integer The largest integer value that can be contained in the 
         * array. The default is 2147483647. Optional.
         */
        int maxInclusive;

    public:

        /**
        * Constructor.
        */
        IntArrayElement () : BaseArrayElement () {};

        /**
        * Destructor.
        */
        virtual ~IntArrayElement () {};

        /**
        * xs:integer The smallest integer value that can be contained in the 
        * array. The default is -2147483648. Optional.
        */
        const int getMinInclusive () const { return minInclusive; }
        void setMinInclusive ( const int val ) { minInclusive = val; }

        /**
        * xs:integer The largest integer value that can be contained in the 
        * array. The default is 2147483647. Optional.
        */
        const int getMaxInclusive () const { return maxInclusive; }
        void setMaxInclusive ( const int val ) { maxInclusive = val; }

    };

    /**
     * Declares the storage for a homogenous array of Boolean values.
     * The <bool_array> element stores data values for generic use within the 
     * COLLADA schema. The arrays themselves are strongly typed but without 
     * semantics. They simply describe a sequence of XML Boolean values.
     */
    typedef BaseArrayElement < bool > BoolArrayElement;

    /**
     * Stores a homogenous array of symbolic name values.
     * The <Name_array> element stores name values as data for generic use 
     * within the COLLADA schema. The array itself is strongly typed but 
     * without semantics. It simply stores a sequence of XML name values.
     */
    typedef BaseArrayElement < String > NameArrayElement;

    /**
     * Declares the storage for a homogenous array of ID reference values.
     * The <IDREF_array> element stores string values that reference IDs within 
     * the instance document.
     */
    typedef BaseArrayElement < String > IDREFArrayElement;

}

#endif // __COLLADAFW_BASE_ARRAY_H__