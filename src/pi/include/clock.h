#ifndef __CLOCK__H__
#define __CLOCK__H__

#include <sys/types.h>
#include <string>
#include "localy.h"

using namespace std;

#define	PERM	0666

class CLock
{
	private:
		struct	sembuf	sop_lock[2];
		struct	sembuf	sop_unlock[1];
		string	nameLock;
		int	key, hSem;
	public:
			CLock();

		void	Get(string name);
		void	Lock();
		void	UnLock();

			~CLock();
};

#endif
