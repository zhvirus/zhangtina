// This file should be only included by vtl_geometry.h

namespace vtl{


//////////////////////////////////////////////////////////////////
// Quaternion, Exp/Log mapping
//////////////////////////////////////////////////////////////////
    template<class T, class V3>
        const Matrix4x4<T> Quaternion<T, V3>::GetMatrix() const
    {
	    T xx,xy,xz,xw,yy,yz,yw,zz,zw;
	    Matrix4x4<T>	m;

	    xx = v.x * v.x;
        xy = v.x * v.y;
        xz = v.x * v.z;
        xw = v.x * w;
                   
        yy = v.y * v.y;
        yz = v.y * v.z;
        yw = v.y * w;
                   
        zz = v.z * v.z;
        zw = v.z * w;


	    m[0][0] = 1 - 2 * ( yy + zz );
	    m[0][1] =     2 * ( xy - zw );
	    m[0][2] =     2 * ( xz + yw );
	    m[0][3] = (T)0.0;
              
	    m[1][0] =     2 * ( xy + zw );
	    m[1][1] = 1 - 2 * ( xx + zz );
	    m[1][2] =     2 * ( yz - xw );
	    m[1][3] = (T)0.0;
              
	    m[2][0] =     2 * ( xz - yw );
	    m[2][1] =     2 * ( yz + xw );
	    m[2][2] = 1 - 2 * ( xx + yy );
	    m[2][3] = (T)0.0;
              
	    m[3][0] = 0.0f;
	    m[3][1] = 0.0f;
	    m[3][2] = 0.0f;
	    m[3][3] = (T)1.0;

	    return m;
    }

    template<class T, class V3>
        Quaternion<T, V3>& Quaternion<T, V3>::SetMatrix(const Matrix4x4<T>& m)
    {
	    T	tr,s;
	    T	out[3];
	    int i,j,k;

	    static int next[3]= {1,2,0};
    	
	    tr = m[0, 0] + m[1, 1] + m[2, 2];

	    // check the diagonal
	    if( tr > 0 )
	    {
		    s = static_cast<T>(sqrt(tr+1.0 ));
		    w = s / 2 ;
		    s = (T)0.5 / s ;
		    x = ( m[2,1] - m[1,2] ) * s;
		    y = ( m[0,2] - m[2,0] ) * s;
		    z = ( m[1,0] - m[0,1] ) * s;
	    }
	    else
	    {		
		    // diagonal is negative
		    i=0;
		    if( m[1,1] >  m[0,0] )
		    {
			    i=1;
		    }
		    if( m[2,2] >  m[i,i] )
		    {
			    i=2;
		    }

		    j= next[i];
		    k= next[j];

		    s = m[i,i] - (m[j,j] + m[k,k]);

		    s = (T)(sqrt(s + 1.0) );

		    out[i] = s / 2;
		    s      = (T)0.5 / s;
		    out[j] = (m[i,j] + m[j,i]) * s;
		    out[k] = (m[i,k] + m[k,i]) * s;
		    w	   = (m[k,j] - m[j,k]) * s;

		    x = out[0];
		    y = out[1];
		    z = out[2];

	    }

	    return *this;
    }

    //Expotinal map
    template <class T> 
        const Quaternion<T> Exp(const Vector3<T>& vec)
    {
	    Quaternion<T> temp;
	    T theta= vec.Length();
	    temp.w= cos((T)0.5*theta);

	    if( theta < MIN_FLOAT)
	    {
		    // Taylor approximation 
		    T stheta= (T)0.5+ theta*theta /48;
		    temp.x = vec.x* stheta;
		    temp.y = vec.y* stheta;
		    temp.z = vec.z* stheta;
	    }
	    else
	    {
		    T _sin= sin(0.5*theta);
		    temp.x = vec.x *_sin/theta;
		    temp.y = vec.y *_sin/theta;
		    temp.z = vec.z *_sin/theta;
	    }
    	
	    return temp;
    }

    //logrithm map
    template <class T> 
        const Vector3<T> Log(const Quaternion <T>& src)
    {
	    T length = sqrt(src.x*src.x+src.y*src.y+src.z*src.z);	

	    if(IsZero(length))
	    {
		    return Vector3<T>(0,0,0);
	    }

	    T scale=(T)2*atan2(length,src.w)/length;
	    Vector3<T> TempVec = src * scale;

	    T newlength=TempVec.Length();
	    if( newlength > PI )
	    {
		    TempVec *= - (2*PI - newlength)/newlength;
	    }
	    return TempVec;
    }


}//namespace vtl