#pragma once 



    template <typename T> 
	
	class Singleton
    {
	
		protected:

			static T*  ms_Singleton;

    public:
        Singleton( void )
        {
				assert( !ms_Singleton );

			#if defined( _MSC_VER ) && _MSC_VER < 1200	 
				int offset = (int)(T*)1 - (int)(Singleton <T>*)(T*)1;
				ms_Singleton = (T*)((int)this + offset);
			#else
				ms_Singleton = static_cast< T* >( this );
			#endif
        }

        ~Singleton( void )
       {  
		   assert( ms_Singleton );  
		   ms_Singleton = 0;  
		}
       

		static T* getSingletonPtr( void )
		{ 
			if(ms_Singleton==NULL)
			{
				ms_Singleton	=	new T;
				return ms_Singleton;
			}
			else
				return ms_Singleton; 
		}

};

	template<typename T>
	T* Singleton<T>::ms_Singleton=NULL;

