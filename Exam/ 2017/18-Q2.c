// Problem 1

#define X_SIZE 40960
#define Y_SIZE 40960
#define V_SIZE 128
double *M,*V;
void main(int argc,char *argv[])
{
	int i,j,aux,pos;
	M=malloc(sizeof(double)*X_SIZE*Y_SIZE);
	V=malloc(sizeof(double)*V_SIZE);
	for (i=0;i<V_SIZE;i++) V[i]=0;
	for (i=0;i<X_SIZE;i++) ReadRowFromFile(M,i);
	// Main loop
	for (i=0;i<X_SIZE;i++){
		for (j=0;j<Y_SIZE;i++){
			aux=ComputeElement(M,i,j);
			pos=ComputePos(i,j);
			V[pos]=+aux;
		}
	}
}

// Parallel version:

#define X_SIZE 40960
#define Y_SIZE 40960
#define V_SIZE 128
double *M,*V;
void main(int argc,char *argv[])
{
	int i,j,aux,pos;
	M=malloc(sizeof(double)*X_SIZE*Y_SIZE);
	V=malloc(sizeof(double)*V_SIZE);
	for (i=0;i<V_SIZE;i++) V[i]=0;
	for (i=0;i<X_SIZE;i++) ReadRowFromFile(M,i);
	// Main loop

	#pragma omp Parallel
	{
		int i_start, i_end, BS;
		BS = X_SIZE/omp_get_num_threads();
		i_start = omp_get_thread_num() * (X_SIZE/omp_get_num_threads()); 	// Block i of the matrix with size X_SIZE/#threads
		if(id < (X_SIZE%omp_get_num_threads()) lower+=id; 					// If 
		i_end = lower + (X_SIZE/nump)+(id<(X_SIZE%nump))					// Calculate the end of the block
		for (i=0;i<X_SIZE;i++){
			for (j=0;j<Y_SIZE;i++){
				aux=ComputeElement(M,i,j);
				pos=ComputePos(i,j);
				#pragma omp atomic											// Only one thread can write on the vector V
				V[pos]=+aux;
			}
		}
	}
}

// Parallel version with data depencences 

#define X_SIZE 40960
#define Y_SIZE 40960
#define V_SIZE 128
double *M,*V;
void main(int argc,char *argv[])
{
	int i,j,aux,pos;
	M=malloc(sizeof(double)*X_SIZE*Y_SIZE);
	V=malloc(sizeof(double)*V_SIZE);
	for (i=0;i<V_SIZE;i++) V[i]=0;
	for (i=0;i<X_SIZE;i++) ReadRowFromFile(M,i);
	// Main loop

	#pragma omp Parallel
	{
		int i_start, i_end, BS;
		BS = X_SIZE/omp_get_num_threads();
		i_start = omp_get_thread_num() * (X_SIZE/omp_get_num_threads()); 	// Block i of the matrix with size X_SIZE/#threads
		if(id < (X_SIZE%omp_get_num_threads()) lower+=id; 					// If 
		i_end = lower + (X_SIZE/nump)+(id<(X_SIZE%nump))					// Calculate the end of the block
		for (i=0;i<X_SIZE;i++){
			for (j=0;j<Y_SIZE;i++){
				pos=ComputePos(i,j);
				if(pos >= i_start && pos < i_end){
					aux=ComputeElement(M,i,j);
					V[pos]=+aux;
				}
			}
		}
	}

	// Problem 2

	