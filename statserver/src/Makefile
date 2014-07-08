PROJECT_HOME=../../statserver
JEEP_HOME=../../jeep
HIREDIS_INCLUDE=${PROJECT_HOME}/include/hiredis
HIREDIS_LIB=${PROJECT_HOME}/libs/hiredis

JSON_INCLUDE=${PROJECT_HOME}/include/jsoncpp
JSON_LIB=${PROJECT_HOME}/libs/jsoncpp

all: statserver

clean:
	rm -f statserver *.o
	
statserver: statserver.c akg.c conf.c collect.c md5.c base64.cc \
			${JEEP_HOME}/jeep_akg.c ${JEEP_HOME}/jeep_conn.c ${JEEP_HOME}/jeep_network.c \
			${JEEP_HOME}/jeep.c ${JEEP_HOME}/jeep_log.c ${JEEP_HOME}/jeep_util.c ${JEEP_HOME}/jeep_table.h

	g++ -g -O3 -Wall -fPIC -DLINUX64  -D__DEBUG__ \
			-o statserver \
			statserver.c akg.c conf.c collect.c md5.c base64.cc \
			${JEEP_HOME}/jeep_akg.c ${JEEP_HOME}/jeep_conn.c ${JEEP_HOME}/jeep_network.c \
			${JEEP_HOME}/jeep.c ${JEEP_HOME}/jeep_log.c ${JEEP_HOME}/jeep_util.c \
			-I/usr/local/include/ -I/usr/include/mysql -I${JEEP_HOME} -I${HIREDIS_INCLUDE} -I${JSON_INCLUDE} \
			-L/lib -L/usr/local/lib/ -L/usr/lib64/mysql/ -L${HIREDIS_LIB} -L${JSON_LIB} \
			-lpthread -levent -lmysqlclient -ldl -lhiredis ${PROJECT_HOME}/libs/jsoncpp/libjson_linux-gcc-4.1.2_libmt.a
