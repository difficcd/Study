/* Requirements 
   - Concurrent Reads
   - Mutually Exclusive Write
   - Priorized Write
*/

typedef struct {
		int rlock_acquire ;
		int wlock_acquire ;
		int rlock_hold ;
		int wlock_hold ;

		pthread_mutex_t lock ;
		pthread_cond_t read_ready_cv ;
		pthread_cond_t write_ready_cv ;
	}
	rwlock_t ;



void rwlock_init(rwlock_t * m) ; 

void read_lock(rwlock_t * m) ;
void read_unlock(rwlock_t * m) ;

void write_lock(rwlock_t * m) ;
void write_unlock(rwlock_t * m) ;
