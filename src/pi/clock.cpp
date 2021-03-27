#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <sstream>

#include "clock.h"
#include "localy.h"
#include "cexception.h"

CLock::CLock()
{
}

void CLock::Get(string name)
{
	ostringstream	ost;
	int		key;

	nameLock = name;

	sop_lock[0].sem_num = 0;
	sop_lock[0].sem_op = 0;
	sop_lock[0].sem_flg = 0;
	sop_lock[1].sem_num = 0;
	sop_lock[1].sem_op = 1;
	sop_lock[1].sem_flg = 0;

	sop_unlock[0].sem_num = 0;
	sop_unlock[0].sem_op = -1;
	sop_unlock[0].sem_flg = 0;

	ost << LOCK_DIRECTORY << nameLock;
	if((key = ftok(ost.str().c_str(), 'A')) < 0)
	{
		ostringstream ost1;
		
		if(errno == 2) throw CException("keyfile error");
		
		ost1 << "key error" << ost.str();
		throw CException(ost1.str());
	}

	if((hSem = semget(key, 1, PERM | IPC_CREAT)) < 0) throw CException("semaphore error");
}

void CLock::Lock()
{
	if(hSem < 0) throw CException("semaphore error");
	semop(hSem, &sop_lock[0], 2);
}

void CLock::UnLock()
{
	if(hSem < 0) throw CException("semaphore error");
	semop(hSem, &sop_unlock[0], 1);
}

CLock::~CLock()
{
}

