#ifndef _SINGLETON_H__
#define _SINGLETON_H__

#include <cassert>

namespace RE330 
{
	/** Singleton class.
    @remarks
		This implementation of a singleton class is slightly different from
		standard approaches. This is because the template will be instantiated
		with abstract classes. Therefore, we cannot call new(T()) here.
    */
	template <typename T> class Singleton
	{
	protected:
		static T* ms_Singleton;

	public:
		Singleton( void )
		{
			assert( !ms_Singleton );
			ms_Singleton = static_cast< T* >( this );
		}
	
		~Singleton( void )
		{  
			assert( ms_Singleton );  
			ms_Singleton = 0;  
		}

		static T& getSingleton( void )
		{	
			assert( ms_Singleton );  
			return ( *ms_Singleton ); 
		}

		static T* getSingletonPtr( void )
		{ 
			return ms_Singleton;
		}
	};

}

#endif
