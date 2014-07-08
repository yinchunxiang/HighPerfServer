PROJECT_HOME=../../../statserver
HIREDIS_INCLUDE=${PROJECT_HOME}/include/hiredis
HIREDIS_LIB=${PROJECT_HOME}/libs/hiredis

OUTPUT=redis_client


all:${OUTPUT}


${OUTPUT}: redis_client.o
	g++ redis_client.cc -o ${OUTPUT} -I${HIREDIS_INCLUDE} -L${HIREDIS_LIB} -lhiredis


